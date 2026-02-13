// SBL Virtual Machine
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include "sblop.h"
#include "sblinst.h"
#include "sblconst.h"

#define abs(x)   ((x) < 0 ? (-x) : (x))
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#define sign(x)  ((x) == 0 ? 0 : (x) < 0 ? -1 : 1)

#define STACK_SIZE 512
#define STACK_TYPE float
typedef STACK_TYPE stype_t ;
typedef struct stack {
    int32_t top;
    stype_t data[STACK_SIZE];
}stack_t;

void stk_push(stack_t *s,float v){
    s->data[++s->top] = v;
}

float stk_pop(stack_t *s){
    return s->data[s->top--];
}

void stk_trace(stack_t* s){
    float max = 0;
    int width = 0;
    for(int32_t i = s->top; i >= 0; i--) max = max(abs(s->data[i]),max);
    width = (int)roundf(1+logf(max)/logf(10.0f));
    for(int i = 0; i < width + 3 + 2 + 2; i++) putchar('-');
    putchar('\n');
    for(int32_t i = s->top; i >= 0; i--) printf("| %*.02f |\n",width+3,s->data[i]);
    for(int i = 0; i < width + 3 + 2 + 2; i++) putchar('-');
    putchar('\n');
}

typedef struct sbl_bin_header{
    char magic[2];  // SB
    uint16_t insts;  //
    uint32_t ctb_off;
}sblh_t;

typedef struct sbl_machine{
    // program data
    sblh_t      info;
    ilist_t     insts;
    constabl_t  consts;
    // exec data
    bool halt;
    uint16_t ip;
}sblm_t;

#define cast(T, v) (T)(v)

int main(int argc, char* argv[]){
    if (argc < 2) return -1;
    FILE* in = fopen(argv[1],"rb");
    int dbg = argv[2] ? argv[2][0] == 'g' ? 1 : 0 : 0;

    stack_t s = {.top = -1, .data = {0}};
    sblm_t M = {0};
    fread(&M.info,sizeof(M.info),1,in);

    printf("%c%c: %hu, %u\n",M.info.magic[0],M.info.magic[1],M.info.insts,M.info.ctb_off);

    M.insts.size = M.info.insts;
    fread(M.insts.data,sizeof(inst_t),M.info.insts,in);

    long cur = ftell(in);
    fseek(in,0L,SEEK_END);
    long end = ftell(in);

    long remain = end - cur;
    fseek(in,cur,SEEK_SET);
    
    M.consts.size = remain / sizeof(float);
    fread(M.consts.data,1,remain,in);

    while(!M.halt){
        uint32_t inst = M.insts.data[M.ip];
        uint16_t op = cast(uint16_t,DEC_OP(inst));
        switch (op) {
        case OP_NONE:
            M.halt = true;
            break;
        case OP_ADD: {
             float a = stk_pop(&s);
             float b = stk_pop(&s);
             stk_push(&s,a + b);
             break;
         }
        case OP_SUB:{
             float a = stk_pop(&s);
             float b = stk_pop(&s);
             stk_push(&s,a - b);
             break;
        }
        case OP_MUL:{
             float a = stk_pop(&s);
             float b = stk_pop(&s);
             stk_push(&s,a * b);
             break;
        }
        case OP_DIV:{
            float a = stk_pop(&s);
            float b = stk_pop(&s);
            stk_push(&s, a / b);
            break;
        }
        case OP_PUSH:{
                stype_t x = M.consts.data[cast(uint32_t,DEC_K(inst))];
                stk_push(&s,x);
                break;
        }
        case OP_POP:{
            stk_pop(&s);
            break;
        }
        case OP_SWAP:{
            stype_t a = stk_pop(&s);
            stype_t b = stk_pop(&s);
            stk_push(&s,a);
            stk_push(&s,b);
            break;
        }
        case OP_DUP:{
            stype_t x = stk_pop(&s);
            stk_push(&s,x);
            stk_push(&s,x);
            break;
        }
        case OP_OVER:{
            stype_t a = stk_pop(&s);
            stype_t b = stk_pop(&s);

            stk_push(&s,b);
            stk_push(&s,a);
            stk_push(&s,b);
            break;
        }
        case OP_JUMP:{
            stype_t x = M.consts.data[cast(uint32_t,DEC_K(inst))];
            M.ip += ((int)x - sign(x));
            break;
        }
        case OP_HOP:{
            stype_t c = stk_pop(&s);
            if(c) M.ip++;
            break;
        }
        case OP_GT:{
            stype_t a = stk_pop(&s);
            stype_t b = stk_pop(&s);
            stk_push(&s,a > b);
            break;
       }
        case OP_ECHO:{
            stype_t x = M.consts.data[cast(uint32_t,DEC_K(inst))];
            if ((int)x == 0) printf("%f\n",s.data[s.top]);
            else for(int i = 0; i < (int)x; i++) printf("%f\n",s.data[s.top - i]);
            break;
        }
        default:
            fprintf(stderr,"[ ERROR ]: unknown opcode %x at %hu", op, M.ip);
            return -1;
        }

        if (dbg){
            stk_trace(&s);
            // printf("ip: %d | op: %d \n",M.ip,DEC_OP(inst));
            // putc('[',stdout);
            // for(int i = 0; i < s.top; i++){
            //     printf("%.02f",s.data[i]);
            //     if(i + 1 < s.top) putc(',',stdout);
            // }
            // puts("]");
            getc(stdin);
        }

        fflush(stdout);
        M.ip++;

    }

    return 0;
}
