# Stack Based Language

SBL is a super simple stack/concatenative programming language

    push 1 push 2
    add

it aims on being simple and easy to follow

## Usage

### Compile
just type make on terminal and it should build everything

### Use

you will get 4 binaries:
- sbl       : interpreter
- sblc      : bytecode compiler
- sbli      : binary inspector/analyzer
- sblr      : repl

write a code in file (usualy .sl) and compile it with sblc bytecode (usualy .bl)
the virtual machine will execute the bytecode

the inspector is used to see informations about the binary, like it's instructions
and the constants table

## Introduction

SBL programs consists of a list of operations that modify the operate on the stack.
operations are separated by space characters (white space, new line, tab ...)

valid sbl:
```sbl
push 34
push 35
add
```
also valid sbl:
```sbl
push 34     push 35 add
```

### Argumented operations

some operations need external data that is passed as arguments after the operation:
- push value
- jump offset/label
- call offset/label

number literals and labels that are not argument of one of these are interpreted
as push and call respectively
this code
```sbl
push 1
dup
call foo
```
is equivalent to
```sbl
1 dup foo
```

### Labels

labels are declared using ':' before the name of the label
```
:sqrsum sqr swap sqr add ret
```
the restricted label ':main' is interpreted as the entry point of the program,
if no :main label is found, the entry point is the first instruction of the file

### While-do and If-then-else

on top of labels, sbl also supports some common syntax sugars like while-do loops
and if-then-else blocks

#### While loop
while loops are made like this:

```sbl
    10 while dup 0 gt do top dec end
```
this is the equivalent with labels:
```
10 
:WHILE 
dup 0 gt
hop jump END
:DO
top dec
jump WHILE
:END
```

#### If-then-else block

if-then-else blocks are made like this:

```sbl
    10 if dup 5 eq then dup top else 13 top end
```

this is the equivalent with labels:
```sbl
10
:IF
dup 5 eq
hop jump ELSE
:THEN
dup top
jump END
:ELSE
13 top
:END
```

there is also the if-then block, without else

```sbl
    10 if dup 5 eq then dup top end
```

```sbl
10
:IF
dup 5 eq
hop jump END
:THEN
dup top
:END
```

### Operations:
- none  : end of program
- add   : b a -- (a + b)
- sub   : b a -- (a - b)
- mul   : b a -- (a * b)
- div   : b a -- (a / b)
- mod   : b a -- (a % b)
- inc   :   a -- (a + 1)
- dec   :   a -- (a + 1)
- sum   : xn xn-1 .. x1 n -- s
- prod  : xn xn-1 .. x1 n -- p

- push k :   -- a
- pop    : a -- 
- swap   : a b -- b a
- dup    : a -- a a
- over   : a b -- a b a
- rot    : a b c -- b c a
- +rot   : a b c -- b c a
- -rot   : a b c -- c a b

- jump k/l : {ip = ip + off}
- hop      : c --  {c ? ip+2 : ip+1}

- gt    : b a -- (a > b)
- lt    : b a -- (a < b)
- ge    : b a -- (a >= b)
- le    : b a -- (a <= b)
- eq    : b a -- (a == b)
- ne    : b a -- (a != b)

- pow   : b a -- (a^b)
- sqr   :   a -- (a*a)
- sqrt  :   a -- (sqrt(a))

- call k/l : {ip = ip + off, push ip to call stack}
- ret   : {pop ip (call stack)}

- top   : {print top element}
- put   : {print top element as char}
- dump  : {pop and print top element}
- trace : {print entire stack}
