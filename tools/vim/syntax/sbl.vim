" Syntax file
" Language: SBL (Stack Based Language)

if exists("b:current_syntax")
    finish
endif

syntax keyword sblKeyword none
syntax keyword sblKeyword add sub mul div mod inc dec sum prod
syntax keyword sblKeyword push pop swap dup over rot -rot +rot
syntax keyword sblKeyword jump hop
syntax keyword sblKeyword gt lt ge le eq ne
syntax keyword sblKeyword pow sqr sqrt
syntax keyword sblKeyword call ret
syntax keyword sblKeyword echo top

syntax operator sblOperator + - * / %% ++ -- . ;

syntax match sblComment "#.*$"
syntax match sblFloat  "\v[-+]?\d+\.\d+"
syntax match sblInt    "\v[-+]?\d+"

syntax match sblLabelUse   "\v<[A-Za-z_][A-Za-z0-9_]*>"
syntax match sblLabelDecl  "\v:<[A-Za-z_][A-Za-z0-9_]*>"

syntax region sblString start="\"" end="\""

highlight default link sblKeyword Keyword
highlight default link sblOperator Operator
highlight default link sblComment Comment
highlight default link sblString String
highlight default link sblFloat Number
highlight default link sblInt Number
highlight default link sblLabelDecl Label
highlight default link sblLabelUse Identifier

let b:current_syntax = "sbl"
