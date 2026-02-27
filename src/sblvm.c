#include <string.h>
#include <math.h>


#include "sbldef.h"
#include "sblinst.h"
#include "sblop.h"
#include "sblvm.h"
#include "utils.h"

sblbinh_t sbl_make_info(ilist_t *il, constabl_t *ct, lablist_t *ll,statbuf_t* sb){
    (void) sb;
    sblbinh_t bi;
    // magic number
    bi.magic[0] = 'S';
    bi.magic[1] = 'B';
    // version
    bi.version = SBL_MAKE_VERSION(SBL_MAJOR,SBL_MINOR);
    // instruction count
    bi.insts = il->size;
    // const table offset
    bi.ctb_off = il->size * sizeof(*il->data);
    bi.stb_off = (il->size * sizeof(*il->data)) + (ct->size * sizeof(*ct->data));
    int32_t start = label_find(ll,"main");          // start address
    if (start < 0) start = 0;
    bi.start = start;
    return bi;
}

int sbl_load_info(sblbinh_t* bi, FILE* fp){
    fread(bi,sizeof(sblbinh_t),1,fp);
    if (strncmp(bi->magic,"SB",2)) return SBL_ERR_NOT_SLB;
    return 0;
}

int sblvm_load(sblvm_t* vm, FILE* fp){
    stack_init(&vm->data);
    ilist_init(&vm->insts);
    constabl_init(&vm->consts);
    sb_init(&vm->statmem);

    int err = sbl_load_info(&vm->bin_info,fp);
    if(err) return err;

    vm->insts.size = vm->bin_info.insts;
    fread(vm->insts.data,sizeof(inst_t),vm->bin_info.insts,fp);
    
    long ctb_bytes  = vm->bin_info.stb_off - vm->bin_info.ctb_off;
    vm->consts.size = ctb_bytes / sizeof(sblcell_t);
    fread(vm->consts.data,1,ctb_bytes,fp);

    long cur = ftell(fp);
    fseek(fp,0L,SEEK_END);
    long end = ftell(fp);

    long remain = end - cur;
    fseek(fp,cur,SEEK_SET);

    vm->statmem.size = remain;
    fread(vm->statmem.data,1,remain,fp);

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
        case OP_IADD: {
             sblcell_t a = stk_pop(&vm->data);
             sblcell_t b = stk_pop(&vm->data);
             stk_push(&vm->data,sblint(a.as.i + b.as.i));
             break;
         }
        case OP_ISUB:{
             sblcell_t a = stk_pop(&vm->data);
             sblcell_t b = stk_pop(&vm->data);
             stk_push(&vm->data,sblint(a.as.i - b.as.i));
             break;
        }
        case OP_IMUL:{
             sblcell_t a = stk_pop(&vm->data);
             sblcell_t b = stk_pop(&vm->data);
             stk_push(&vm->data,sblint(a.as.i * b.as.i));
             break;
        }
        case OP_IDIV:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblint(a.as.i / b.as.i));
            break;
        }
        case OP_IMOD:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);
            stk_push(&vm->data, sblint(a.as.i % b.as.i));
            break;
        }
        case OP_IINC:{
            sblcell_t a = stk_pop(&vm->data);
            stk_push(&vm->data,sblint(a.as.i + 1));
            break;
        }
        case OP_IDEC:{
            sblcell_t a = stk_pop(&vm->data);
            stk_push(&vm->data,sblint(a.as.i - 1));
            break;
        }
        case OP_IPOW:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);
            stk_push(&vm->data, sblint(powll(a.as.i, b.as.i)));
            break;
        }
        case OP_ISQR:{
            sblcell_t x = stk_pop(&vm->data);
            x.as.i = x.as.i * x.as.i;
            stk_push(&vm->data, x);
            break;
        }
        case OP_PUSH:{
                sblcell_t x = vm->consts.data[cast(uint32_t,DEC_K(inst))];
                stk_push(&vm->data,x);
                break;
        }
        case OP_POP:{
            stk_pop(&vm->data);
            break;
        }
        case OP_SWAP:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);
            stk_push(&vm->data,a);
            stk_push(&vm->data,b);
            break;
        }
        case OP_DUP:{
            sblcell_t x = stk_pop(&vm->data);
            stk_push(&vm->data,x);
            stk_push(&vm->data,x);
            break;
        }
        case OP_OVER:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);

            stk_push(&vm->data,b);
            stk_push(&vm->data,a);
            stk_push(&vm->data,b);
            break;
        }
        case OP_LROT:{
            sblcell_t tmp = vm->data.data[vm->data.sp - 3];
            vm->data.data[vm->data.sp - 3] = vm->data.data[vm->data.sp - 2];
            vm->data.data[vm->data.sp - 2] = vm->data.data[vm->data.sp - 1];
            vm->data.data[vm->data.sp - 1] = tmp;
            break;
        }
        case OP_RROT:{
            sblcell_t tmp = vm->data.data[vm->data.sp - 1];
            vm->data.data[vm->data.sp - 1] = vm->data.data[vm->data.sp - 2];
            vm->data.data[vm->data.sp - 2] = vm->data.data[vm->data.sp - 3];
            vm->data.data[vm->data.sp - 3] = tmp;
            break;
        }
        case OP_JUMP:{
            sblcell_t x = vm->consts.data[cast(uint32_t,DEC_K(inst))];
            vm->ip += (int32_t)x.as.i - 1;
            break;
        }
        case OP_HOP:{
            sblcell_t c = stk_pop(&vm->data);
            if(c.as.u) vm->ip++;
            break;
        }
        case OP_EQ:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblbool(sblcell_eq(a,b)));
            break;
        }
        case OP_NE:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblbool(!sblcell_eq(a,b)));
            break;
        }
        case OP_IGT:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblint(a.as.i > b.as.i));
            break;
        }
        case OP_ILT:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblbool(a.as.i < b.as.i));
            break;
        }
        case OP_IGE:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblbool(a.as.i >= b.as.i));
            break;
        }
        case OP_ILE:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblbool(a.as.i <= b.as.i));
            break;
        }
        case OP_AND:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblbool(a.as.i && b.as.i));
            break;
        }
        case OP_OR:{
            sblcell_t a = stk_pop(&vm->data);
            sblcell_t b = stk_pop(&vm->data);
            stk_push(&vm->data,sblbool(a.as.i || b.as.i));
            break;
        }
        case OP_NOT:{
            sblcell_t a = stk_pop(&vm->data);
            stk_push(&vm->data,sblbool(!a.as.u));
            break;
        }
        case OP_CALL:{
            sblcell_t off = vm->consts.data[cast(uint32_t,DEC_K(inst))];
            stk_push(&vm->call,sbluint(vm->ip));
            vm->ip += (int)off.as.i - 1;
            break;
        }
        case OP_RET:{
            vm->ip = stk_pop(&vm->call).as.u;
            break;
        }
        case OP_TOP:{
            sblcell_print(vm->data.data[vm->data.sp - 1]);
            break;
        }
        case OP_PUT:{
            sblcell_t c = vm->data.data[vm->data.sp - 1];
            if(c.type != CELL_INT && c.type != CELL_UINT){
                printf("ERROR: Tried to print non integer as character");
            }
            printf("%c",(char)vm->data.data[vm->data.sp - 1].as.i);
            break;
        }
        case OP_DUMP:{
            sblcell_t x = stk_pop(&vm->data);
            sblcell_print(x);
            break;
        }
        case OP_TRACE:{
            stk_trace(&vm->data);
            break;
        }
        case OP_PUTS:{
            uint32_t off = vm->data.data[vm->data.sp - 1].as.u;
            printf("%s",vm->statmem.data + off);
            break;
        }
        default:
            return EXEC_ERR_UNKNOWN_OPC;
    }
    vm->ip++;
    return 0;
}
