#ifndef _SBLY_H
#define _SBLY_H

#include <stdint.h>
#include "sblcell.h"

typedef enum token_kind{
    TOK_NONE     = 0,      // nothing, eof
    TOK_INTLIT   = 1,      // signed literal
    TOK_UINTLIT  = 2,      // unsigned literal
    TOK_FLOATLIT = 3,      // float literal
    TOK_STRLIT   = 4,      // string literal (reserved for future)

    TOK_LABEL_DEF = 10,  // :foo
    TOK_IDENT     = 11,  // foo
    TOK_OP        = 12,  // builting operator ("keyword")
    
    // if abstraction
    TOK_IF = 21,
    TOK_THEN = 22,
    TOK_ELSE = 23,
    // while abstraction
    TOK_WHILE = 24,
    TOK_DO  = 25,
    // end
    TOK_END = 26,
}tokind_n;

char* tokk_string(tokind_n tk);

typedef struct token{
    tokind_n kind;
    const char* start;
    size_t len;
    union{
        sblfloat_t f;
        sbluint_t  u;
        sblint_t   i;
    }num;
    int16_t  op;
    uint32_t line, column;
}token_t;

typedef struct lexer{
    const char* src;
    size_t pos;
    size_t len;
    uint32_t line, column;
}lexer_t;

lexer_t lexer(char* src, size_t len);

token_t lex_next(lexer_t* l);

void lex_rewind(lexer_t* l);

#endif /* _SBLY_H */
