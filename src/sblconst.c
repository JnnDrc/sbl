#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "sbldef.h"
#include "sblconst.h"

int constabl_init(constabl_t* ct){
    ct->size = 0;
    ct->capacity  = CONST_TABL_INIT_CAP;

    ct->data = calloc(ct->capacity,sizeof(sblval_t));
    if(!ct->data) return -1;

    return 0;
}

int const_find(constabl_t* ct, sblval_t v){
    for(int i = 0; i < ct->size; i++) if(sblval_eq(ct->data[i],v)) return i;
    return CONST_NOT_FOUND;
}

int const_add(constabl_t* ct, sblval_t v){
    if(ct->size >= ct->capacity){
        uint16_t ncap = ct->capacity * 2;
        sblval_t* ndata = calloc(ncap, sizeof(sblval_t));
        if(!ndata) return CONST_NOT_FOUND;
        memcpy(ndata,ct->data,ct->size * sizeof(sblval_t));
        free(ct->data);
        ct->data = ndata;
        ct->capacity = ncap;
    }
    int i = ct->size;
    ct->data[ct->size++] = v;
    return i;
}

int const_find_or_add(constabl_t* ct, sblval_t v){
    int ci = const_find(ct,v);
    if(ci < 0) ci = const_add(ct,v);
    return ci;
}
