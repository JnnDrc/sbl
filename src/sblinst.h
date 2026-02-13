#ifndef _SBLINST_H
#define _SBLINST_H

#include <stdint.h>

// instruction: uint32_t
//           10       22
// OpK     | op |     CI    |
// OpAB    | op |  A  |  B  |

#define INSTRUCTION uint32_t
#define INST_SIZE   (sizeof(INSTRUCTION))
typedef INSTRUCTION inst_t;

#define INST_BITS (INST_SIZE*8)

#define OP_BITS 10
#define K_BITS  (INST_BITS - OP_BITS)
#define AB_BITS ((INST_BITS - OP_BITS) << (uint8_t)1)

#define BM(n,b)   (~(1UL) >> (b - n))

#define BM22    0x3FFFFF
#define BM11    0x7FF

#define BMK   BM(K_BITS,INST_BITS)
#define BMAB  BM(AB_BITS,INST_BITS)

#define MAKE_OP(op)         ((op) << K_BITS)
#define MAKE_OPK(op,k)      (MAKE_OP(op) | ((k) & BMK))
#define MAKE_OPAB(op,a,b)   (MAKE_OP(op) | (((a) & BMAB) << AB_BITS) | ((b) << AB_BITS))

#define DEC_OP(i) ((inst_t)(i) >> K_BITS)
#define DEC_K(i)  ((inst_t)(i) & BMK)
#define DEC_A(i)  (((inst_t)(i) >> AB_BITS) & BMAB)
#define DEC_B(i)  ((inst_t)(i) & BMAB)

// ----------------------------

#define MAX_INSTRUCTIONS UINT16_MAX
typedef struct instruction_list{
    uint16_t size;
    inst_t data[MAX_INSTRUCTIONS];
}ilist_t;

int ilist_add(ilist_t* il, inst_t i);

#endif /* SLBINST_H */
