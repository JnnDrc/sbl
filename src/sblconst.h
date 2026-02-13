#ifndef _SBLCONST_H
#define _SBLCONST_H

#include <stdint.h>

#define CONST_TABLE_SIZE 1024
typedef struct const_table{
    uint16_t   size;
    float      data[1024];
}constabl_t;


int const_find(constabl_t ct, float f);
int const_add(constabl_t* ct, float f);

#endif /* _SBLCOST_H */
