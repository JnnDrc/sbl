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

#define F_HASIN     2 << 0
#define F_HASOUT    2 << 1
#define F_LEXDRAIN  2 << 2

int main(int argc, char* argv[]){
    uint8_t flags = 0;
    if(argc < 2){
        fprintf(stderr,"USAGE: %s input [-o output] [flags]", argv[0]);
        return -1;
    }
    char *in = NULL, *out = "sbl.out";
    for(int i = 1; i < argc; i++){
        if(argv[i][0] != '-'){
            if(!HASF(flags,F_HASIN)){
                in = argv[i];
                SETF(flags,F_HASIN);
            }else{
                fprintf(stderr,"ERROR: Passed two input files, maximum of one");
                return -1;
            }
        }
        else{
            if(streq(argv[i],"-o")){
                if(!HASF(flags,F_HASOUT)){
                    out = argv[++i];
                    SETF(flags,F_HASOUT);
                }else{
                    fprintf(stderr,"ERROR: Passed two output files, maximum of one");
                    return -1;
                }
            }
            if(streq(argv[i],"--lex")) SETF(flags,F_LEXDRAIN);
        }
    }

    FILE* fin  = fopen(in,"r");
    FILE* fout = fopen(out,"wb");

    if(!fin){
        fprintf(stderr,"ERROR: failed to open input file %s",in);
        return -1;
    }
    if(!fout){
        fprintf(stderr,"ERROR: failed to open output file %s",out);
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

    if(HASF(flags,F_LEXDRAIN)){
        for(token_t cur = lex_next(&l); cur.kind != TOK_NONE; cur = lex_next(&l))
            fprintf(HASF(flags,F_HASOUT) ? fout : stdout,
                    "%d:%d:\t  %s \'%.*s\'\n",
                    cur.line,cur.column,
                    tokk_string(cur.kind),
                    (int)cur.len,cur.start);
        return 0;
    }

    patchtable_t pt;
    iolist_t   iol;
    iolist_init(&iol);
    patchtable_init(&pt);

    constabl_t ct;
    lablist_t ll;
    constabl_init(&ct);
    lablist_init(&ll);
    statbuf_t sb;
    sb_init(&sb);

    // compile to instruction objects
    for(token_t cur = lex_next(&l);
        !sblc_compile_token(cur,&l,&iol,&ct,&ll,&pt,&sb);
        cur = lex_next(&l));

    // backpatch unresolved labels
    for(uint i = 0; i < pt.size; i++) sblc_patch(pt.data[i],&iol,&ct,&ll);

    // assemble instruction objects to vm instructions
    ilist_t il = iolist_make(iol);

    // emit to file
    sblc_emit(fout,&il,&ct, &ll,&sb);

    fclose(fout);
    return 0;
}
