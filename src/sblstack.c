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

int stack_init(stack_t* s){
    s->top = -1;
    s->cap = STACK_INIT_CAP;

    s->data = calloc(s->cap,sizeof(stype_t));
    if(!s->data) return -1;

    return 0;
}

void stk_push(stack_t *s,float v){
    if(s->top + 1 >= s->cap){
        uint32_t ncap = s->cap * 2;
        stype_t* ndata = calloc(ncap, sizeof(stype_t));
        if(!ndata) return;
        memcpy(ndata,s->data,s->top);
        free(s->data);
        s->data = ndata;
        s->cap = ncap;
    }
    s->data[++s->top] = v;
}

float stk_pop(stack_t *s){
    return s->data[s->top--];
}

void stk_trace(stack_t* s){
    printf("[> ");
    for(int i = 0; i <= s->top; i++){
        printf("%.02f ",s->data[i]);
    }
    printf("]\n");
}

void stk_preety(stack_t *s){
    float max = 0;
    int width = 0;
    for(int32_t i = s->top; i >= 0; i--) max = max(abs(s->data[i]),max);
    width = (int)roundf(1+logf(max)/logf(10.0f));
    for(int i = 0; i < width + 3 + 2 + 2; i++) putchar('-');
    putchar('\n');
    for(int32_t i = s->top; i >= 0; i--) printf("| %*.02f |\n",width+3,s->data[i]);
    for(int i = 0; i < width + 3 + 2 + 2; i++) putchar('-');
    putchar('\n');
}
