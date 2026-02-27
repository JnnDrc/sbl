#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "sblcc.h"

#include "sblop.h"
#include "sblconst.h"
#include "sblinst.h"
#include "sblvm.h"
#include "sbldef.h"
#include "sbly.h"
#include "sblcell.h"

int sblc_patch(patch_t p, iolist_t* iol,constabl_t* ct, lablist_t* ll){
    intmax_t l = label_find(ll,p.label);
    if (l < 0){
        fprintf(stderr,"%d:%d: call/jump to undeclared label '%s'\n",p.line,p.column,p.label);
        exit(1);
    }
    sblcell_t off = sblint(l - (intmax_t)p.inst_addr);
    int ci = const_find(ct,off);
    if (ci < 0) ci = const_add(ct,off);
    iol->data[p.inst_addr].arg.k = ci;

    return 0;
}

int sblc_compile_op(token_t cur, lexer_t* lex, iolist_t* iol, constabl_t* ct, lablist_t* ll, patchtable_t* pt, statbuf_t* sb){
    char buf[128];
    switch (cur.op) {
        case OP_PUSH:
            {
                token_t next = lex_next(lex);
                switch(next.kind){
                    case TOK_INTLIT:
                        {
                            sblcell_t n = sblint(next.num.i);
                            int ci = const_find_or_add(ct,n);
                            iolist_add(iol,iobj_opk(OP_PUSH,ci));
                            break;
                        }
                    case TOK_UINTLIT:
                        {
                            sblcell_t n = sbluint(next.num.u);
                            int ci = const_find_or_add(ct,n);
                            iolist_add(iol,iobj_opk(OP_PUSH,ci));
                            break;
                        }
                    case TOK_FLOATLIT:
                        {
                            sblcell_t n = sblfloat(next.num.f);
                            int ci = const_find_or_add(ct,n);
                            iolist_add(iol,iobj_opk(OP_PUSH,ci));
                            break;
                        }
                    case TOK_STRLIT:
                        {
                            char* tmp = malloc(cur.len+1);
                            if(!tmp){
                                fprintf(stderr, "PANIC: Failed to allocate memory");
                                exit(-1);
                            }
                            memcpy(tmp,cur.start,cur.len);
                            tmp[cur.len] = '\0';
                            size_t off = sb_create(sb,tmp,cur.len+1);
                            free(tmp);

                            sblcell_t voff = sblint(off);
                            int ci = const_find_or_add(ct,voff);
                            iolist_add(iol,iobj_opk(OP_PUSH,ci));
                            break;
                        }
                    default:
                        fprintf(stderr, "%d:%d: expected operand for push",cur.line,cur.column);
                        exit(1);
                }
                break;
            }
        case OP_JUMP:
        case OP_CALL:
            {
                token_t next = lex_next(lex);
                strncpy(buf,next.start,next.len);
                buf[next.len] = '\0';
                if (next.kind == TOK_INTLIT || next.kind == TOK_UINTLIT){
                    sblcell_t off = next.kind == TOK_INTLIT ? sblint(next.num.i) : sbluint(next.num.u);
                    int ci = const_find(ct,off);
                    if (ci < 0) ci = const_add(ct, off);
                    iolist_add(iol,iobj_opk(cur.op,ci));
                }
                else if(next.kind == TOK_IDENT){
                    intmax_t l = label_find(ll,buf);
                    if(l < 0){
                        patch_add(pt,patch(buf,iol->size,cur.line,cur.column));
                        iolist_add(iol,iobj_opk(cur.op,INT32_MAX));
                    }
                    else{
                        sblcell_t off = sblint(l - (intmax_t)iol->size);
                        int ci = const_find(ct,off);
                        if (ci < 0) ci = const_add(ct, off);
                        iolist_add(iol,iobj_opk(cur.op,ci));
                    }
                }
                else {
                    fprintf(stderr,"%d:%d: expected offset or label for jump/call",cur.line,cur.column);
                    exit(1);
                }
            }
            break;
        default:
            iolist_add(iol,iobj_op(cur.op));
            break;
    }
    return 0;
}

int sblc_compile_token(token_t cur, lexer_t* lex, iolist_t* iol, constabl_t* ct, lablist_t* ll, patchtable_t* pt, statbuf_t* sb){
    char    buf[128];
    switch(cur.kind){
    case TOK_NONE:
        {
            iolist_add(iol,iobj_op(OP_NONE));
            return 1;
        }
    case TOK_INTLIT:
        {
            sblcell_t n = sblint(cur.num.i);
            int ci = const_find(ct,n);
            if (ci < 0) ci = const_add(ct,n);
            iolist_add(iol,iobj_opk(OP_PUSH,ci));
            break;
        }
    case TOK_UINTLIT:
        {
            sblcell_t n = sbluint(cur.num.u);
            int ci = const_find(ct,n);
            if (ci < 0) ci = const_add(ct,n);
            iolist_add(iol,iobj_opk(OP_PUSH,ci));
            break;
        }
    case TOK_FLOATLIT:
        {
            sblcell_t n = sblfloat(cur.num.f);
            int ci = const_find(ct,n);
            if (ci < 0) ci = const_add(ct,n);
            iolist_add(iol,iobj_opk(OP_PUSH,ci));
            break;
        }
    case TOK_STRLIT:
        {
            char* tmp = malloc(cur.len+1);
            if(!tmp){
                fprintf(stderr, "PANIC: Failed to allocate memory");
                exit(-1);
            }
            memcpy(tmp,cur.start,cur.len);
            tmp[cur.len] = '\0';
            size_t off = sb_create(sb,tmp,cur.len+1);
            free(tmp);
            sblcell_t voff = sblint(off);
            int ci = const_find_or_add(ct,voff);
            iolist_add(iol,iobj_opk(OP_PUSH,ci));
            break;
        }
    case TOK_LABEL_DEF:
        {
            strncpy(buf,cur.start + 1,cur.len - 1);
            buf[cur.len - 1] = '\0';

            if (label_find(ll,buf) >= 0){
                fprintf(stderr,"%d:%d: redeclaration of label '%s'\n",cur.line,cur.column,buf);
                exit(1);
            }

            label_add(ll,label(buf,iol->size));
            break;
        }
    case TOK_IDENT:
        {
            strncpy(buf,cur.start,cur.len);
            buf[cur.len] = '\0';
            intmax_t l = label_find(ll,buf);

            if(l < 0 ){
                patch_add(pt,patch(buf,iol->size,cur.line,cur.column));
                iolist_add(iol,iobj_opk(OP_CALL,INT32_MAX));
            }
            else{
                sblcell_t off = sblint(l - (intmax_t)iol->size);
                int ci = const_find(ct,off);
                if (ci < 0) ci = const_add(ct,off);
                iolist_add(iol,iobj_opk(OP_CALL,ci));
            } 
            break;
        }
    case TOK_OP:
        {
            strncpy(buf,cur.start,cur.len);
            buf[cur.len] = '\0';
            sblc_compile_op(cur,lex,iol,ct,ll,pt,sb);
            break;
        }
    case TOK_IF:
        {
            size_t __then, __else, __end;
            for(token_t tok = lex_next(lex); tok.kind != TOK_THEN; tok = lex_next(lex))
                sblc_compile_token(tok,lex,iol,ct,ll,pt,sb);
            // hop jump __IF_ELSE
            iolist_add(iol,iobj_op(OP_HOP));

            __then = iol->size; // save address for backpatch
            iolist_add(iol,iobj_opk(OP_JUMP,INT32_MAX));

            token_t tok;
            for(tok = lex_next(lex);
                tok.kind != TOK_ELSE && tok.kind != TOK_END;
                tok = lex_next(lex)) sblc_compile_token(tok,lex,iol,ct,ll,pt,sb);
            
            if(tok.kind == TOK_ELSE){
                size_t __jump_to_end_addr = iol->size;
                iolist_add(iol,iobj_opk(OP_JUMP,INT32_MAX));

                __else = iol->size;
                for(token_t ntok = lex_next(lex); ntok.kind != TOK_END; ntok = lex_next(lex))
                    sblc_compile_token(ntok,lex,iol,ct,ll,pt,sb);

                __end = iol->size;

                int eci = const_find_or_add(ct,sblint(__end - __jump_to_end_addr));
                iol->data[__jump_to_end_addr].arg.k = eci;
            }else if(tok.kind == TOK_END){
                // in case of no else, the else is the end, respecting the structure
                // :if cond hop jump else :then body jump end :else body :end
                // in this case, the structure is
                // :if cond hop jump else :then body :else
                __else = iol->size;
            }else{
                fprintf(stderr,"%d:%d: unknown error",tok.line,tok.column);
                exit(1);
            }

            int elci = const_find_or_add(ct,sblint(__else - __then));
            iol->data[__then].arg.k = elci;
            break;
        }
    case TOK_THEN:
        {
            fprintf(stderr,"%d:%d: unexpected 'then' without if",cur.line,cur.column);
            exit(1);
            break;
        }
    case TOK_ELSE:
        {
            fprintf(stderr,"%d:%d: unexpected 'else' without if-then",cur.line,cur.column);
            exit(1);
            break;
        }
    case TOK_WHILE:
        {
            size_t __while = iol->size;
            for(token_t tok = lex_next(lex); tok.kind != TOK_DO; tok = lex_next(lex))
                sblc_compile_token(tok,lex,iol,ct,ll,pt,sb);
            // hop jump __WHILE_END
            iolist_add(iol,iobj_op(OP_HOP));

            size_t __do = iol->size;      // save address for backpatch
            iolist_add(iol,iobj_opk(OP_JUMP, INT32_MAX));
            
            for(token_t tok = lex_next(lex); tok.kind != TOK_END; tok = lex_next(lex))
                sblc_compile_token(tok,lex,iol,ct,ll,pt,sb);
            // add jump __WHILE
            int wci = const_find_or_add(ct,sblint(__while - iol->size));
            iolist_add(iol,iobj_opk(OP_JUMP,wci));

            // patch jump _WHILE_END
            size_t __end = iol->size;
            int eci = const_find_or_add(ct,sblint(__end - __do));
            iol->data[__do].arg.k = eci;

            break;
        }
    case TOK_DO:
        {
            fprintf(stderr,"%d:%d: unexpected 'do' without while",cur.line,cur.column);
            exit(1);
            break;
        }
    case TOK_END:
        {
            fprintf(stderr,"%d:%d: unexpected 'end' without while-do/if-then-else block",cur.line,cur.column);
            exit(1);
            break;
        }
    }
    return 0;
}

void sblc_emit(FILE* fp, ilist_t* il, constabl_t* ct, lablist_t* ll, statbuf_t* sb){
    // header
    sblbinh_t bi = sbl_make_info(il,ct,ll,sb);
    fwrite(&bi,sizeof(bi),1,fp);

    // instructions
    fwrite(il->data,sizeof(*il->data),il->size,fp);
    // const table
    fwrite(ct->data,sizeof(*ct->data),ct->size,fp);
    // static buffer
    fwrite(sb->data,1,sb->size,fp);
}

label_t label(char* id, uint32_t line){
    return (label_t){.label = strdup(id), .line = line};
}

int lablist_init(lablist_t* ll){
    ll->capacity = LABLIST_INIT_CAPACITY;
    ll->size = 0;
    ll->data = calloc(ll->capacity,sizeof(label_t));
    if(!ll->data) return -1;

    return 0;
}

int label_add(lablist_t* ll, label_t l){
    if (ll->size >= ll->capacity){
        uint32_t ncap = ll->capacity * 2;
        label_t* ndata = calloc(ncap, sizeof(label_t));
        if(!ndata) return -1;
        memcpy(ndata,ll->data,ll->size);
        free(ll->data);
        ll->data = ndata;
        ll->capacity = ncap;
    }
    ll->data[ll->size++] = l;
    return 0;
}
int32_t  label_find(lablist_t* ll, char* id){
    for (uint i = 0; i < ll->size; i++) if(streq(ll->data[i].label,id)) return ll->data[i].line;
    return -1;
}

patch_t patch(char* label, uint32_t addr, uint32_t line, uint32_t column){
    return (patch_t){.label = strdup(label), .inst_addr = addr, .line = line, .column = column};
}

int patchtable_init(patchtable_t* pt){
    pt->size = 0;
    pt->capacity = PATCH_TABLE_INIT_CAP;

    pt->data = calloc(pt->capacity,sizeof(patch_t));
    if(!pt->data) return -1;

    return 0;
}

int patch_add(patchtable_t* pt, patch_t p){
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

int sb_init(statbuf_t* sb){
    sb->size = 0;
    sb->capacity = STATBUF_INIT_CAP;

    sb->data = calloc(sb->capacity,sizeof(inst_t));
    if(!sb->data) return -1;

    return 0;
}

uint32_t sb_create(statbuf_t* sb, void* data, size_t size){
    if (sb->size >= sb->capacity){
        uint32_t ncap = sb->capacity * 2;
        char* ndata = calloc(ncap, sizeof(char));
        if(!ndata) return -1;
        memcpy(ndata,sb->data,sb->size);
        free(sb->data);
        sb->data = ndata;
        sb->capacity = ncap;
    }
    uint32_t off = sb->size;
    if(data) memcpy(sb->data+sb->size,data,size);
    sb->size += size;
    return off;
}
