// SBL Compiler
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sblop.h"
#include "sblconst.h"
#include "sblinst.h"
#include "utils.h"

#include "sblcc.h"

#define MAX_LINE 128

int address_labels(char* buf,size_t size, lablist_t* ll){
    char line[MAX_LINE] = {0};
    size_t l = 0;
    char* cur = buf;
    char* end = buf + size;

    while(strgets(line,sizeof(line),&cur,end)){
        int r = sblc_label_line(line,l,ll);
        if (!r) l++;
    }
    return 0;
}

int compile_file(char* buf,size_t size, ilist_t* il, constabl_t* ct, lablist_t* ll, size_t* ln){
    char line[MAX_LINE] = {0};
    size_t l = 0;
    char* cur = buf;
    char* end = buf + size;

    while(strgets(line,sizeof(line),&cur,end)){
        l++;
        int r = sblc_compile_line(line,il,ct,ll);
        if (r < 0){
            *ln = l;
            return r;
        }
    }
    ilist_add(il,MAKE_OP(OP_NONE));
    return 0;
}

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
    
    fseek(fin,0L,SEEK_END);
    size_t fsize = ftell(fin);
    rewind(fin);

    char* fbuf = calloc(fsize,1);
    fread(fbuf,1,fsize,fin);
    fclose(fin);

    constabl_t ct;
    ilist_t    il;
    lablist_t ll;
    constabl_init(&ct);
    ilist_init(&il);
    lablist_init(&ll);
    
    int r = 0;

    r = address_labels(fbuf, fsize, &ll);
    size_t ln;
    r = compile_file(fbuf, fsize, &il, &ct, &ll, &ln);

    switch(r){
        case 0: break;
        case -1:
                fprintf(stderr,"ERROR: unknown operation at line %zu",ln);
                return -1;
        case -2:
                fprintf(stderr,"ERROR: Unknown label at line %zu",ln);
                return -1;
    }
    sblc_emit(fout,&il,&ct, &ll);
    fclose(fout);
    return 0;
}
