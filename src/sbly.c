#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "sbly.h"

#include "sblval.h"
#include "sblop.h"

// static char* tokk_string(tokind_n tk){
//     switch(tk){
//         case TOK_NONE:          return "NONE";
//         case TOK_NUMLIT:        return "NUMLIT";
//         case TOK_STRLIT:        return "STRLIT";
//         case TOK_LABEL_DEF:     return "LABEL_DEF";
//         case TOK_IDENT:         return "IDENT";
//         case TOK_OP:            return "OP";
//       break;
//     }
//     return "UNKNOWN";
// }

lexer_t lexer(char* src, size_t len){
    return (lexer_t){.src = src, .len = len,.pos = 0, .line = 1, .column = 1};
}

static void skip_ws(lexer_t *l){
    while(l->pos < l->len){
        char c = l->src[l->pos];
        if (c == '\n'){
            l->line++;
            l->column = 1;
        }
        if (!isspace((int)c)) break;
        l->pos++;
        l->column++;
    }
}
static void skip_comment(lexer_t *l){
    if (l->pos >= l->len || l->src[l->pos] != '#') return;
    l->pos++;
    l->column++;

    while(l->pos < l->len && l->src[l->pos] != '\n'){
        l->pos++;
        l->column++;
    }
}

static void skip_ws_comment(lexer_t *l){
    for(;;){
        skip_ws(l);
        if(l->pos < l->len && l->src[l->pos] == '#') skip_comment(l);
        else break;
    }
}

static token_t lex_number(lexer_t *l){
    size_t start = l->pos;
    uint32_t col = l->column;

    while(l->pos < l->len && isdigit((int)l->src[l->pos])){
        l->pos++;
        l->column++;
    }

    token_t t = {0};
    t.kind    = TOK_NUMLIT;
    t.start   = l->src + start;
    t.len     = l->pos - start;
    t.number  = strton(t.start,NULL);
    t.line    = l->line;
    t.column  = col;

    return t;
}

static token_t lex_ident(lexer_t *l){
    size_t start = l->pos;
    uint32_t col = l->column;

    while(l->pos < l->len && !isspace((int)l->src[l->pos])){
        l->pos++;
        l->column++;
    }

    token_t t = {0};
    t.start   = l->src + start;
    t.len     = l->pos - start;
    t.line    = l->line;
    t.column  = col;

    if (t.start[0] == ':'){
        t.kind = TOK_LABEL_DEF;
    }else{
        int32_t o = sblo_isopn(t.start,t.len);
        if (o){
            t.kind = TOK_OP;
            t.op = o;
        }
        else t.kind = TOK_IDENT;
    }
    return t;
}

token_t lex_next(lexer_t* l){
    skip_ws_comment(l);

    if(l->pos >= l->len) return (token_t){.kind = TOK_NONE};

    char c = l->src[l->pos];

    if ((int)isdigit(c)) return lex_number(l);

    return lex_ident(l);
}

void lex_rewind(lexer_t* l){
    l->pos    = 0;
    l->line   = 1;
    l->column = 1;
}
