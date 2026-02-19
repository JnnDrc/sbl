#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "sbldef.h"
#include "sblconst.h"
#include "sblstack.h"

int constabl_init(constabl_t* ct){
    ct->size = 0;
    ct->capacity  = CONST_TABL_INIT_CAP;

    ct->data = calloc(ct->capacity,sizeof(stkobj_t));
    if(!ct->data) return -1;

    return 0;
}

int const_find(constabl_t* ct, stkobj_t f){
    for(int i = 0; i < ct->size; i++) if (memeq(&ct->data[i],&f,sizeof(stkobj_t))) return i;
    return CONST_NOT_FOUND;
}

int const_add(constabl_t* ct, stkobj_t f){
    if(ct->size >= ct->capacity){
        uint16_t ncap = ct->capacity * 2;
        stkobj_t* ndata = calloc(ncap, sizeof(stkobj_t));
        if(!ndata) return CONST_NOT_FOUND;
        memcpy(ndata,ct->data,ct->size * sizeof(stkobj_t));
        free(ct->data);
        ct->data = ndata;
        ct->capacity = ncap;
    }
    int i = ct->size;
    ct->data[ct->size++] = f;
    return i;
}
