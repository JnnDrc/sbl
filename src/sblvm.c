#include <string.h>
#include <math.h>


#include "sbldef.h"
#include "sblinst.h"
#include "sblop.h"
#include "sblvm.h"

sblbinh_t sbl_make_info(ilist_t *il, constabl_t *ct, lablist_t *ll){
    (void) ct;
    sblbinh_t bi;
    // magic number
    bi.magic[0] = 'S';
    bi.magic[1] = 'B';

    // instruction count
    bi.insts = il->size;
    // const table offset
    bi.ctb_off = il->size * sizeof(*il->data);
    int32_t start = label_find(ll,"main");          // start address
    if (start < 0) start = 0;
    bi.start = start;
    bi.version = SBL_MAKE_VERSION(SBL_MAJOR,SBL_MINOR);
    return bi;
}

int sblvm_load(sblvm_t* vm, FILE* fp){
    stack_init(&vm->data);
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

    vm->consts.size = remain / sizeof(sblval_t);
    fread(vm->consts.data,1,remain,fp);

    vm->ip   = vm->bin_info.start;
    vm->halt = false;

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
             sblval_t a = stk_pop(&vm->data);
             sblval_t b = stk_pop(&vm->data);
             stk_push(&vm->data,sblnum(a.as.num + b.as.num));
             break;
         }
        case OP_SUB:{
             sblval_t a = stk_pop(&vm->data);
             sblval_t b = stk_pop(&vm->data);
             stk_push(&vm->data,sblnum(a.as.num - b.as.num));
             break;
        }
        case OP_MUL:{
             sblval_t a = stk_pop(&vm->data);
             sblval_t b = stk_pop(&vm->data);
             stk_push(&vm->data,sblnum(a.as.num * b.as.num));
             break;
        }
        case OP_DIV:{
            sblval_t a = stk_pop(&vm->data);
            sblval_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblnum(a.as.num / b.as.num));
            break;
        }
        case OP_MOD:{
            sblval_t a = stk_pop(&vm->data);
            sblval_t b = stk_pop(&vm->data);
            stk_push(&vm->data, sblnum(fmod(a.as.num,b.as.num)));
            break;
        }
        case OP_INC:{
            sblval_t a = stk_pop(&vm->data);
            stk_push(&vm->data,sblnum(a.as.num + 1));
            break;
        }
        case OP_DEC:{
            sblval_t a = stk_pop(&vm->data);
            stk_push(&vm->data,sblnum(a.as.num - 1));
            break;
        }
        case OP_SUM:{
            sblval_t n = stk_pop(&vm->data);
            sblval_t s = sblnum(0);
            for(int i = 0; i < n.as.num; i++) s.as.num += stk_pop(&vm->data).as.num;
            stk_push(&vm->data,s);
            break;
        }
        case OP_PROD:{
            sblval_t n = stk_pop(&vm->data);
            sblval_t p = sblnum(1);
            for(int i = 0; i < n.as.num; i++) p.as.num *= stk_pop(&vm->data).as.num;
            stk_push(&vm->data,p);
            break;
        }
        case OP_PUSH:{
                sblval_t x = vm->consts.data[cast(uint32_t,DEC_K(inst))];
                stk_push(&vm->data,x);
                break;
        }
        case OP_POP:{
            stk_pop(&vm->data);
            break;
        }
        case OP_SWAP:{
            sblval_t a = stk_pop(&vm->data);
            sblval_t b = stk_pop(&vm->data);
            stk_push(&vm->data,a);
            stk_push(&vm->data,b);
            break;
        }
        case OP_DUP:{
            sblval_t x = stk_pop(&vm->data);
            stk_push(&vm->data,x);
            stk_push(&vm->data,x);
            break;
        }
        case OP_OVER:{
            sblval_t a = stk_pop(&vm->data);
            sblval_t b = stk_pop(&vm->data);

            stk_push(&vm->data,b);
            stk_push(&vm->data,a);
            stk_push(&vm->data,b);
            break;
        }
        case OP_LROT:{
            sblval_t tmp = vm->data.data[vm->data.sp - 3];
            vm->data.data[vm->data.sp - 3] = vm->data.data[vm->data.sp - 2];
            vm->data.data[vm->data.sp - 2] = vm->data.data[vm->data.sp - 1];
            vm->data.data[vm->data.sp - 1] = tmp;
            break;
        }
        case OP_RROT:{
            sblval_t tmp = vm->data.data[vm->data.sp - 1];
            vm->data.data[vm->data.sp - 1] = vm->data.data[vm->data.sp - 2];
            vm->data.data[vm->data.sp - 2] = vm->data.data[vm->data.sp - 3];
            vm->data.data[vm->data.sp - 3] = tmp;
            break;
        }
        case OP_JUMP:{
            sblval_t x = vm->consts.data[cast(uint32_t,DEC_K(inst))];
            vm->ip += (int32_t)x.as.off - 1;
            break;
        }
        case OP_HOP:{
            sblval_t c = stk_pop(&vm->data);
            if(c.as.off) vm->ip++;
            break;
        }
        case OP_GT:{
            sblval_t a = stk_pop(&vm->data);
            sblval_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblnum(a.as.num > b.as.num));
            break;
        }
        case OP_LT:{
            sblval_t a = stk_pop(&vm->data);
            sblval_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblnum(a.as.num < b.as.num));
            break;
        }
        case OP_GE:{
            sblval_t a = stk_pop(&vm->data);
            sblval_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblnum(a.as.num >= b.as.num));
            break;
        }
        case OP_LE:{
            sblval_t a = stk_pop(&vm->data);
            sblval_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblnum(a.as.num <= b.as.num));
            break;
        }
        case OP_EQ:{
            sblval_t a = stk_pop(&vm->data);
            sblval_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblnum(sblval_eq(a,b)));
            break;
        }
        case OP_NE:{
            sblval_t a = stk_pop(&vm->data);
            sblval_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblnum(!sblval_eq(a,b)));
            break;
        }
        case OP_POW:{
            sblval_t a = stk_pop(&vm->data);
            sblval_t b = stk_pop(&vm->data);
            stk_push(&vm->data, sblnum(pow(a.as.num, b.as.num)));
            break;
        }
        case OP_SQR:{
            sblval_t x = stk_pop(&vm->data);
            x.as.num = x.as.num * x.as.num;
            stk_push(&vm->data, x);
            break;
        }
        case OP_SQRT:{
            sblval_t x = stk_pop(&vm->data);
            x.as.num = sqrt(x.as.num);
            stk_push(&vm->data,x);
            break;
        }
        case OP_CALL:{
            sblval_t ip = vm->consts.data[cast(uint32_t,DEC_K(inst))];
            stk_push(&vm->call,sblint(vm->ip));
            vm->ip += (int)ip.as.off - 1;
            break;
        }
        case OP_RET:{
            vm->ip = stk_pop(&vm->call).as.off;
            break;
        }
        case OP_TOP:{
            printf("%.02f\n",vm->data.data[vm->data.sp - 1].as.num);
            break;
        }
        default:
            return EXEC_ERR_UNKNOWN_OPC;
    }
    vm->ip++;
    return 0;
}
