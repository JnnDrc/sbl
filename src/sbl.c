// SBL Virtual Machine
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "sblop.h"
#include "sblinst.h"
#include "sblconst.h"

#include "sblvm.h"

#define cast(T, v) (T)(v)

#define SETF(fs, f) ((fs) |= (f))
#define HASF(fs, f) ((fs) & (f))
#define F_DEBUG 1

#define streq(s1,s2) !strcmp(s1,s2)

int main(int argc, char* argv[]){
    uint8_t flags = 0;
    if (argc < 2){
        fprintf(stderr,"USAGE: %s input [flags]", argv[0]);
        return -1;
    }
    FILE* in = fopen(argv[1],"rb");
    for (int i = 2; i < argc; i++){
        if(streq(argv[i],"--debug")) SETF(flags,F_DEBUG);
    }
    
    sblvm_t vm = {0};
    int r = sblvm_load(&vm,in);
    if (r == 1){
        fprintf(stderr,"ERROR: can't run file, not sbinl");
        return -1;
    }

    while(!vm.halt){
        uint32_t inst = vm.insts.data[vm.ip];
        uint16_t op = DEC_OP(inst);
        switch (op) {
            case OP_NONE:
                vm.halt = true;
                break;
            case OP_ADD: {
                 float a = stk_pop(&vm.stack);
                 float b = stk_pop(&vm.stack);
                 stk_push(&vm.stack,a + b);
                 break;
             }
            case OP_SUB:{
                 float a = stk_pop(&vm.stack);
                 float b = stk_pop(&vm.stack);
                 stk_push(&vm.stack,a - b);
                 break;
            }
            case OP_MUL:{
                 float a = stk_pop(&vm.stack);
                 float b = stk_pop(&vm.stack);
                 stk_push(&vm.stack,a * b);
                 break;
            }
            case OP_DIV:{
                float a = stk_pop(&vm.stack);
                float b = stk_pop(&vm.stack);
                stk_push(&vm.stack, a / b);
                break;
            }
            case OP_PUSH:{
                    stype_t x = vm.consts.data[cast(uint32_t,DEC_K(inst))];
                    stk_push(&vm.stack,x);
                    break;
            }
            case OP_POP:{
                stk_pop(&vm.stack);
                break;
            }
            case OP_SWAP:{
                stype_t a = stk_pop(&vm.stack);
                stype_t b = stk_pop(&vm.stack);
                stk_push(&vm.stack,a);
                stk_push(&vm.stack,b);
                break;
            }
            case OP_DUP:{
                stype_t x = stk_pop(&vm.stack);
                stk_push(&vm.stack,x);
                stk_push(&vm.stack,x);
                break;
            }
            case OP_OVER:{
                stype_t a = stk_pop(&vm.stack);
                stype_t b = stk_pop(&vm.stack);

                stk_push(&vm.stack,b);
                stk_push(&vm.stack,a);
                stk_push(&vm.stack,b);
                break;
            }
            case OP_LROT:{
                stype_t tmp = vm.stack.data[vm.stack.top - 2];
                vm.stack.data[vm.stack.top - 2] = vm.stack.data[vm.stack.top - 1];
                vm.stack.data[vm.stack.top - 1] = vm.stack.data[vm.stack.top];
                vm.stack.data[vm.stack.top] = tmp;
                break;
            }
            case OP_RROT:{
                // 1 2 2 -> 2 1 2
                // a b c -> c a b
                stype_t tmp = vm.stack.data[vm.stack.top];
                vm.stack.data[vm.stack.top] = vm.stack.data[vm.stack.top - 1];
                vm.stack.data[vm.stack.top - 1] = vm.stack.data[vm.stack.top - 2];
                vm.stack.data[vm.stack.top - 2] = tmp;
                break;
            }
            case OP_JUMP:{
                stype_t x = vm.consts.data[cast(uint32_t,DEC_K(inst))];
                vm.ip += (int)x - 1;
                break;
            }
            case OP_HOP:{
                stype_t c = stk_pop(&vm.stack);
                if(c) vm.ip++;
                break;
            }
            case OP_GT:{
                stype_t a = stk_pop(&vm.stack);
                stype_t b = stk_pop(&vm.stack);
                stk_push(&vm.stack,a > b);
                break;
            }
            case OP_POW:{
                stype_t a = stk_pop(&vm.stack);
                stype_t b = stk_pop(&vm.stack);
                stk_push(&vm.stack, powf(a, b));
                break;
            }
            case OP_SQR:{
                stype_t x = stk_pop(&vm.stack);
                stk_push(&vm.stack, x*x);
                break;
            }
            case OP_SQRT:{
                stype_t x = stk_pop(&vm.stack);
                stk_push(&vm.stack,sqrtf(x));
                break;
            }
            case OP_ECHO:{
                stype_t x = vm.consts.data[cast(uint32_t,DEC_K(inst))];
                if ((int)x == 0) printf("%f\n",vm.stack.data[vm.stack.top]);
                else{
                    printf("|");
                    for(int i = vm.stack.top - (x - 1); i <= vm.stack.top ;i++) printf("%f ",vm.stack.data[i]);
                    printf("\n");
                }
                fflush(stdout);
                break;
            }
            default:
                fprintf(stderr,"ERROR: unknown opcode %d at %zu", op, vm.ip);
                return -1;
        }

        if (HASF(flags,F_DEBUG)){
            printf("ip: %zu | op: %s \n",vm.ip,sblo_op_string(DEC_OP(inst)));
            printf("[");
            for(int i = 0; i <= vm.stack.top; i++){
                printf("%.02f ",vm.stack.data[i]);
            }
            printf("]\n");
            getc(stdin);
            fflush(stdout);
        }

        vm.ip++;
    }

    return 0;
}
