# NEED TO IMPLEMENT FORWARD JUMP
# [n] -> [f]
:fac 1 swap
:L1 dup 1 ne hop jump L2
swap over mul
swap dec jump L1
:L2 pop ret

:main
10
fac
top
