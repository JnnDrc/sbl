#ifndef _SBLVM_H
#define _SBLVM_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "sblconst.h"
#include "sblinst.h"

#define STACK_SIZE 512
#define STACK_TYPE float
typedef STACK_TYPE stype_t ;
typedef struct stack {
    int32_t top;
    stype_t data[STACK_SIZE];
}stack_t;

void stk_push(stack_t *s,float v);
float stk_pop(stack_t *s);
void stk_trace(stack_t* s);

typedef struct sbl_binh{
    char magic[2];  // SB
    uint16_t insts;  //
    uint32_t ctb_off;
}sblbinh_t;

typedef struct sblvm{
    sblbinh_t   bin_info;

    stack_t     stack;
    ilist_t     insts;
    constabl_t  consts;

    size_t      ip;
    bool        halt;
}sblvm_t;

#define SBL_ERR_NOT_SLB 1
int sblvm_load(sblvm_t* vm, FILE* fp);

#endif /* _SBLVM_H */
