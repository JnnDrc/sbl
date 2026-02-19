#include "sblop.h"

char* sblo_op_string(sbl_op_n op){
    switch (op) {
        case OP_NONE:   return "none";
        case OP_ADD:    return "add";
        case OP_SUB:    return "sub";
        case OP_MUL:    return "mul";
        case OP_DIV:    return "div";
        case OP_MOD:    return "mod";
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
        case OP_NEQ:    return "neq";
        case OP_POW:    return "pow";
        case OP_SQR:    return "sqr";
        case OP_SQRT:   return "sqrt";
        case OP_CALL:   return "call";
        case OP_RET:    return "ret";
        case OP_ECHO:   return "echo";
        case OP_PRINT:  return "print";
        default:        return "unknown";
    }
}
