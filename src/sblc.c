// SBL Compiler
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "sblconst.h"
#include "sblinst.h"
#include "sbldef.h"

#include "sblcc.h"
#include "sbly.h"

#define MAX_LINE 128

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

    // read file
    fseek(fin,0L,SEEK_END);
    size_t fsize = ftell(fin);
    rewind(fin);

    char* fbuf = calloc(fsize,1);
    fread(fbuf,1,fsize,fin);
    fclose(fin);

    // initialize compiler
    lexer_t l = lexer(fbuf,strlen(fbuf));

    patchtable_t pt;
    iolist_t   iol;
    iolist_init(&iol);
    patchtable_init(&pt);

    constabl_t ct;
    lablist_t ll;
    constabl_init(&ct);
    lablist_init(&ll);

    // compile to instruction objects
    for(token_t cur = lex_next(&l);
        !slbc_compile_token(cur,&l,&iol,&ct,&ll,&pt);
        cur = lex_next(&l));

    // backpatch unresolved labels
    for(uint i = 0; i < pt.size; i++) sblc_patch(pt.data[i],&iol,&ct,&ll);

    // assemble instruction objects to vm instructions
    ilist_t il = iolist_make(iol);
    // emit to file
    sblc_emit(fout,&il,&ct, &ll);

    fclose(fout);
    return 0;
}
