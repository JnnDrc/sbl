push 1
dup
over
:LOOP
add
echo
dup
-rot
dup
push 500
swap
gt
hop
jump LOOP
