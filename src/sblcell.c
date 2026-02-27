#include <stdio.h>
#include <assert.h>
#include "sblcell.h"

sblcell_t sblfloat(sblfloat_t f){
    return (sblcell_t){.type = CELL_FLOAT,  .as.f = f};
}
sblcell_t sblint(sblint_t i){
    return (sblcell_t){.type = CELL_INT, .as.i = i};
}
sblcell_t sbluint(sbluint_t u){
    return (sblcell_t){.type = CELL_UINT, .as.u = u};
}
sblcell_t sblbool(sblbool_t b){
    return (sblcell_t){.type = CELL_BOOL, .as.u = b};
}
sblcell_t sblptr(sblptr_t p){
    return (sblcell_t){.type = CELL_BOOL, .as.u = p};
}

bool sblcell_eq(sblcell_t a, sblcell_t b){
    if(a.type != b.type) return false;
    switch(a.type){
        case CELL_FLOAT:
            return a.as.f == b.as.f;
        case CELL_INT:
            return a.as.i == b.as.i;
        case CELL_UINT:
        case CELL_BOOL:
        case CELL_PTR:
            return a.as.u == b.as.u;
        default:
            assert(0 && "UNREACHEABLE");
      break;
    }
}
void sblcell_print(sblcell_t c){
    switch(c.type){
        case CELL_INT:
            printf("%zd",c.as.i);
            break;
        case CELL_UINT:
            printf("%zu",c.as.u);
            break;
        case CELL_BOOL:
            printf("%s",c.as.u ? "true" : "false");
            break;
        case CELL_FLOAT:
            printf("%.02f",c.as.f);
            break;
        case CELL_PTR:
            printf("%p",(void*)c.as.u);
            break;
    }
}

bool sblcell_is(sblcell_t c, sblcell_type_n t){
    if (c.type != t) return false;
    return true;
}
