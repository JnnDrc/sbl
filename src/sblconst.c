#include "sblconst.h"

int const_find(constabl_t ct, float f){
    for(int i = 0; i < ct.size; i++) if (ct.data[i] == f) return i;
    return -1;
}

int const_add(constabl_t* ct, float f){
    if(ct->size >= CONST_TABLE_SIZE) return -1;
    int i = ct->size;
    ct->data[ct->size++] = f;
    return i;
}
