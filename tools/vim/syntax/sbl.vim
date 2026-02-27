" Syntax file
" Language: SBL (Stack Based Language)

if exists("b:current_syntax")
    finish
endif

syntax keyword sblKeyword none
syntax keyword sblKeyword add sub mul div mod inc dec
syntax keyword sblKeyword iadd isub imul idiv imod iinc idec ipow isqr
syntax keyword sblKeyword uadd usub umul udiv umod uinc udec upow usqr
syntax keyword sblKeyword fadd fsub fmul fdiv fmod finc fdec fpow fsqr fsqrt
syntax keyword sblKeyword push pop drop swap dup over rot -rot +rot
syntax keyword sblKeyword jump jmp hop
syntax keyword sblKeyword eq ne
syntax keyword sblKeyword gt lt ge le
syntax keyword sblKeyword igt ilt ige ile
syntax keyword sblKeyword ugt ult uge ule
syntax keyword sblKeyword fgt flt fge fle
syntax keyword sblKeyword and or not
syntax keyword sblKeyword call ret
syntax keyword sblKeyword top put dump trace puts

syntax match sblComment "#.*$"
syntax match sblFloat  "\v[-+]?\d+\.\d+[f]"
syntax match sblInt    "\v[-+]?\d+[ui]"

syntax match sblLabelDecl "\v:[A-Za-z_][A-Za-z0-9_]*"
syntax match sblLabelUse  "\v<[A-Za-z_][A-Za-z0-9_]*>"

syntax region sblString start="\"" end="\""

highlight default link sblKeyword   Keyword
highlight default link sblComment   Comment
highlight default link sblString    String
highlight default link sblFloat     Number
highlight default link sblInt       Number
highlight default link sblLabelDecl Label
highlight default link sblLabelUse  Identifier

let b:current_syntax = "sbl"
