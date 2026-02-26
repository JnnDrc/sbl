# n -- f
:fac 1 <> :L1 dup 1 ~ hop jump L2 <> over * <> -- jump L1 :L2 pop ;
:main 10 fac .
