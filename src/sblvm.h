#ifndef _SBLVM_H
#define _SBLVM_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "sblver.h"
#include "sblconst.h"
#include "sblinst.h"
#include "sblstack.h"
#include "sblcc.h"

typedef struct sbl_binh{
    char        magic[2];  // SB    // 2
    sblver_t    version;            // 2
    uint32_t    insts;              // 4
    uint32_t    ctb_off;            // 4
    uint32_t    stb_off;            // 4
    uint32_t    start;              // 4
}sblbinh_t;                         // 20 bytes

sblbinh_t sbl_make_info(ilist_t *il, constabl_t *ct, lablist_t *ll,statbuf_t* sb);
int sbl_load_info(sblbinh_t* bi, FILE* fp);

typedef struct sblvm{
    sblbinh_t   bin_info;

    ilist_t     insts;
    constabl_t  consts;
    statbuf_t   statmem;

    stack_t     data;
    stack_t     call;

    size_t      ip;
    bool        halt;
}sblvm_t;

#define SBL_ERR_NOT_SLB 1
int sblvm_load(sblvm_t* vm, FILE* fp);

#define EXEC_ERR_UNKNOWN_OPC 1
int sblvm_exec(sblvm_t* vm);

typedef void (*oprt_f)(sblvm_t*, inst_t);
#define fnop(op) void op(sblvm_t* vm, inst_t i)

#endif /* _SBLVM_H */
