#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "sbly.h"

#include "sblop.h"
#include "sbldef.h"

char* tokk_string(tokind_n tk){
    switch(tk){
        case TOK_NONE:          return "NONE";
        case TOK_INTLIT:        return "INTLIT";
        case TOK_FLOATLIT:      return "FLOATLIT";
        case TOK_STRLIT:        return "STRLIT";
        case TOK_LABEL_DEF:     return "LABEL_DEF";
        case TOK_IDENT:         return "IDENT";
        case TOK_OP:            return "OP";
        case TOK_IF:            return "IF";
        case TOK_THEN:          return "THEN";
        case TOK_ELSE:          return "ELSE";
        case TOK_WHILE:         return "WHILE";
        case TOK_DO:            return "DO";
        case TOK_END:           return "END";
        default:                return "UNKNOWN";
    }
}
char* keywords[] = {"if","then","else","while","do","end"};
tokind_n iskwd(const char* str, int len){
    for(size_t i = 0; i < sizeof(keywords)/sizeof(*keywords); i++){
        if(streqn(str,keywords[i],len)) return 21 + i;
    }
    return 0;
}

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
    
    bool is_fp = false;
    
    if(l->src[l->pos] == '+' || l->src[l->pos] == '-'){
        l->pos++;
        l->column++;
    }

    while(l->pos < l->len && isdigit((int)l->src[l->pos])){
        l->pos++;
        l->column++;
    }

    if(l->pos < l->len && l->src[l->pos] == '.'){
        is_fp  = true;
        l->pos++;
        l->column++;
        while(l->pos < l->len && isdigit((int)l->src[l->pos])){
            l->pos++;
            l->column++;
        }
    }

    char suffix = 0;
    if(l->pos < l->len){
        char c = l->src[l->pos];
        if(c == 'f' || c == 'i' || c == 'u'){
            suffix = c;
            l->pos++;
            l->column++;
        }
    }
    
    token_t t = {0};
    t.start   = l->src + start;
    t.len     = l->pos - start;
    t.line    = l->line;
    t.column  = col;

    if(is_fp || suffix == 'f'){
        t.kind  = TOK_FLOATLIT;
        t.num.f = strtofp(t.start,NULL);
    }
    else if(suffix == 'u'){
        t.kind  = TOK_UINTLIT;
        t.num.u = strtou(t.start,NULL,10);
    }
    else{
        t.kind  = TOK_INTLIT;
        t.num.i = strtoi(t.start,NULL,10);
    }

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
        else{
            tokind_n kw = iskwd(t.start,t.len);
            if(kw) t.kind = kw;
            else t.kind = TOK_IDENT;
        }
    }
    return t;
}

static token_t lex_string(lexer_t* l){
    size_t start = l->pos;
    uint32_t col = l->column;
    
    while(l->pos < l->len){
        l->pos++;
        l->column++;
        if(l->src[l->pos] == '\n'){
            l->line++;
            l->column = 1;
        }
        if (l->src[l->pos] == '"'){
            l->pos++;
            l->column++;
            break;
        }
    }

    token_t t = {0};
    t.kind    = TOK_STRLIT;
    t.start   = l->src+start+1;
    t.len     = l->pos - start - 2;
    t.line    = l->line;
    t.column  = col;

    return t;
}

token_t lex_next(lexer_t* l){
    skip_ws_comment(l);

    if(l->pos >= l->len) return (token_t){.kind = TOK_NONE};

    char c = l->src[l->pos];
    
    if((c == '+' || c == '-') && isdigit(l->src[l->pos+1])) return lex_number(l);

    if ((int)isdigit(c)) return lex_number(l);
    
    if(c == '"') return lex_string(l);

    return lex_ident(l);
}

void lex_rewind(lexer_t* l){
    l->pos    = 0;
    l->line   = 1;
    l->column = 1;
}
