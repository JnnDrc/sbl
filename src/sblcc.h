#ifndef _SBLCC_H
#define _SBLCC_H

#include <stdio.h>

#include "sblinst.h"
#include "sblconst.h"

int sblc_compile_line(char* line, ilist_t* il, constabl_t* ct);
void sblc_emit(FILE* fp, ilist_t* il, constabl_t* ct);

#endif /* _SBLCC_H */ 
