#ifndef _SBLINST_H
#define _SBLINST_H

#include <stdint.h>

/*
 * inst_t: machine byte packade instruction
 * iobj_t: virtual representation used for compilation
 * */


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
#define MAKE_OPAB(op,a,b)   (MAKE_OP(op) | (((a) & BM11) << AB_BITS) | ((b && BM11)))

#define DEC_OP(i) ((inst_t)(i) >> K_BITS)
#define DEC_K(i)  ((inst_t)(i) & BM22)
#define DEC_A(i)  (((inst_t)(i) >> AB_BITS) & BM11)
#define DEC_B(i)  ((inst_t)(i) & BM11)

// -------------------------------------

#define ILIST_INIT_CAP 256
typedef struct instruction_list{
    uint32_t size;
    uint32_t capacity;
    inst_t*  data;
}ilist_t;

int ilist_init(ilist_t* il);
int ilist_add(ilist_t* il, inst_t i);

// -------------------------------------

typedef enum inst_kind{
    INST_OP,
    INST_OPK,
    INST_OPAB,  // not used, reserved if some day needed
}ikind_t;

typedef struct instruction_object{
    ikind_t kind;
    uint16_t op;
    union{
        uint32_t k;
        struct{
          uint16_t a;
          uint16_t b;
        }ab;
    }arg;
}iobj_t;

iobj_t iobj_op(uint16_t op);
iobj_t iobj_opk(uint16_t op, uint32_t k);
iobj_t iobj_opab(uint16_t op, uint16_t a, uint16_t b);
inst_t iobj_make(iobj_t io);

#define IOLIST_INIT_CAP 256
typedef struct iobj_list{
    uint32_t size;
    uint32_t capacity;
    iobj_t* data;
}iolist_t;

int iolist_init(iolist_t* iol);
int iolist_add(iolist_t* iol, iobj_t io);

ilist_t iolist_make(iolist_t iol);

#endif /* SLBINST_H */
