#include <stdlib.h>
#include <string.h>

#include "sblinst.h"

int ilist_init(ilist_t* il){
    il->size = 0;
    il->capacity = ILIST_INIT_CAP;

    il->data = calloc(il->capacity,sizeof(inst_t));
    if(!il->data) return -1;

    return 0;
}

int ilist_add(ilist_t* il, inst_t i){
    if (il->size >= il->capacity){
        uint32_t ncap = il->capacity * 2;
        inst_t* ndata = calloc(ncap, sizeof(inst_t));
        if(!ndata) return -1;
        memcpy(ndata,il->data,il->size);
        free(il->data);
        il->data = ndata;
        il->capacity = ncap;
    }
    il->data[il->size++] = i;
    return 0;
}


iobj_t iobj_op(uint16_t op){
    return (iobj_t){.kind = INST_OP, .op = op, .arg.k = 0};
}
iobj_t iobj_opk(uint16_t op, uint32_t k){
    return (iobj_t){.kind = INST_OPK, .op = op, .arg.k = k};
}
iobj_t iobj_opab(uint16_t op, uint16_t a, uint16_t b){
    return (iobj_t){.kind = INST_OPAB, .op = op, .arg.ab.a = a, .arg.ab.b = b};
}

inst_t iobj_make(iobj_t io){
    switch(io.kind){
        case INST_OP:   return MAKE_OP(io.op);
        case INST_OPK:  return MAKE_OPK(io.op,io.arg.k);
        case INST_OPAB: return MAKE_OPAB(io.op, io.arg.ab.a,io.arg.ab.b);
        default:        return 0;
    }
}

int iolist_init(iolist_t* iol){
    iol->size = 0;
    iol->capacity = IOLIST_INIT_CAP;

    iol->data = calloc(iol->capacity,sizeof(iobj_t));
    if(!iol->data) return -1;

    return 0;
}
int iolist_add(iolist_t* iol, iobj_t io){
    if (iol->size >= iol->capacity){
        uint32_t ncap = iol->capacity * 2;
        iobj_t* ndata = calloc(ncap, sizeof(iobj_t));
        if(!ndata) return -1;
        memcpy(ndata,iol->data,iol->size);
        free(iol->data);
        iol->data = ndata;
        iol->capacity = ncap;
    }
    iol->data[iol->size++] = io;
    return 0;
}

ilist_t iolist_make(iolist_t iol){
    ilist_t il;
    ilist_init(&il);
    for(uint32_t i = 0; i < iol.size; i++) ilist_add(&il,iobj_make(iol.data[i]));
    return il;
}
