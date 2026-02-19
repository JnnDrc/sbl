#ifndef _SBLCC_H
#define _SBLCC_H

#include <stdio.h>

#include "sblinst.h"
#include "sblconst.h"

#define COMMENT '#'
#define LABEL   ':'

typedef struct label{
    char*    label;
    uint32_t line;
}label_t;

#define LABLIST_INIT_CAPACITY 16
typedef struct lablist{
    uint32_t size;
    uint32_t capacity;
    label_t* data;
}lablist_t;

label_t label(char* id, uint32_t line);
int lablist_init(lablist_t* ll);

int       label_add(lablist_t* ll, label_t l);
int32_t   label_find(lablist_t* ll, char* id);

int sblc_compile_line(char* line, ilist_t* il, constabl_t* ct, lablist_t* ll);
void sblc_emit(FILE* fp, ilist_t* il, constabl_t* ct);

#endif /* _SBLCC_H */ 
