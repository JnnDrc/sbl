# [a b] -> [(a^2 + b^2)]
:sqrsum sqr swap sqr add ret

:main
push 3 push 2
call sqrsum
top
