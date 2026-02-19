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
