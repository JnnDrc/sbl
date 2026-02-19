# Stack Based Language

SBL is a super simple stack programming language

it consists of simple lines of single instructions and constants to manipulate the stack
like:

    push 1
    push 2
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
