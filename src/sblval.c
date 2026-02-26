#include <stdbool.h>
#include <stdio.h>

#include "sblval.h"
#include "sbldef.h"

sblval_t sblnum(sblnum_t n){
    return (sblval_t){.type = SBL_VAL_NUM, .as.num = n};
}
sblval_t sblint(sblint_t i){
    return (sblval_t){.type = SBL_VAL_INT, .as.off = i};
}
sblval_t sblptr(sblptr_t p){
    return (sblval_t){.type = SBL_VAL_PTR, .as.ptr = p};
}

bool sblval_eq(sblval_t a, sblval_t b){
    if (a.type != b.type) return false;
    if (!memeq(&a.as,&b.as,sizeof(a.as))) return false;
    return true;
}

void sblval_print(sblval_t v){
    switch(v.type){
        case SBL_VAL_NUM:
            printf("%.02f",v.as.num);
            break;
        case SBL_VAL_INT:
              printf("%d",v.as.off);
              break;
        case SBL_VAL_PTR:
              printf("%p",v.as.ptr);
              break;
    }
}
