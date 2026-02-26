#ifndef _SBLCONST_H
#define _SBLCONST_H

#include <stdint.h>
#include "sblval.h"

#define CONST_TABL_INIT_CAP 128
typedef struct const_table{
    uint16_t    size;
    uint16_t    capacity;
    sblval_t*   data;
}constabl_t;

int constabl_init(constabl_t* ct);

#define CONST_NOT_FOUND -1
int const_find(constabl_t* ct, sblval_t v);
int const_add(constabl_t* ct,  sblval_t v);

int const_find_or_add(constabl_t* ct, sblval_t v);

#endif /* _SBLCOST_H */
