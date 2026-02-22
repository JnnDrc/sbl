#ifndef _SBLY_H
#define _SBLY_H

#include <stdint.h>
#include "sblval.h"

typedef enum token_kind{
    TOK_NONE,       // nothing, eof
    TOK_NUMLIT,     // number literal
    TOK_STRLIT,     // string literal (reserved for future)

    TOK_LABEL_DEF,  // :foo
    TOK_IDENT,      // foo
    TOK_OP,         // builting operator ("keyword") (implement later)
}tokind_n;

typedef struct token{
    tokind_n kind;
    const char* start;
    size_t len;
    sblnum_t number;
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
