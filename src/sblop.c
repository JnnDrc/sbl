#include "sblop.h"

char* sblo_op_string(sbl_op_n op){
    switch (op) {
        case OP_NONE:   return "none";
        case OP_ADD:    return "add";
        case OP_SUB:    return "sub";
        case OP_MUL:    return "mul";
        case OP_DIV:    return "div";
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
        case OP_ECHO:   return "echo";
        default:        return "unknown";
    }
}
