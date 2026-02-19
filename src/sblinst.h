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
#define AB_BITS ((INST_BITS - OP_BITS)/2)

#define BM(n,b)   (~(1UL) >> (b - n))

#define BM22    0x3FFFFF
#define BM11    0x7FF

#define BMK     BM(K_BITS,INST_BITS)
#define BMAB    BM(AB_BITS,INST_BITS)

#define MAKE_OP(op)         ((op) << K_BITS)
#define MAKE_OPK(op,k)      (MAKE_OP(op) | ((k) & BM22))
#define MAKE_OPAB(op,a,b)   (MAKE_OP(op) | (((a) & BM11) << AB_BITS) | ((b) << AB_BITS))

#define DEC_OP(i) ((inst_t)(i) >> K_BITS)
#define DEC_K(i)  ((inst_t)(i) & BM22)
#define DEC_A(i)  (((inst_t)(i) >> AB_BITS) & BM11)
#define DEC_B(i)  ((inst_t)(i) & BM11)

// ----------------------------

#define ILIST_INIT_CAP 256
typedef struct instruction_list{
    uint32_t size;
    uint32_t capacity;
    inst_t*  data;
}ilist_t;

int ilist_init(ilist_t* il);
int ilist_add(ilist_t* il, inst_t i);

#endif /* SLBINST_H */
