#ifndef _SBLOP_H
#define _SBLOP_H

#include <stdint.h>

typedef enum sbl_op{
    OP_NONE = 0,    // 0

    OP_ADD   = 1,   // op [a b] -> [c]
    OP_SUB   = 2,   // op [a b] -> [c]
    OP_MUL   = 3,   // op [a b] -> [c]
    OP_DIV   = 4,   // op [a b] -> [c]
    OP_MOD   = 5,   // op [a b] -> [c]
    OP_INC   = 6,   // op [a] -> [b]
    OP_DEC   = 7,   // op [a] -> [b]
    OP_SUM   = 8,   // op [... a b c n] -> [s]
    OP_PROD  = 9,   // op [... a b c n] -> [p]

    OP_PUSH  = 10,  // op k [] -> [a]
    OP_POP   = 11,  // op   [a] -> []
    OP_SWAP  = 12,  // op   [a b] -> [b a]
    OP_DUP   = 13,  // op   [a] -> [a a]
    OP_OVER  = 14,  // op   [a b] -> [a b a]
    OP_LROT  = 15,  // op   [a b c] -> [b c a]
    OP_RROT  = 16,  // op   [a b c] -> [c a b]

    OP_JUMP  = 20,  // op k [] -> [] (ip = ip - #k)
    OP_HOP   = 21,  // op   [c] -> [] (ip = c ? ip+2 : ip+1)

    OP_GT    = 25,  // op   [a b] -> [c]
    OP_LT    = 26,  // op   [a b] -> [c]
    OP_GE    = 27,  // op   [a b] -> [c]
    OP_LE    = 28,  // op   [a b] -> [c]
    OP_EQ    = 29,  // op   [a b] -> [c]
    OP_NE    = 30,  // op   [a b] -> [c]

    OP_POW   = 35,   // op  [a b] -> [c]
    OP_SQR   = 36,   // op  [a] -> [a]
    OP_SQRT  = 37,   // op  [a] -> [a]

    OP_CALL  = 40,   // op k c[] -> c[ip]
    OP_RET   = 41,   // op   c[ip] -> [ip]

    OP_ECHO  = 80,  // op k [] -> [] (print #k elements)
    OP_TOP   = 81,  // op
}sbl_op_n;

char* sblo_op_string(sbl_op_n op);
int32_t sblo_isop(const char* op);
int32_t sblo_isopn(const char* op, size_t len);

#endif /* _SBLOP_H */
