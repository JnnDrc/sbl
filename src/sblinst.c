#include "sblinst.h"

int ilist_add(ilist_t* il, inst_t i){
    if (il->size >= MAX_INSTRUCTIONS) return -1;
    il->data[il->size++] = i;
    return 0;
}
