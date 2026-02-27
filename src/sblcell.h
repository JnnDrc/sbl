#ifndef _SBLCELL_H
#define _SBLCELL_H

#include <stdint.h>
#include <stdbool.h>

#if INTPTR_MAX == INT64_MAX
#define SBL_FLOAT double
#define SBL_UINT  uint64_t
#define SBL_INT   int64_t
#define strtoi(str,endptr, radix) strtoll(str,endptr, radix)
#define strtou(str,endptr, radix) strtoull(str,endptr, radix)
#define strtofp(str,endptr)       strtod(str,endptr)
#else
#define SBL_FLOAT float
#define SBL_UINT  uint32_t
#define SBL_INT   int32_t
#define strtoi(str,endptr, radix) strtol(str,endptr, radix)
#define strtou(str,endptr, radix) strtoul(str,endptr, radix)
#define strtofp(str,endptr)       strtof(str,endptr)
#endif

typedef SBL_FLOAT sblfloat_t;
typedef SBL_UINT  sbluint_t;
typedef SBL_INT   sblint_t;
typedef SBL_UINT  sblbool_t;
typedef SBL_UINT  sblptr_t;

typedef enum sblcell_type{
    CELL_INT,
    CELL_UINT,
    CELL_BOOL,
    CELL_FLOAT,
    CELL_PTR,
}sblcell_type_n;

typedef struct sblcell{
    sblcell_type_n type;
    union{
        sblfloat_t  f;
        sbluint_t   u;
        sblint_t    i;
    }as;
}sblcell_t;

sblcell_t sblfloat(sblfloat_t f);
sblcell_t sblint(sblint_t i);
sblcell_t sbluint(sbluint_t u);
sblcell_t sblbool(sblbool_t b);
sblcell_t sblptr(sblptr_t p);

bool sblcell_eq(sblcell_t a, sblcell_t b);
void sblcell_print(sblcell_t c);

bool sblcell_is(sblcell_t c, sblcell_type_n t);


#endif /* _SBLCELL_H */
