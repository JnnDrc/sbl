#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "sblstack.h"

#define abs(x)   ((x) < 0 ? (-x) : (x))
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#define sign(x)  ((x) == 0 ? 0 : (x) < 0 ? -1 : 1)

stkobj_t stkfloat(STACK_FLOAT f){
    return (stkobj_t){.as_float = f};
}
stkobj_t stkint(STACK_INT i){
    return (stkobj_t){.as_int = i};
}
stkobj_t stkuint(STACK_UINT u){
    return (stkobj_t){.as_uint = u};
}

int stack_init(stack_t* s){
    s->sp = 0;
    s->cap = STACK_INIT_CAP;

    s->data = calloc(s->cap,sizeof(stkobj_t));
    if(!s->data) return -1;

    return 0;
}

void stk_push(stack_t *s,stkobj_t v){
    if(s->sp  >= s->cap){
        uint32_t ncap = s->cap * 2;
        stkobj_t* ndata = calloc(ncap, sizeof(stkobj_t));
        if(!ndata) return;
        memcpy(ndata,s->data,s->sp);
        free(s->data);
        s->data = ndata;
        s->cap = ncap;
    }
    s->data[s->sp++] = v;
}

stkobj_t stk_pop(stack_t *s){
    return s->data[--s->sp];
}

stkobj_t stk_get(stack_t *s, int32_t i){
    return s->data[s->sp + i];
}

void stk_trace(stack_t* s){
    printf("[> ");
    for(int i = 0; i < s->sp; i++){
        printf("%.02f ",s->data[i].as_float);
    }
    printf("]\n");
}

void stk_preety(stack_t *s){
    float max = 0;
    int width = 0;
    for(int32_t i = s->sp - 1; i >= 0; i--) max = max(abs(s->data[i].as_float),max);
    width = (int)roundf(1+logf(max)/logf(10.0f));
    for(int i = 0; i < width + 3 + 2 + 2; i++) putchar('-');
    putchar('\n');
    for(int32_t i = s->sp - 1; i >= 0; i--) printf("| %*.02f |\n",width+3,s->data[i].as_float);
    for(int i = 0; i < width + 3 + 2 + 2; i++) putchar('-');
    putchar('\n');
}
