#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "sblcc.h"

#include "sblop.h"
#include "sblconst.h"
#include "sblinst.h"
#include "sblvm.h"
#include "sbldef.h"
#include "sbly.h"

#include "utils.h"

int sblc_label_line(char* line, size_t linenum, lablist_t* ll){
    strtrim(line);
    if(strlen(line) <= 0)   return -1;
    if(line[0] == COMMENT)  return -1;
    if(line[0] == LABEL){
        char *l = line+1;
        char* lab = strtok(l," ");
        if (label_find(ll,lab) < 0) label_add(ll,label(lab,linenum));
        return 1;
    }
    return 0;
}

int sblc_compile_line(char* line, ilist_t* il, constabl_t* ct, lablist_t* ll){
    strtrim(line);
    if(strlen(line) <= 0)   return 0;
    if(line[0] == COMMENT || line[0] == LABEL)  return 0;

    char*    op  = strtok(line," ");
    char*    kas = strtok(NULL," ");
    sblval_t ka  = sblnum(atof(kas));
    int16_t  b   = atoi(strtok(NULL," "));
    (void) b;

    if      (streq(op,"add") || streq(op,"+")) ilist_add(il,MAKE_OP(OP_ADD));
    else if (streq(op,"sub") || streq(op,"-")) ilist_add(il,MAKE_OP(OP_SUB));
    else if (streq(op,"mul") || streq(op,"*")) ilist_add(il,MAKE_OP(OP_MUL));
    else if (streq(op,"div") || streq(op,"/")) ilist_add(il,MAKE_OP(OP_DIV));
    else if (streq(op,"mod") || streq(op,"%")) ilist_add(il,MAKE_OP(OP_MOD));
    else if (streq(op,"inc") || streq(op,"++")) ilist_add(il,MAKE_OP(OP_INC));
    else if (streq(op,"dec") || streq(op,"--")) ilist_add(il,MAKE_OP(OP_DEC));
    else if (streq(op,"sum")) ilist_add(il,MAKE_OP(OP_SUM));
    else if (streq(op,"prod")) ilist_add(il,MAKE_OP(OP_PROD));
    else if (streq(op,"push")){
        int ci = const_find(ct,ka);
        if (ci < 0) ci = const_add(ct, ka);
        ilist_add(il, MAKE_OPK(OP_PUSH,ci));
    }
    else if (streq(op,"pop"))  ilist_add(il,MAKE_OP(OP_POP));
    else if (streq(op,"swap") || streq(op,"<>")) ilist_add(il,MAKE_OP(OP_SWAP));
    else if (streq(op,"dup"))  ilist_add(il,MAKE_OP(OP_DUP));
    else if (streq(op,"over")) ilist_add(il,MAKE_OP(OP_OVER));
    else if (streq(op,"rot"))  ilist_add(il,MAKE_OP(OP_LROT));
    else if (streq(op,"+rot")) ilist_add(il,MAKE_OP(OP_LROT));
    else if (streq(op,"-rot")) ilist_add(il,MAKE_OP(OP_RROT));
    else if (streq(op,"jump")){
        sblval_t jmp;
        // k is number
        if (isdigit(kas[0])){
            jmp = sblint(ka.as.num);
        }else {
            // k is label
            int l = label_find(ll,kas);
            if (l < 0) return -2;
            jmp = sblint(l - il->size);
        }
        int ci = const_find(ct,jmp);
        if (ci < 0) ci = const_add(ct, jmp);
        ilist_add(il,MAKE_OPK(OP_JUMP,ci));
    } 
    else if (streq(op,"hop"))  ilist_add(il,MAKE_OP(OP_HOP));
    else if (streq(op,"gt"))   ilist_add(il,MAKE_OP(OP_GT));
    else if (streq(op,"lt"))   ilist_add(il,MAKE_OP(OP_LT));
    else if (streq(op,"ge"))   ilist_add(il,MAKE_OP(OP_GE));
    else if (streq(op,"le"))   ilist_add(il,MAKE_OP(OP_LE));
    else if (streq(op,"eq"))   ilist_add(il,MAKE_OP(OP_EQ));
    else if (streq(op,"ne"))   ilist_add(il,MAKE_OP(OP_NE));
    else if (streq(op,"pow"))  ilist_add(il,MAKE_OP(OP_POW));
    else if (streq(op,"sqr"))  ilist_add(il,MAKE_OP(OP_SQR));
    else if (streq(op,"sqrt")) ilist_add(il,MAKE_OP(OP_SQRT));
    else if (streq(op,"call")){
        sblval_t jmp;
        // k is number
        if (isdigit(kas[0])){
            jmp = sblint(ka.as.num);
        }else {
            // k is label
            int l = label_find(ll,kas);
            if (l < 0) return -2;
            jmp = sblint(l - il->size);
        }
        int ci = const_find(ct,jmp);
        if (ci < 0) ci = const_add(ct, jmp);
        ilist_add(il,MAKE_OPK(OP_CALL,ci));
    }
    else if (streq(op,"ret"))  ilist_add(il,MAKE_OP(OP_RET));
    else if (streq(op,"top") || streq(op,".")) ilist_add(il,MAKE_OP(OP_TOP));
    else{
        int i = 0;
        if (isdigit(op[i])){
            sblval_t n = sblnum(atof(op));
            int ci = const_find(ct,n);
            if (ci < 0) ci = const_add(ct, n);
            ilist_add(il, MAKE_OPK(OP_PUSH,ci));
        }else{
            int l = label_find(ll,op);
            if (l < 0) return -2;
            sblval_t jmp = sblint(l - il->size);
            int ci = const_find(ct,jmp);
            if (ci < 0) ci = const_add(ct,jmp);
            ilist_add(il,MAKE_OPK(OP_CALL,ci));
        }
    }
    return 0;
}

int sblc_patch(patch_t p, iolist_t* iol,constabl_t* ct, lablist_t* ll){
    int l = label_find(ll,p.label);
    if (l < 0){
        fprintf(stderr,"%d:%d: call/jump to undeclared label '%s'\n",p.line,p.column,p.label);
        exit(1);
    }
    sblval_t off = sblint(l - p.inst_addr);
    int ci = const_find(ct,off);
    if (ci < 0) ci = const_add(ct,off);
    iol->data[p.inst_addr].arg.k = ci;

    return 0;
}

int sblc_compile_op(token_t cur, lexer_t* lex, iolist_t* iol, constabl_t* ct, lablist_t* ll, patchtable_t* pt){
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
                if (next.kind == TOK_NUMLIT){
                    sblval_t off = sblint(next.number);
                    int ci = const_find(ct,off);
                    if (ci < 0) ci = const_add(ct, off);
                    iolist_add(iol,iobj_opk(cur.op,ci));
                }
                else if(next.kind == TOK_IDENT){
                    int l = label_find(ll,buf);
                    if(l < 0){
                        patch_add(pt,patch(buf,iol->size,cur.line,cur.column));
                        iolist_add(iol,iobj_opk(cur.op,INT32_MAX));
                    }
                    else{
                        sblval_t off = sblint(l - iol->size);
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

int slbc_compile_token(token_t cur, lexer_t* lex, iolist_t* iol, constabl_t* ct, lablist_t* ll, patchtable_t* pt){
    char    buf[128];
    switch(cur.kind){
    case TOK_NONE:
        {
            iolist_add(iol,iobj_op(OP_NONE));
            return 1;
        }
    case TOK_NUMLIT:
        {
            sblval_t n = sblnum(cur.number);
            int ci = const_find(ct,n);
            if (ci < 0) ci = const_add(ct,n);
            iolist_add(iol,iobj_opk(OP_PUSH,ci));
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
            int l = label_find(ll,buf);

            if(l < 0 ){
                patch_add(pt,patch(buf,iol->size,cur.line,cur.column));
                iolist_add(iol,iobj_opk(OP_CALL,INT32_MAX));
            }
            else{
                sblval_t off = sblint(l - iol->size);
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
            sblc_compile_op(cur,lex,iol,ct,ll,pt);
            break;
        }
    }
    return 0;
}

void sblc_emit(FILE* fp, ilist_t* il, constabl_t* ct, lablist_t* ll){
    // header
    sblbinh_t bi = sbl_make_info(il,ct,ll);
    fwrite(&bi,sizeof(bi),1,fp);

    // instructions
    fwrite(il->data,sizeof(*il->data),il->size,fp);
    // const table
    fwrite(ct->data,sizeof(*ct->data),ct->size,fp);
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
