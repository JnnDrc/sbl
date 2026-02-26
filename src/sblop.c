#include "sblop.h"
#include "sbldef.h"

static struct sblo_str_op sblo_opstr_table[] = {
    {.str = "none", .op = OP_NONE},
    {.str = "add",  .op = OP_ADD},
    {.str = "+",    .op = OP_ADD},
    {.str = "sub",  .op = OP_SUB},
    {.str = "-",    .op = OP_SUB},
    {.str = "mul",  .op = OP_MUL},
    {.str = "*",    .op = OP_MUL},
    {.str = "div",  .op = OP_DIV},
    {.str = "/",    .op = OP_DIV},
    {.str = "mod",  .op = OP_MOD},
    {.str = "%",    .op = OP_MOD},
    {.str = "inc",  .op = OP_INC},
    {.str = "++",   .op = OP_INC},
    {.str = "dec",  .op = OP_DEC},
    {.str = "--",   .op = OP_DEC},
    {.str = "sum",  .op = OP_SUM},
    {.str = "prod", .op = OP_PROD},
    {.str = "push", .op = OP_PUSH},
    {.str = "pop",  .op = OP_POP},
    {.str = "drop", .op = OP_POP},
    {.str = "swap", .op = OP_SWAP},
    {.str = "<>",   .op = OP_SWAP},
    {.str = "dup",  .op = OP_DUP},
    {.str = "over", .op = OP_OVER},
    {.str = "rot",  .op = OP_LROT},
    {.str = "+rot", .op = OP_LROT},
    {.str = "<|",   .op = OP_LROT},
    {.str = "-rot", .op = OP_RROT},
    {.str = "|>",   .op = OP_RROT},
    {.str = "jump", .op = OP_JUMP},
    {.str = "jmp",  .op = OP_JUMP},
    {.str = "hop",  .op = OP_HOP},
    {.str = "gt",   .op = OP_GT},
    {.str = ">",    .op = OP_GT},
    {.str = "lt",   .op = OP_LT},
    {.str = "<",    .op = OP_LT},
    {.str = "ge",   .op = OP_GE},
    {.str = ">=",   .op = OP_GE},
    {.str = "le",   .op = OP_LE},
    {.str = "<=",   .op = OP_LE},
    {.str = "eq",   .op = OP_EQ},
    {.str = "=",    .op = OP_EQ},
    {.str = "ne",   .op = OP_NE},
    {.str = "~",    .op = OP_NE},
    {.str = "pow",  .op = OP_POW},
    {.str = "**",   .op = OP_POW},
    {.str = "sqr",  .op = OP_SQR},
    {.str = "sqrt", .op = OP_SQRT},
    {.str = "call", .op = OP_CALL},
    {.str = "ret",  .op = OP_RET},
    {.str = ";",    .op = OP_RET},
    {.str = "top",  .op = OP_TOP},
    {.str = ".",    .op = OP_TOP},
    {.str = "put",  .op = OP_PUT},
    {.str = "@",    .op = OP_PUT},
    {.str = "dump", .op = OP_DUMP},
    {.str = "trace",.op = OP_TRACE},
    {.str = NULL,   .op = 0}
};
char* sblo_op_string(sbl_op_n op){
    switch (op) {
        case OP_NONE:   return "none";
        case OP_ADD:    return "add";
        case OP_SUB:    return "sub";
        case OP_MUL:    return "mul";
        case OP_DIV:    return "div";
        case OP_MOD:    return "mod";
        case OP_INC:    return "inc";
        case OP_DEC:    return "dec";
        case OP_SUM:    return "sum";
        case OP_PROD:   return "prod";
        case OP_PUSH:   return "push";
        case OP_POP:    return "pop";
        case OP_SWAP:   return "swap";
        case OP_DUP:    return "dup";
        case OP_OVER:   return "over";
        case OP_LROT:   return "lrot";
        case OP_RROT:   return "rrot";
        case OP_JUMP:   return "jump";
        case OP_HOP:    return "hop";
        case OP_GT:     return "gt";
        case OP_LT:     return "lt";
        case OP_GE:     return "ge";
        case OP_LE:     return "le";
        case OP_EQ:     return "eq";
        case OP_NE:     return "ne";
        case OP_POW:    return "pow";
        case OP_SQR:    return "sqr";
        case OP_SQRT:   return "sqrt";
        case OP_CALL:   return "call";
        case OP_RET:    return "ret";
        case OP_TOP:    return "top";
        case OP_PUT:    return "put";
        case OP_DUMP:   return "dump";
        case OP_TRACE:  return "trace";
        default:        return "unknown";
    }
}

int32_t sblo_isop(const char* op){
    return sblo_isopn(op,strlen(op));
}

int32_t sblo_isopn(const char* op, size_t len){
    for(int i = 0; sblo_opstr_table[i].str; i++)
        if(streqn(op,sblo_opstr_table[i].str,len))
            return sblo_opstr_table[i].op;
    return 0;
}
