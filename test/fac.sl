# NEED TO IMPLEMENT FORWARD JUMP
# [n] -> [f]
:fac
1       # 6 1
swap    # 1 6
:L1
dup     # 6 5 5
1       # 6 5 5 1
ne      # 6 5 0
hop     # 6 5
jump L2
swap   # 5 6
over   # 5 6 5
*    # 5 30
<>   # 30 5
--    # 30 4
jump L1

:L2
pop
ret

:main
10
fac
.
