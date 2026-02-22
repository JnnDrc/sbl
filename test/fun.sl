# [a b] -> [(a^2 + b^2)]
:sqrsum
sqr     # [a b]     -> [a b^2]
swap    # [a b^2]   -> [b^2 a]
sqr     # [b^2 a]   -> [b^2 a^2]
add     # [b^2 a^2] -> [(a^2 + b^2)]
ret

:main
push 3
push 2
call sqrsum
top
