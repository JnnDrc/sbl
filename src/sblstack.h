#ifndef _SBLSTACK_H
#define _SBLSTACK_H

#include <stdint.h>
#include "sblcell.h"

#define STACK_INIT_CAP 512
typedef struct stack {
    int32_t   sp;    // points to next free slot
    int32_t   cap;
    sblcell_t* data;
}stack_t;

int stack_init(stack_t* s);

void stk_push(stack_t *s,sblcell_t v);
sblcell_t stk_pop(stack_t *s);
sblcell_t stk_get(stack_t *s, int32_t i);
void stk_trace(stack_t* s);
void stk_preety(stack_t *s);

#endif /* _SBLSTACK_H */
