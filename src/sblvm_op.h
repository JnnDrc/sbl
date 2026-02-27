#ifndef _SBLVM_OP_H
#define _SBLVM_OP_H

#include "sblvm.h"

fnop(op_none);
fnop(op_nop);
fnop(op_halt);

fnop(op_jump);
fnop(op_hop);


fnop(op_call);

fnop(op_ret);

fnop(op_iadd);
fnop(op_isub);
fnop(op_imul);
fnop(op_idiv);
fnop(op_imod);
fnop(op_iinc);
fnop(op_idec);
fnop(op_ipow);
fnop(op_isqr);

fnop(op_uadd);
fnop(op_usub);
fnop(op_umul);
fnop(op_udiv);
fnop(op_umod);
fnop(op_uinc);
fnop(op_udec);
fnop(op_upow);
fnop(op_usqr);

fnop(op_fadd);
fnop(op_fsub);
fnop(op_fmul);
fnop(op_fdiv);
fnop(op_fmod);
fnop(op_finc);
fnop(op_fdec);
fnop(op_fpow);
fnop(op_fsqr);
fnop(op_fsqrt);

fnop(op_push);
fnop(op_pop);
fnop(op_swap);
fnop(op_dup);
fnop(op_over);
fnop(op_lrot);
fnop(op_rrot);

fnop(op_eq);
fnop(op_ne);

fnop(op_igt);
fnop(op_ilt);
fnop(op_ige);
fnop(op_ile);

fnop(op_and);
fnop(op_or);
fnop(op_not);

fnop(op_top);
fnop(op_put);
fnop(op_dump);
fnop(op_trace);
fnop(op_puts);
#endif /* _SBLVM_OP_H */
