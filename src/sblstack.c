#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "sblstack.h"
#include "sblstack.h"

int stack_init(stack_t* s){
    s->sp = 0;
    s->cap = STACK_INIT_CAP;

    s->data = calloc(s->cap,sizeof(sblcell_t));
    if(!s->data) return -1;

    return 0;
}

void stk_push(stack_t *s,sblcell_t v){
    if(s->sp  >= s->cap){
        uint32_t ncap = s->cap * 2;
        sblcell_t* ndata = calloc(ncap, sizeof(sblcell_t));
        if(!ndata) return;
        memcpy(ndata,s->data,s->sp);
        free(s->data);
        s->data = ndata;
        s->cap = ncap;
    }
    s->data[s->sp++] = v;
}

sblcell_t stk_pop(stack_t *s){
    return s->data[--s->sp];
}

sblcell_t stk_get(stack_t *s, int32_t i){
    return s->data[s->sp + i];
}

void stk_trace(stack_t* s){
    printf("[> ");
    for(int i = 0; i <= s->sp; i++){
        if (i == s->sp) printf("* ");
        else{
            sblcell_print(s->data[i]);
            putchar(' ');
        }
    }
    printf("]\n");
}
