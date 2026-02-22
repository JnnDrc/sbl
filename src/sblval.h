#ifndef _SBLVAL_H
#define _SBLVAL_H

#include <stdint.h>
#include <stdbool.h>

#define FLOAT32     5
#define FLOAT64     1
#define INT32       2
#define INT64       3

#ifndef SBL_NUMBER
#define SBL_NUMBER FLOAT32
#endif // SBLNUMBER

#if SBL_NUMBER == FLOAT32
#define SBLNUM float
#define strton(str, endptr) strtof(str, endptr)
#elif SBL_NUMBER == FLOAT64
#define SBLNUM double
#elif SBL_NUMBER == INT32
#define strton(str, endptr) strtod(str, endptr)
#define SBLNUM int32_t
#define strton(str, endptr) strtol(str, endptr)
#elif SBL_NUMBER == INT64
#define SBLNUM int64_t
#define strton(str, endptr) strtoll(str, endptr)
#endif /**/

#define SBL_INTEGER int32_t
#define SBL_POINTER void*

typedef SBLNUM      sblnum_t;
typedef SBL_INTEGER sblint_t;
typedef SBL_POINTER sblptr_t;

typedef enum sblval_type{
    SBL_VAL_NUM,
    SBL_VAL_INT,
    SBL_VAL_PTR,
}sblval_type_n;

typedef struct sblval{
    uint8_t type;
    union{
        sblnum_t num;   // number used for calculation (float, may be int for optimizations)
        sblint_t off;   // int32_t (used in the const table for jump, call, etc...)
        sblptr_t ptr;   // uintptr_t
    }as;
}sblval_t;

sblval_t sblnum(sblnum_t n);
sblval_t sblint(sblint_t i);
sblval_t sblptr(sblptr_t p);

bool sblval_eq(sblval_t a, sblval_t b);

#define sblval_is(v,t) (v.type == t)

#endif // _SBLVAL_H
