#ifndef _SBLOP_H
#define _SBLOP_H

#include <stdint.h>

// type notation
// x/y/z/w -> any
// i -> int
// u -> uint
// z -> integer (int/uint)
// b -> bool
// f -> float
// p -> pointer
// o -> offset (uint/pointer) (local address/ heap address)
typedef enum sbl_op{
    // General -----------------------------------------------------------------
    OP_NONE = 0,    // no op, stop execution (EOF)
    OP_NOP  = 1,    // no op, continue
    OP_HALT = 2,    // stop execution

    OP_JUMP  = 3,   // op k   --    (ip += k)
    OP_HOP   = 4,   // op   b --    (b ? ip+2 : ip+1)
                    //
    OP_CALL  = 5,   // op k    -- ip (call stack)
    OP_RET   = 6,   // op   ip -- (call stack)
    // Arithmetic --------------------------------------------------------------
    // -- Int
    OP_IADD   = 10,   // op i i -- i
    OP_ISUB   = 11,   // op i i -- i
    OP_IMUL   = 12,   // op i i -- i
    OP_IDIV   = 13,   // op i i -- i
    OP_IMOD   = 14,   // op i i -- i
    OP_IINC   = 15,   // op i -- i
    OP_IDEC   = 16,   // op i -- i
    OP_IPOW   = 17,   // op  [a b] -> [c]
    OP_ISQR   = 18,   // op  [a] -> [a]
    // -- Uint
    OP_UADD   = 20,   // op u u -- u
    OP_USUB   = 21,   // op u u -- u
    OP_UMUL   = 22,   // op u u -- u
    OP_UDIV   = 23,   // op u u -- u
    OP_UMOD   = 24,   // op u u -- u
    OP_UINC   = 25,   // op u -- u
    OP_UDEC   = 26,   // op u -- u
    OP_UPOW   = 27,   // op u u -- u
    OP_USQR   = 28,   // op u -- u
    // -- Float
    OP_FADD   = 30,   // op f f -- f
    OP_FSUB   = 31,   // op f f -- f
    OP_FMUL   = 32,   // op f f -- f
    OP_FDIV   = 33,   // op f f -- f
    OP_FMOD   = 34,   // op f f -- f
    OP_FINC   = 35,   // op f -- f
    OP_FDEC   = 36,   // op f -- f
    OP_FPOW   = 37,   // op f f -- f
    OP_FSQR   = 38,   // op f -- f
    OP_FSQRT  = 39,   // op f -- f
    // Stack operations --------------------------------------------------------
    OP_PUSH  = 40,  // op k   -- x
    OP_POP   = 41,  // op   x -- 
    OP_SWAP  = 42,  // op   x y -- y x
    OP_DUP   = 43,  // op   x -- x x
    OP_OVER  = 44,  // op   x y -- x y x
    OP_LROT  = 45,  // op   x y z -- y z x
    OP_RROT  = 46,  // op   x y z -- z x y

    // Logic -------------------------------------------------------------------
    OP_EQ    = 50,  // op   x y -- b
    OP_NE    = 51,  // op   x y -- b
    // -- Int
    OP_IGT   = 52,  // op   i i -- b
    OP_ILT   = 53,  // op   i i -- b
    OP_IGE   = 54,  // op   i i -- b
    OP_ILE   = 55,  // op   i i -- b
    // -- Uint
    OP_UGT   = 56,  // op   u u -- b
    OP_ULT   = 57,  // op   u u -- b
    OP_UGE   = 58,  // op   u u -- b
    OP_ULE   = 59,  // op   u u -- b
    // -- Float
    OP_FGT   = 60,  // op   f f -- b
    OP_FLT   = 61,  // op   f f -- b
    OP_FGE   = 62,  // op   f f -- b
    OP_FLE   = 63,  // op   f f -- b

    OP_AND   = 65,  // op   b b -- b
    OP_OR    = 66,  // op   b b -- b
    OP_NOT   = 67,  // op   b -- b

    // IO ----------------------------------------------------------------------
    OP_TOP   = 80,  // op x -- x (prints value)
    OP_PUT   = 81,  // op z -- z (prints char of value a)
    OP_DUMP  = 82,  // op x --   (prints value of a, drops it)
    OP_TRACE = 83,  // op . -- . (debug print entire stack)
    OP_PUTS  = 84,  // op o --   (print null terminated string)
}sbl_op_n;

struct sblo_str_op{
    char*   str;
    sbl_op_n op;
};

char* sblo_op_string(sbl_op_n op);
int32_t sblo_isop(const char* op);
int32_t sblo_isopn(const char* op, size_t len);

#endif /* _SBLOP_H */
