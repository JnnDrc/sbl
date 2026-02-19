#include <string.h>
#include <math.h>


#include "sbldef.h"
#include "sblinst.h"
#include "sblop.h"
#include "sblvm.h"

int sblvm_load(sblvm_t* vm, FILE* fp){
    stack_init(&vm->stack);
    ilist_init(&vm->insts);
    constabl_init(&vm->consts);

    fread(&vm->bin_info,sizeof(sblbinh_t),1,fp);
    if (strncmp(vm->bin_info.magic,"SB",2)) return SBL_ERR_NOT_SLB;

    vm->insts.size = vm->bin_info.insts;
    fread(vm->insts.data,sizeof(inst_t),vm->bin_info.insts,fp);

    long cur = ftell(fp);
    fseek(fp,0L,SEEK_END);
    long end = ftell(fp);

    long remain = end - cur;
    fseek(fp,cur,SEEK_SET);

    vm->consts.size = remain / sizeof(float);
    fread(vm->consts.data,1,remain,fp);

    return 0;
}

#define EXEC_ERR_UNKNOWN_OPC 1
int sblvm_exec(sblvm_t* vm){
    uint32_t inst = vm->insts.data[vm->ip];
    uint16_t op = DEC_OP(inst);
    switch (op) {
        case OP_NONE:
            vm->halt = true;
            break;
        case OP_ADD: {
             float a = stk_pop(&vm->stack);
             float b = stk_pop(&vm->stack);
             stk_push(&vm->stack,a + b);
             break;
         }
        case OP_SUB:{
             float a = stk_pop(&vm->stack);
             float b = stk_pop(&vm->stack);
             stk_push(&vm->stack,a - b);
             break;
        }
        case OP_MUL:{
             float a = stk_pop(&vm->stack);
             float b = stk_pop(&vm->stack);
             stk_push(&vm->stack,a * b);
             break;
        }
        case OP_DIV:{
            float a = stk_pop(&vm->stack);
            float b = stk_pop(&vm->stack);
            stk_push(&vm->stack, a / b);
            break;
        }
        case OP_PUSH:{
                stype_t x = vm->consts.data[cast(uint32_t,DEC_K(inst))];
                stk_push(&vm->stack,x);
                break;
        }
        case OP_POP:{
            stk_pop(&vm->stack);
            break;
        }
        case OP_SWAP:{
            stype_t a = stk_pop(&vm->stack);
            stype_t b = stk_pop(&vm->stack);
            stk_push(&vm->stack,a);
            stk_push(&vm->stack,b);
            break;
        }
        case OP_DUP:{
            stype_t x = stk_pop(&vm->stack);
            stk_push(&vm->stack,x);
            stk_push(&vm->stack,x);
            break;
        }
        case OP_OVER:{
            stype_t a = stk_pop(&vm->stack);
            stype_t b = stk_pop(&vm->stack);

            stk_push(&vm->stack,b);
            stk_push(&vm->stack,a);
            stk_push(&vm->stack,b);
            break;
        }
        case OP_LROT:{
            stype_t tmp = vm->stack.data[vm->stack.top - 2];
            vm->stack.data[vm->stack.top - 2] = vm->stack.data[vm->stack.top - 1];
            vm->stack.data[vm->stack.top - 1] = vm->stack.data[vm->stack.top];
            vm->stack.data[vm->stack.top] = tmp;
            break;
        }
        case OP_RROT:{
            // 1 2 2 -> 2 1 2
            // a b c -> c a b
            stype_t tmp = vm->stack.data[vm->stack.top];
            vm->stack.data[vm->stack.top] = vm->stack.data[vm->stack.top - 1];
            vm->stack.data[vm->stack.top - 1] = vm->stack.data[vm->stack.top - 2];
            vm->stack.data[vm->stack.top - 2] = tmp;
            break;
        }
        case OP_JUMP:{
            stype_t x = vm->consts.data[cast(uint32_t,DEC_K(inst))];
            vm->ip += (int)x - 1;
            break;
        }
        case OP_HOP:{
            stype_t c = stk_pop(&vm->stack);
            if(c) vm->ip++;
            break;
        }
        case OP_GT:{
            stype_t a = stk_pop(&vm->stack);
            stype_t b = stk_pop(&vm->stack);
            stk_push(&vm->stack,a > b);
            break;
        }
        case OP_POW:{
            stype_t a = stk_pop(&vm->stack);
            stype_t b = stk_pop(&vm->stack);
            stk_push(&vm->stack, powf(a, b));
            break;
        }
        case OP_SQR:{
            stype_t x = stk_pop(&vm->stack);
            x = x * x;
            stk_push(&vm->stack, x);
            break;
        }
        case OP_SQRT:{
            stype_t x = stk_pop(&vm->stack);
            x = sqrtf(x);
            stk_push(&vm->stack,x);
            break;
        }
        case OP_ECHO:{
            stype_t x = vm->consts.data[cast(uint32_t,DEC_K(inst))];
            if ((int)x == 0) printf("%.02f\n",vm->stack.data[vm->stack.top]);
            else{
                printf("|");
                for(int i = vm->stack.top - (x - 1); i <= vm->stack.top ;i++) printf("%.02f ",vm->stack.data[i]);
                printf("\n");
            }
            fflush(stdout);
            break;
        }
        case OP_PRINT:{
            printf("%.02f\n",vm->stack.data[vm->stack.top]);
        }
        default:
            return EXEC_ERR_UNKNOWN_OPC;
    }
    vm->ip++;
    return 0;
}
