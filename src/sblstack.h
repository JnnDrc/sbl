#ifndef _SBLSTACK_H
#define _SBLSTACK_H

#include <stdint.h>

#define STK_SINGLE 1
#define STK_DOUBLE 2

#define STKOBJ STK_SINGLE

#if STKOBJ == STK_DOUBLE
#define STACK_FLOAT double
#define STACK_INT  int64_t
#define STACK_UINT uint64_t
#else
#define STACK_FLOAT float
#define STACK_INT   int32_t
#define STACK_UINT  uint32_t
#endif /* STKOBJ SIZE */

typedef union stkobj{
    STACK_FLOAT as_float;
    STACK_INT   as_int;
    STACK_UINT  as_uint;
}stkobj_t;
#define STKOBJ_SIZE (sizeof(STACK_OBJECT_TYPE))

stkobj_t stkfloat(STACK_FLOAT f);
stkobj_t stkint(STACK_INT i);
stkobj_t stkuint(STACK_UINT u);

#define STACK_INIT_CAP 512
typedef struct stack {
    int32_t  sp;    // points to next free slot
    int32_t  cap;
    stkobj_t* data;
}stack_t;

int stack_init(stack_t* s);

void stk_push(stack_t *s,stkobj_t v);
stkobj_t stk_pop(stack_t *s);
stkobj_t stk_get(stack_t *s, int32_t i);
void stk_trace(stack_t* s);
void stk_preety(stack_t *s);

#endif /* _SBLSTACK_H */
