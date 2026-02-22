// SBL Compiler
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "sblop.h"
#include "sblconst.h"
#include "sblinst.h"
#include "utils.h"

#include "sblcc.h"
#include "sbly.h"

#define MAX_LINE 128

// compiler structure
// - read source
// - macro expansion
// - instruction generation
// - label address backpatching
// - emit bytecode

typedef struct patch{
    char*    label;
    uint32_t inst_addr;
    uint32_t line, column;
}patch_t;

patch_t patch(char* label, uint32_t addr, uint32_t line, uint32_t column){
    return (patch_t){.label = label, .inst_addr = addr, .line = line, .column = column};
}

#define PATCH_TABLE_INIT_CAP 128
typedef struct patchtable{
    uint32_t capacity;
    uint32_t size;
    patch_t* data;
}patchtable_t;

int patchtable_init(patchtable_t* pt){
    pt->size = 0;
    pt->capacity = PATCH_TABLE_INIT_CAP;

    pt->data = calloc(pt->capacity,sizeof(patch_t));
    if(!pt->data) return -1;

    return 0;
}

int patchtable_add(patchtable_t* pt, patch_t p){
    if (pt->size >= pt->capacity){
        uint32_t ncap = pt->capacity * 2;
        patch_t* ndata = calloc(ncap, sizeof(patch_t));
        if(!ndata) return -1;
        memcpy(ndata,pt->data,pt->size);
        free(pt->data);
        pt->data = ndata;
        pt->capacity = ncap;
    }
    pt->data[pt->size++] = p;
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

int compile_op(token_t cur, lexer_t* lex, iolist_t* iol, constabl_t* ct, lablist_t* ll, patchtable_t* pt){
    char buf[128];
    switch (cur.op) {
        case OP_PUSH:
            {
                token_t next = lex_next(lex);
                if (next.kind != TOK_NUMLIT){
                    fprintf(stderr, "%d:%d: expected operand for push",cur.line,cur.column);
                    exit(1);
                }
                sblval_t n = sblnum(next.number);
                int ci = const_find(ct,n);
                if (ci < 0) ci = const_add(ct,n);
                iolist_add(iol,iobj_opk(OP_PUSH,ci));
                break;
            }
        case OP_JUMP:
        case OP_CALL:
            {
                token_t next = lex_next(lex);
                strncpy(buf,next.start,next.len);
                buf[next.len] = '\0';
                sblval_t off;
                if (next.kind == TOK_NUMLIT){
                    off = sblint(next.number);
                }
                else if(next.kind == TOK_IDENT){
                    int l = label_find(ll,buf);
                    if(l < 0){
                        patchtable_add(pt,patch(buf,iol->size,cur.line,cur.column));
                        off = sblint(INT32_MAX);
                    }
                    else off = sblint(l - iol->size);
                }
                else {
                    fprintf(stderr,"%d:%d: expected offset or label for jump/call",cur.line,cur.column);
                    exit(1);
                }
                int ci = const_find(ct,off);
                if (ci < 0) ci = const_add(ct, off);
                iolist_add(iol,iobj_opk(cur.op,ci));
            }
            break;
        default:
            iolist_add(iol,iobj_op(cur.op));
            break;
    }
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
    
    lexer_t l = lexer(fbuf,strlen(fbuf));

    patchtable_t pt;
    iolist_t   iol;
    iolist_init(&iol);
    patchtable_init(&pt);

    constabl_t ct;
    lablist_t ll;
    constabl_init(&ct);
    lablist_init(&ll);
    
    token_t cur;
    char    buf[128];
    for(;;){
        cur = lex_next(&l);
        switch(cur.kind){
        case TOK_NONE:
            {
                iolist_add(&iol,iobj_op(OP_NONE));
                goto end;
            }
        case TOK_NUMLIT:
            {
                sblval_t n = sblnum(cur.number);
                int ci = const_find(&ct,n);
                if (ci < 0) ci = const_add(&ct,n);
                iolist_add(&iol,iobj_opk(OP_PUSH,ci));
                break;
            }
        case TOK_STRLIT:
            {
                assert(0 && "UNREACHEABLE: STRINGS ARE NOT SUPPORTED YET");
                break;
            }
        case TOK_LABEL_DEF:
            {
                strncpy(buf,cur.start + 1,cur.len - 1);
                buf[cur.len - 1] = '\0';
                label_add(&ll,label(buf,iol.size));
                break;
            }
        case TOK_IDENT:
            {
                strncpy(buf,cur.start,cur.len);
                buf[cur.len] = '\0';
                int l = label_find(&ll,buf);
                sblval_t j;
                if(l < 0 ){
                    patchtable_add(&pt,patch(buf,iol.size,cur.line,cur.column));
                    j = sblint(INT32_MAX);
                }
                else j = sblint(l - iol.size);
                int ci = const_find(&ct,j);
                if (ci < 0) ci = const_add(&ct,j);
                iolist_add(&iol,iobj_opk(OP_CALL,ci));
                break;
            }
        case TOK_OP:
            {
                strncpy(buf,cur.start,cur.len);
                buf[cur.len] = '\0';
                compile_op(cur,&l,&iol,&ct,&ll,&pt);
                break;
            }
        }
    }
end:
    
    // TODO PATCH
    ilist_t il = iolist_make(iol);
    
    sblc_emit(fout,&il,&ct, &ll);
    fclose(fout);
    return 0;
}
