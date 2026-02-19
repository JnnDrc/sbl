#ifndef _SBLVM_H
#define _SBLVM_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "sblconst.h"
#include "sblinst.h"
#include "sblstack.h"

typedef struct sbl_binh{
    char        magic[2];  // SB
    uint16_t    insts;
    uint32_t    ctb_off;
    uint32_t    start;
}sblbinh_t;

typedef struct sblvm{
    sblbinh_t   bin_info;

    stack_t     data;
    stack_t     ret;
    ilist_t     insts;
    constabl_t  consts;

    size_t      ip;
    bool        halt;
}sblvm_t;

#define SBL_ERR_NOT_SLB 1
int sblvm_load(sblvm_t* vm, FILE* fp);

#define EXEC_ERR_UNKNOWN_OPC 1
int sblvm_exec(sblvm_t* vm);

#endif /* _SBLVM_H */
