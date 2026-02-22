#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "sblstack.h"
#include "sblstack.h"

int stack_init(stack_t* s){
    s->sp = 0;
    s->cap = STACK_INIT_CAP;

    s->data = calloc(s->cap,sizeof(sblval_t));
    if(!s->data) return -1;

    return 0;
}

void stk_push(stack_t *s,sblval_t v){
    if(s->sp  >= s->cap){
        uint32_t ncap = s->cap * 2;
        sblval_t* ndata = calloc(ncap, sizeof(sblval_t));
        if(!ndata) return;
        memcpy(ndata,s->data,s->sp);
        free(s->data);
        s->data = ndata;
        s->cap = ncap;
    }
    s->data[s->sp++] = v;
}

sblval_t stk_pop(stack_t *s){
    return s->data[--s->sp];
}

sblval_t stk_get(stack_t *s, int32_t i){
    return s->data[s->sp + i];
}

void stk_trace(stack_t* s){
    printf("[> ");
    for(int i = 0; i <= s->sp; i++){
        if (i == s->sp) printf("* ");
        else{
            switch(s->data[i].type){
                case SBL_VAL_NUM:
                    printf("%.02f ",s->data[i].as.num);
                    break;
                case SBL_VAL_INT:
                    printf("%.02f ",s->data[i].as.num);
                    break;
                case SBL_VAL_PTR:
                    printf("%.02f ",s->data[i].as.num);
                    break;
                default:
                    fprintf(stderr,"ERROR: Unknown value on stack");
                    break;
            }
        }
    }
    printf("]\n");
}
