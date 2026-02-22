#include "sblop.h"

#include "sbldef.h"

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
        default:        return "unknown";
    }
}

int32_t sblo_isop(const char* op){
    if      (streq(op,"none"))  return OP_NONE;
    else if (streq(op,"add"))   return OP_ADD;
    else if (streq(op,"sub"))   return OP_SUB;
    else if (streq(op,"mul"))   return OP_MUL;
    else if (streq(op,"div"))   return OP_DIV;
    else if (streq(op,"mod"))   return OP_MOD;
    else if (streq(op,"inc"))   return OP_INC;
    else if (streq(op,"dec"))   return OP_DEC;
    else if (streq(op,"sum"))   return OP_SUM;
    else if (streq(op,"prod"))  return OP_PROD;
    else if (streq(op,"push"))  return OP_PUSH;
    else if (streq(op,"pop"))   return OP_POP;
    else if (streq(op,"swap"))  return OP_SWAP;
    else if (streq(op,"dup"))   return OP_DUP;
    else if (streq(op,"over"))  return OP_OVER;
    else if (streq(op,"rot"))   return OP_LROT;
    else if (streq(op,"+rot"))  return OP_LROT;
    else if (streq(op,"-rot"))  return OP_RROT;
    else if (streq(op,"jump"))  return OP_JUMP;
    else if (streq(op,"jmp"))   return OP_JUMP;
    else if (streq(op,"hop"))   return OP_HOP;
    else if (streq(op,"gt"))    return OP_GT;
    else if (streq(op,"lt"))    return OP_LT;
    else if (streq(op,"ge"))    return OP_GE;
    else if (streq(op,"le"))    return OP_LE;
    else if (streq(op,"eq"))    return OP_EQ;
    else if (streq(op,"ne"))    return OP_NE;
    else if (streq(op,"pow"))   return OP_POW;
    else if (streq(op,"sqr"))   return OP_SQR;
    else if (streq(op,"sqrt"))  return OP_SQRT;
    else if (streq(op,"call"))  return OP_CALL;
    else if (streq(op,"ret"))   return OP_RET;
    else if (streq(op,"top"))   return OP_TOP;
    else return -1;
}

int32_t sblo_isopn(const char* op, size_t len){
    if      (streqn(op,"none",len))  return OP_NONE;
    else if (streqn(op,"add",len))   return OP_ADD;
    else if (streqn(op,"sub",len))   return OP_SUB;
    else if (streqn(op,"mul",len))   return OP_MUL;
    else if (streqn(op,"div",len))   return OP_DIV;
    else if (streqn(op,"mod",len))   return OP_MOD;
    else if (streqn(op,"inc",len))   return OP_INC;
    else if (streqn(op,"dec",len))   return OP_DEC;
    else if (streqn(op,"sum",len))   return OP_SUM;
    else if (streqn(op,"prod",len))  return OP_PROD;
    else if (streqn(op,"push",len))  return OP_PUSH;
    else if (streqn(op,"pop",len))   return OP_POP;
    else if (streqn(op,"swap",len))  return OP_SWAP;
    else if (streqn(op,"dup",len))   return OP_DUP;
    else if (streqn(op,"over",len))  return OP_OVER;
    else if (streqn(op,"rot",len))   return OP_LROT;
    else if (streqn(op,"+rot",len))  return OP_LROT;
    else if (streqn(op,"-rot",len))  return OP_RROT;
    else if (streqn(op,"jump",len))  return OP_JUMP;
    else if (streqn(op,"jmp",len))   return OP_JUMP;
    else if (streqn(op,"hop",len))   return OP_HOP;
    else if (streqn(op,"gt",len))    return OP_GT;
    else if (streqn(op,"lt",len))    return OP_LT;
    else if (streqn(op,"ge",len))    return OP_GE;
    else if (streqn(op,"le",len))    return OP_LE;
    else if (streqn(op,"eq",len))    return OP_EQ;
    else if (streqn(op,"ne",len))    return OP_NE;
    else if (streqn(op,"pow",len))   return OP_POW;
    else if (streqn(op,"sqr",len))   return OP_SQR;
    else if (streqn(op,"sqrt",len))  return OP_SQRT;
    else if (streqn(op,"call",len))  return OP_CALL;
    else if (streqn(op,"ret",len))   return OP_RET;
    else if (streqn(op,"top",len))   return OP_TOP;
    else return OP_NONE;
}
