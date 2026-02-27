#include <string.h>

#include "sblvm.h"
#include "sblop.h"
#include "sblvm_op.h"
#include "sblinst.h"

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

oprt_f operations[1024] = {
    [OP_NONE]  = op_none,
    [OP_NOP]   = op_nop,
    [OP_HALT]  = op_halt,
    [OP_JUMP]  = op_jump,
    [OP_HOP]   = op_hop,
    [OP_CALL]  = op_call,
    [OP_RET]   = op_ret,

    [OP_IADD]  = op_iadd,
    [OP_ISUB]  = op_isub,
    [OP_IMUL]  = op_imul,
    [OP_IDIV]  = op_idiv,
    [OP_IMOD]  = op_imod,
    [OP_IINC]  = op_iinc,
    [OP_IDEC]  = op_idec,
    [OP_IPOW]  = op_ipow,
    [OP_ISQR]  = op_isqr,

    [OP_PUSH] = op_push,
    [OP_POP]  = op_pop,
    [OP_SWAP] = op_swap,
    [OP_DUP]  = op_dup,
    [OP_OVER] = op_over,
    [OP_LROT] = op_lrot,
    [OP_RROT] = op_rrot,
    
    [OP_EQ]  = op_eq,
    [OP_NE]  = op_ne,
    [OP_IGT] = op_igt,
    [OP_ILT] = op_ilt,
    [OP_IGE] = op_ige,
    [OP_ILE] = op_ile,

    [OP_AND] = op_and,
    [OP_OR]  = op_or,
    [OP_NOT] = op_not,

    [OP_TOP]   = op_top,
    [OP_PUT]   = op_put,
    [OP_DUMP]  = op_dump,
    [OP_TRACE] = op_trace,
    [OP_PUTS]  = op_puts,
};

#define EXEC_ERR_UNKNOWN_OPC 1
int sblvm_exec(sblvm_t* vm){
    uint32_t inst = vm->insts.data[vm->ip];
    uint16_t op = DEC_OP(inst);

    operations[op](vm,inst);

    vm->ip++;
    return 0;
}
