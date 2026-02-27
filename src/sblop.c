#include "sblop.h"
#include "sbldef.h"

// str-op table
// first entry for an op is it principal representation
static struct sblo_str_op sblo_opstr_table[] = {
    {.str = "none", .op = OP_NONE},
    {.str = "nop",  .op = OP_NOP},
    {.str = "halt", .op = OP_HALT},

    {.str = "jump", .op = OP_JUMP},{.str = "jmp",  .op = OP_JUMP},
    {.str = "hop",  .op = OP_HOP},

    {.str = "call", .op = OP_CALL},
    {.str = "ret",  .op = OP_RET},{.str = ";",    .op = OP_RET},
    
    {.str = "iadd",  .op = OP_IADD},{.str = "+",    .op = OP_IADD},
    {.str = "add",   .op = OP_IADD},
    {.str = "isub",  .op = OP_ISUB},{.str = "-",    .op = OP_ISUB},
    {.str = "sub",   .op = OP_ISUB},
    {.str = "imul",  .op = OP_IMUL},{.str = "*",    .op = OP_IMUL},
    {.str = "mul",   .op = OP_IMUL},
    {.str = "idiv",  .op = OP_IDIV},{.str = "/",    .op = OP_IDIV},
    {.str = "div",   .op = OP_IDIV},
    {.str = "imod",  .op = OP_IMOD},{.str = "%",    .op = OP_IMOD},
    {.str = "mod",   .op = OP_IMOD},
    {.str = "iinc",  .op = OP_IINC},{.str = "++",   .op = OP_IINC},
    {.str = "inc",   .op = OP_IINC},
    {.str = "idec",  .op = OP_IDEC},{.str = "--",   .op = OP_IDEC},
    {.str = "dec",   .op = OP_IDEC},
    {.str = "ipow",  .op = OP_IPOW},{.str = "**",   .op = OP_IPOW},
    {.str = "pow",   .op = OP_IPOW},
    {.str = "isqr",  .op = OP_ISQR},
    {.str = "sqr",   .op = OP_ISQR},

    {.str = "uadd",  .op = OP_UADD},
    {.str = "usub",  .op = OP_USUB},
    {.str = "umul",  .op = OP_UMUL},
    {.str = "udiv",  .op = OP_UDIV},
    {.str = "umod",  .op = OP_UMOD},
    {.str = "uinc",  .op = OP_UINC},
    {.str = "udec",  .op = OP_UDEC},
    {.str = "upow",  .op = OP_UPOW},
    {.str = "usqr",  .op = OP_USQR},

    {.str = "fadd",  .op = OP_FADD},{.str = "+.",  .op = OP_FADD},
    {.str = "fsub",  .op = OP_FSUB},{.str = "-.",  .op = OP_FSUB},
    {.str = "fmul",  .op = OP_FMUL},{.str = "*.",  .op = OP_FMUL},
    {.str = "fdiv",  .op = OP_FDIV},{.str = "/.",  .op = OP_FDIV},
    {.str = "fmod",  .op = OP_FMOD},{.str = "%.",  .op = OP_FMOD},
    {.str = "finc",  .op = OP_FINC},{.str = "++.",  .op = OP_FINC},
    {.str = "fdec",  .op = OP_FDEC},{.str = "--.",  .op = OP_FDEC},
    {.str = "fpow",  .op = OP_FPOW},{.str = "**.", .op = OP_FPOW},
    {.str = "fsqr",  .op = OP_FSQR},
    {.str = "fsqrt", .op = OP_FSQRT},

    {.str = "push", .op = OP_PUSH},
    {.str = "pop",  .op = OP_POP},
    {.str = "drop", .op = OP_POP},
    {.str = "swap", .op = OP_SWAP},{.str = "<>",   .op = OP_SWAP},
    {.str = "dup",  .op = OP_DUP},
    {.str = "over", .op = OP_OVER},
    {.str = "rot",  .op = OP_LROT},{.str = "+rot", .op = OP_LROT},{.str = "<|",   .op = OP_LROT},
    {.str = "-rot", .op = OP_RROT},{.str = "|>",   .op = OP_RROT},

    {.str = "eq",   .op = OP_EQ},{.str = "=",    .op = OP_EQ},
    {.str = "ne",   .op = OP_NE},{.str = "~",    .op = OP_NE},

    {.str = "igt",  .op = OP_IGT},{.str = ">",    .op = OP_IGT},
    {.str = "gt",   .op = OP_IGT},
    {.str = "ilt",  .op = OP_ILT},{.str = "<",    .op = OP_ILT},
    {.str = "lt",   .op = OP_ILT},
    {.str = "ige",  .op = OP_IGE},{.str = ">=",   .op = OP_IGE},
    {.str = "ge",   .op = OP_IGE},
    {.str = "ile",  .op = OP_ILE},{.str = "<=",   .op = OP_ILE},
    {.str = "le",   .op = OP_ILE},

    {.str = "ugt",   .op = OP_UGT},
    {.str = "ult",   .op = OP_ULT},
    {.str = "uge",   .op = OP_UGE},
    {.str = "ule",   .op = OP_ULE},

    {.str = "fgt",   .op = OP_FGT},{.str = ">.",    .op = OP_FGT},
    {.str = "flt",   .op = OP_FLT},{.str = "<.",    .op = OP_FLT},
    {.str = "fge",   .op = OP_FGE},{.str = ">=.",   .op = OP_FGE},
    {.str = "fle",   .op = OP_FLE},{.str = "<=.",   .op = OP_FLE},

    {.str = "and",  .op = OP_AND},{.str = "&",    .op = OP_AND},
    {.str = "or",   .op = OP_OR}, {.str = "|",    .op = OP_OR},
    {.str = "not",  .op = OP_NOT},{.str = "!",    .op = OP_NOT},

    {.str = "top",  .op = OP_TOP}, {.str = ".", .op = OP_TOP},
    {.str = "put",  .op = OP_PUT}, {.str = "@", .op = OP_PUT},
    {.str = "dump", .op = OP_DUMP},
    {.str = "trace",.op = OP_TRACE},
    {.str = "puts", .op = OP_PUTS},
    {.str = NULL,   .op = 0}
};

char* sblo_op_string(sbl_op_n op){
    for(int i = 0; sblo_opstr_table[i].str; i++)
        if(sblo_opstr_table[i].op == op) return sblo_opstr_table[i].str;
    return "unknown";
}

int32_t sblo_isop(const char* op){
    return sblo_isopn(op,strlen(op));
}

int32_t sblo_isopn(const char* op, size_t len){
    for(int i = 0; sblo_opstr_table[i].str; i++)
        if(streqn(op,sblo_opstr_table[i].str,len)) return sblo_opstr_table[i].op;
    return 0;
}
