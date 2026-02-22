#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "src/sbly.c"
#include "src/sblcc.c"
#include "src/sblinst.c"
#include "src/sblconst.c"
#include "src/sblval.c"
#include "src/sblvm.c"
#include "src/utils.c"
#include "src/sblstack.c"

int main(void){
    char* src = ":dadd add dup ret # a b -- c c\n:main 3 2 dadd top";
    
    lexer_t lex = lexer(src, strlen(src));
    token_t t;

    lablist_t  ll;
    constabl_t ct;
    ilist_t    il;
    lablist_init(&ll);
    constabl_init(&ct);
    ilist_init(&il);

    
    uint32_t ic = 0;
    while((t = lex_next(&lex)).kind != TOK_NONE){
        switch(t.kind){
        case TOK_LABEL_DEF:
            char* b = malloc(t.len);
            strncpy(b,t.start+1,t.len);
            b[t.len-1] = '\0';
            label_add(&ll,label(b,ic));
            free(b);
            break;
        case TOK_OP:
            ic++;
            break;
          break;
        }
    }

    lex_rewind(&lex);

    for(int i = 0; i < ll.size; i++) printf("%s : %d\n",ll.data[i].label,ll.data[i].line);
    
    char buf[128];
    while((t = lex_next(&lex)).kind != TOK_NONE){
        strncpy(buf, t.start,t.len);
        buf[t.len] = '\0';
        switch(t.kind){
        case TOK_NONE:
            break;
        case TOK_NUMLIT:
            {
                sblval_t n = sblnum(t.number);
                int ci = const_find(&ct,n);
                if (ci < 0) ci = const_add(&ct,n);
                ilist_add(&il,MAKE_OPK(OP_PUSH,ci));
                break;
            }
        case TOK_STRLIT:
            assert(0 && "NOT IMPLEMENTED");
            break;
        case TOK_LABEL_DEF:
            continue;
        case TOK_IDENT:
            {
                int l = label_find(&ll,buf);
                sblval_t j = sblint(l - il.size);
                int ci = const_find(&ct,j);
                if (ci < 0) ci = const_add(&ct,j);
                ilist_add(&il,MAKE_OPK(OP_CALL,ci));
                break;
            }
        case TOK_OP:
            ilist_add(&il,MAKE_OP(t.op));
          break;
        }
    }
    FILE* fo = fopen("pt.out","wb");
    int width = (int)roundf(1+logf(il.size)/logf(10.0f));
    for(uint i = 0; i < il.size; i++){
        uint32_t it = il.data[i];
        printf("%0*d: (%03d) %s\t    %d (%hu %hu) \n",width,i,DEC_OP(it),sblo_op_string(DEC_OP(it)),cast(uint32_t,DEC_K(it)),cast(uint16_t,DEC_A(it)),cast(uint16_t,DEC_B(it)));
    }
    sblc_emit(fo,&il,&ct,&ll);

    return 0;
}
