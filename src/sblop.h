#ifndef _SBLOP_H
#define _SBLOP_H

typedef enum sbl_op{
    OP_NONE = 0,    // 0
    OP_ADD  = 1,    // op
    OP_SUB  = 2,    // op
    OP_MUL  = 3,    // op
    OP_DIV  = 4,    // op
    OP_PUSH = 5,    // op k
    OP_POP  = 6,    // op
    OP_SWAP = 7,    // op
    OP_DUP  = 8,    // op
    OP_OVER = 9,    // op

    OP_JUMP = 15,   // op k
    OP_HOP  = 16,   // op
    OP_GT   = 20,
    OP_LT   = 21,
    OP_GE   = 22,
    OP_LE   = 23,
    OP_EQ   = 24,
    OP_NEQ  = 25,
    // OP_POW,
    // OP_SQR,
    // OP_SQRT,
    OP_ECHO     // op k
}sbl_op_n;

#endif /* _SBLOP_H */
