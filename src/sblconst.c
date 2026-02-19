#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "sblconst.h"

int constabl_init(constabl_t* ct){
    ct->size = 0;
    ct->capacity  = CONST_TABL_INIT_CAP;

    ct->data = calloc(ct->capacity,sizeof(float));
    if(!ct->data) return -1;

    return 0;
}

int const_find(constabl_t* ct, float f){
    for(int i = 0; i < ct->size; i++) if (ct->data[i] == f) return i;
    return CONST_NOT_FOUND;
}

int const_add(constabl_t* ct, float f){
    if(ct->size >= ct->capacity){
        uint16_t ncap = ct->capacity * 2;
        float* ndata = calloc(ncap, sizeof(float));
        if(!ndata) return CONST_NOT_FOUND;
        memcpy(ndata,ct->data,ct->size);
        free(ct->data);
        ct->data = ndata;
        ct->capacity = ncap;
    }
    int i = ct->size;
    ct->data[ct->size++] = f;
    return i;
}
