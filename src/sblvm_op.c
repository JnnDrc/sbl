#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "sbldef.h"
#include "sblvm.h"

// Nothing and halt

fnop(op_none){
    vm->halt = true;
}
fnop(op_nop){
    return;
}
fnop(op_halt){
    vm->halt = true;
}

// Control flow

fnop(op_jump){
    sblcell_t x = vm->consts.data[cast(uint32_t,DEC_K(i))];
    vm->ip += (int32_t)x.as.i - 1;
}

fnop(op_hop){
    sblcell_t c = stk_pop(&vm->data);
    if(c.as.u) vm->ip++;
}

fnop(op_call){
    sblcell_t off = vm->consts.data[cast(uint32_t,DEC_K(i))];
    stk_push(&vm->call,sbluint(vm->ip));
    vm->ip += (int)off.as.i - 1;
}

fnop(op_ret){
    vm->ip = stk_pop(&vm->call).as.u;
}

// Signed integer arithmetics
fnop(op_iadd){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_INT || b.type != CELL_INT){
     fprintf(stderr,"ERROR: mismatching types on iadd operation");
     exit(1);
    }

    stk_push(&vm->data,sblint(a.as.i + b.as.i));
}

fnop(op_isub){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_INT || b.type != CELL_INT){
     fprintf(stderr,"ERROR: mismatching types on isub operation");
     exit(1);
    }

    stk_push(&vm->data,sblint(a.as.i - b.as.i));
}

fnop(op_imul){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_INT || b.type != CELL_INT){
     fprintf(stderr,"ERROR: mismatching types on imul operation");
     exit(1);
    }

    stk_push(&vm->data,sblint(a.as.i * b.as.i));
}

fnop(op_idiv){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_INT || b.type != CELL_INT){
     fprintf(stderr,"ERROR: mismatching types on idiv operation");
     exit(1);
    }

    stk_push(&vm->data,sblint(a.as.i / b.as.i));
}

fnop(op_imod){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_INT || b.type != CELL_INT){
     fprintf(stderr,"ERROR: mismatching types on imod operation");
     exit(1);
    }

    stk_push(&vm->data,sblint(a.as.i % b.as.i));
}

fnop(op_iinc){
    sblcell_t a = stk_pop(&vm->data);

    if(a.type != CELL_INT){
     fprintf(stderr,"ERROR: mismatching types on iinc operation");
     exit(1);
    }

    stk_push(&vm->data,sblint(a.as.i + 1));
}

fnop(op_idec){
    sblcell_t a = stk_pop(&vm->data);

    if(a.type != CELL_INT){
     fprintf(stderr,"ERROR: mismatching types on idec operation");
     exit(1);
    }

    stk_push(&vm->data,sblint(a.as.i - 1));
}

fnop(op_ipow){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_INT || b.type != CELL_UINT){
     fprintf(stderr,"ERROR: mismatching types on ipow operation");
     exit(1);
    }

    stk_push(&vm->data,sblint(powll(a.as.i,b.as.u)));
}

fnop(op_isqr){
    sblcell_t a = stk_pop(&vm->data);

    if(a.type != CELL_INT){
     fprintf(stderr,"ERROR: mismatching types on isqr operation");
     exit(1);
    }
    stk_push(&vm->data,sblint(a.as.i * a.as.i));
}

// Unsigned integer arithmetics
fnop(op_uadd){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_UINT || b.type != CELL_UINT){
     fprintf(stderr,"ERROR: mismatching types on uadd operation");
     exit(1);
    }

    stk_push(&vm->data,sbluint(a.as.u + b.as.u));
}
fnop(op_usub){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_UINT || b.type != CELL_UINT){
     fprintf(stderr,"ERROR: mismatching types on usub operation");
     exit(1);
    }

    stk_push(&vm->data,sbluint(a.as.u - b.as.u));
}
fnop(op_umul){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_UINT || b.type != CELL_UINT){
     fprintf(stderr,"ERROR: mismatching types on umul operation");
     exit(1);
    }

    stk_push(&vm->data,sbluint(a.as.u * b.as.u));
}
fnop(op_udiv){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_UINT || b.type != CELL_UINT){
     fprintf(stderr,"ERROR: mismatching types on udiv operation");
     exit(1);
    }

    stk_push(&vm->data,sbluint(a.as.u / b.as.u));
}
fnop(op_umod){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_UINT || b.type != CELL_UINT){
     fprintf(stderr,"ERROR: mismatching types on umod operation");
     exit(1);
    }

    stk_push(&vm->data,sbluint(a.as.u % b.as.u));
}
fnop(op_uinc){
    sblcell_t a = stk_pop(&vm->data);

    if(a.type != CELL_UINT){
     fprintf(stderr,"ERROR: mismatching types on uinc operation");
     exit(1);
    }

    stk_push(&vm->data,sbluint(a.as.u + 1));
}
fnop(op_udec){
    sblcell_t a = stk_pop(&vm->data);

    if(a.type != CELL_UINT){
     fprintf(stderr,"ERROR: mismatching types on udec operation");
     exit(1);
    }

    stk_push(&vm->data,sbluint(a.as.u - 1));
}
fnop(op_upow){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_UINT || b.type != CELL_UINT){
     fprintf(stderr,"ERROR: mismatching types on upow operation");
     exit(1);
    }

    stk_push(&vm->data,sbluint(powull(a.as.u,b.as.u)));
}
fnop(op_usqr){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_UINT || b.type != CELL_UINT){
     fprintf(stderr,"ERROR: mismatching types on usqr operation");
     exit(1);
    }

    stk_push(&vm->data,sbluint(a.as.u * b.as.u));
}

// Floating point arithmetics

fnop(op_fadd){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_FLOAT || b.type != CELL_FLOAT){
     fprintf(stderr,"ERROR: mismatching types on fadd operation");
     exit(1);
    }

    stk_push(&vm->data,sblfloat(a.as.f + b.as.f));
}
fnop(op_fsub){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_FLOAT || b.type != CELL_FLOAT){
     fprintf(stderr,"ERROR: mismatching types on fsub operation");
     exit(1);
    }

    stk_push(&vm->data,sblfloat(a.as.f - b.as.f));
}
fnop(op_fmul){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_FLOAT || b.type != CELL_FLOAT){
     fprintf(stderr,"ERROR: mismatching types on fmul operation");
     exit(1);
    }

    stk_push(&vm->data,sblfloat(a.as.f * b.as.f));
}
fnop(op_fdiv){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_FLOAT || b.type != CELL_FLOAT){
     fprintf(stderr,"ERROR: mismatching types on fdiv operation");
     exit(1);
    }

    stk_push(&vm->data,sblfloat(a.as.f / b.as.f));
}
fnop(op_fmod){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_FLOAT || b.type != CELL_FLOAT){
     fprintf(stderr,"ERROR: mismatching types on fmod operation");
     exit(1);
    }

    stk_push(&vm->data,sblfloat(fmod(a.as.f, b.as.f)));
}
fnop(op_finc){
    sblcell_t a = stk_pop(&vm->data);

    if(a.type != CELL_FLOAT){
     fprintf(stderr,"ERROR: mismatching types on finc operation");
     exit(1);
    }

    stk_push(&vm->data,sblfloat(a.as.f + 1.0));
}
fnop(op_fdec){
    sblcell_t a = stk_pop(&vm->data);

    if(a.type != CELL_FLOAT){
     fprintf(stderr,"ERROR: mismatching types on fdec operation");
     exit(1);
    }

    stk_push(&vm->data,sblfloat(a.as.f - 1.0));
}
fnop(op_fpow){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_FLOAT || b.type != CELL_FLOAT){
     fprintf(stderr,"ERROR: mismatching types on fpow operation");
     exit(1);
    }

    stk_push(&vm->data,sblfloat(pow(a.as.f,b.as.f)));
}
fnop(op_fsqr){
    sblcell_t a = stk_pop(&vm->data);

    if(a.type != CELL_FLOAT){
     fprintf(stderr,"ERROR: mismatching types on fsqr operation");
     exit(1);
    }

    stk_push(&vm->data,sblfloat(a.as.f * a.as.f));
}
fnop(op_fsqrt){
    sblcell_t a = stk_pop(&vm->data);

    if(a.type != CELL_FLOAT){
        fprintf(stderr,"ERROR: mismatching types on fsqrt operation");
        exit(1);
    }

    stk_push(&vm->data,sblfloat(sqrt(a.as.f)));
}

// Stack operations

fnop(op_push){
    sblcell_t x = vm->consts.data[cast(uint32_t,DEC_K(i))];
    stk_push(&vm->data,x);
}

fnop(op_pop){
    stk_pop(&vm->data);
}
fnop(op_swap){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);
    stk_push(&vm->data,a);
    stk_push(&vm->data,b);
}
fnop(op_dup){
    sblcell_t x = stk_pop(&vm->data);
    stk_push(&vm->data,x);
    stk_push(&vm->data,x);
}
fnop(op_over){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);
    stk_push(&vm->data,b);
    stk_push(&vm->data,a);
    stk_push(&vm->data,b);
}
fnop(op_lrot){
    sblcell_t tmp = vm->data.data[vm->data.sp - 3];
    vm->data.data[vm->data.sp - 3] = vm->data.data[vm->data.sp - 2];
    vm->data.data[vm->data.sp - 2] = vm->data.data[vm->data.sp - 1];
    vm->data.data[vm->data.sp - 1] = tmp;
}
fnop(op_rrot){
    sblcell_t tmp = vm->data.data[vm->data.sp - 1];
    vm->data.data[vm->data.sp - 1] = vm->data.data[vm->data.sp - 2];
    vm->data.data[vm->data.sp - 2] = vm->data.data[vm->data.sp - 3];
    vm->data.data[vm->data.sp - 3] = tmp;
}

// Comparasions

fnop(op_eq){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);
    stk_push(&vm->data,sblbool(sblcell_eq(a,b)));
}
fnop(op_ne){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);
    stk_push(&vm->data,sblbool(!sblcell_eq(a,b)));
}

fnop(op_igt){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_INT || b.type != CELL_INT){
        fprintf(stderr,"ERROR: mismatching types on ipow operation");
        exit(1);
    }

    stk_push(&vm->data,sblbool(a.as.i > b.as.i));
}
fnop(op_ilt){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_INT || b.type != CELL_INT){
        fprintf(stderr,"ERROR: mismatching types on ipow operation");
        exit(1);
    }

    stk_push(&vm->data,sblbool(a.as.i < b.as.i));
}
fnop(op_ige){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_INT || b.type != CELL_INT){
        fprintf(stderr,"ERROR: mismatching types on ipow operation");
        exit(1);
    }

    stk_push(&vm->data,sblbool(a.as.i >= b.as.i));
}
fnop(op_ile){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_INT || b.type != CELL_INT){
        fprintf(stderr,"ERROR: mismatching types on ipow operation");
        exit(1);
    }

    stk_push(&vm->data,sblbool(a.as.i <= b.as.i));
}

// Logic

fnop(op_and){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_BOOL || b.type != CELL_BOOL){
        fprintf(stderr,"ERROR: mismatching types on and operation");
        exit(1);
    }

    stk_push(&vm->data,sblbool(a.as.i && b.as.i));
}
fnop(op_or){
    sblcell_t a = stk_pop(&vm->data);
    sblcell_t b = stk_pop(&vm->data);

    if(a.type != CELL_BOOL || b.type != CELL_BOOL){
        fprintf(stderr,"ERROR: mismatching types on and operation");
        exit(1);
    }

    stk_push(&vm->data,sblbool(a.as.i || b.as.i));
}
fnop(op_not){
    sblcell_t a = stk_pop(&vm->data);
    stk_push(&vm->data,sblbool(!a.as.u));
}

// IO

fnop(op_top){
    sblcell_print(vm->data.data[vm->data.sp - 1]);
}

fnop(op_put){
    sblcell_t c = vm->data.data[vm->data.sp - 1];
    if(c.type != CELL_INT && c.type != CELL_UINT){
        printf("ERROR: Tried to print non integer as character");
        exit(1);
    }
    printf("%c",(char)vm->data.data[vm->data.sp - 1].as.i);
}
fnop(op_dump){
    sblcell_t x = stk_pop(&vm->data);
    sblcell_print(x);
}
fnop(op_trace){
    stk_trace(&vm->data);
}
fnop(op_puts){
    uint32_t off = vm->data.data[vm->data.sp - 1].as.u;
    printf("%s",vm->statmem.data + off);
}
