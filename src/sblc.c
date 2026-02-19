// SBL Compiler
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sblop.h"
#include "sblconst.h"
#include "sblinst.h"

#include "sblcc.h"

int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr,"USAGE: %s input [output]", argv[0]);
        return -1;
    }

    FILE* fin  = fopen(argv[1],"r");
    char* out  = argv[2] ? argv[2] : "sbl.out";
    FILE* fout = fopen(out,"wb");

    if(!fin){
        fprintf(stderr,"ERROR: failed to open file %s",argv[1]);
        return -1;
    }
    if(!fout){
        fprintf(stderr,"ERROR: can't open output file");
        return -1;
    }

    constabl_t ct;
    ilist_t    il;
    lablist_t ll;
    constabl_init(&ct);
    ilist_init(&il);
    lablist_init(&ll);
    
    char line[128] = {0};
    size_t l = 0;
    while(fgets(line,sizeof(line),fin)){
        l++;
        if(strlen(line) == 0) continue;
        if(line[0] == '#') continue;
        
        int r = sblc_compile_line(line,&il,&ct,&ll);
        switch(r){
            case 0: break;
            case -1:
                    fprintf(stderr,"ERROR: unknown operation at line %zu",l);
                    return -1;
            case -2:
                    fprintf(stderr,"ERROR: Unknown label at %zu",l);
                    return -1;
        }
    }
    ilist_add(&il,MAKE_OP(OP_NONE));

    sblc_emit(fout,&il,&ct, &ll);

    return 0;
}
