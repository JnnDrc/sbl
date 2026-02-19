#ifndef _SBLSTACK_H
#define _SBLSTACK_H

#include <stdint.h>

#define STACK_INIT_CAP 512
#define STACK_TYPE float
typedef STACK_TYPE stype_t;

typedef struct stack {
    int32_t  top;
    int32_t  cap;
    stype_t* data;
}stack_t;

int stack_init(stack_t* s);

void stk_push(stack_t *s,float v);
float stk_pop(stack_t *s);
void stk_trace(stack_t* s);
void stk_preety(stack_t *s);

#endif /* _SBLSTACK_H */
