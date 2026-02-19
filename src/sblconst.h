#ifndef _SBLCONST_H
#define _SBLCONST_H

#include <stdint.h>

#include "sblstack.h"

#define CONST_TABL_INIT_CAP 128
typedef struct const_table{
    uint16_t size;
    uint16_t capacity;
    stkobj_t*   data;
}constabl_t;

int constabl_init(constabl_t* ct);

#define CONST_NOT_FOUND -1
int const_find(constabl_t* ct, stkobj_t f);
int const_add(constabl_t* ct,  stkobj_t f);

#endif /* _SBLCOST_H */
