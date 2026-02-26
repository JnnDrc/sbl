#ifndef _SBLCC_H
#define _SBLCC_H

#include <stdio.h>

#include "sbly.h"
#include "sblinst.h"
#include "sblconst.h"

#define COMMENT '#'
#define LABEL   ':'

typedef struct label{
    char*    label;
    uint32_t line;
}label_t;

#define LABLIST_INIT_CAPACITY 32
typedef struct lablist{
    uint32_t size;
    uint32_t capacity;
    label_t* data;
}lablist_t;

label_t label(char* id, uint32_t line);
int lablist_init(lablist_t* ll);

int       label_add(lablist_t* ll, label_t l);
int32_t   label_find(lablist_t* ll, char* id);

// -------------------------------------

typedef struct patch{
    char*    label;
    uint32_t inst_addr;
    uint32_t line, column;
}patch_t;

#define PATCH_TABLE_INIT_CAP 128
typedef struct patchtable{
    uint32_t capacity;
    uint32_t size;
    patch_t* data;
}patchtable_t;

patch_t patch(char* label, uint32_t addr, uint32_t line, uint32_t column);

int patchtable_init(patchtable_t* pt);
int patch_add(patchtable_t* pt, patch_t p);

// -------------------------------------

int sblc_compile_line(char* line, ilist_t* il, constabl_t* ct, lablist_t* ll);
void sblc_emit(FILE* fp, ilist_t* il, constabl_t* ct, lablist_t* ll);

int sblc_patch(patch_t p, iolist_t* iol,constabl_t* ct, lablist_t* ll);
int sblc_compile_op(token_t cur, lexer_t* lex, iolist_t* iol, constabl_t* ct, lablist_t* ll, patchtable_t* pt);
int sblc_compile_token(token_t cur, lexer_t* lex, iolist_t* iol, constabl_t* ct, lablist_t* ll, patchtable_t* pt);

#endif /* _SBLCC_H */ 
