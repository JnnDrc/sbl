#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sblop.h"
#include "sblconst.h"
#include "sblinst.h"
#include "sblstack.h"
#include "sbldef.h"


static void trim(char* str){
    if(!str) return;
    size_t s = 0, e = strlen(str) - 1;
    while(isspace(str[s])){
        s++;
    }
    while(e > s && isspace(str[e])){
        e--;
    }
    if(s > 0 || e < (strlen(str) - 1)){
        memmove(str,str+s,e - s + 1);
        str[e - s + 1] = '\0';
    }
}

int sblc_compile_line(char* line, ilist_t* il, constabl_t* ct){
    trim(line);
    if(strlen(line) <= 0) return 0;
    if(line[0] == '#')    return 0;
    char* op = strtok(line," ");
    stype_t ka = atoi(strtok(NULL," "));
    int16_t b  = atoi(strtok(NULL," "));
    (void)b;

    if      (streq(op,"add")) ilist_add(il,MAKE_OP(OP_ADD));
    else if (streq(op,"sub")) ilist_add(il,MAKE_OP(OP_SUB));
    else if (streq(op,"mul")) ilist_add(il,MAKE_OP(OP_MUL));
    else if (streq(op,"div")) ilist_add(il,MAKE_OP(OP_DIV));
    else if (streq(op,"push")){
        int ci = const_find(ct,ka);
        if (ci < 0) ci = const_add(ct, ka);
        ilist_add(il, MAKE_OPK(OP_PUSH,ci));
    }
    else if (streq(op,"pop"))  ilist_add(il,MAKE_OP(OP_POP));
    else if (streq(op,"swap")) ilist_add(il,MAKE_OP(OP_SWAP));
    else if (streq(op,"dup"))  ilist_add(il,MAKE_OP(OP_DUP));
    else if (streq(op,"over")) ilist_add(il,MAKE_OP(OP_OVER));
    else if (streq(op,"rot"))  ilist_add(il,MAKE_OP(OP_LROT));
    else if (streq(op,"+rot")) ilist_add(il,MAKE_OP(OP_LROT));
    else if (streq(op,"-rot")) ilist_add(il,MAKE_OP(OP_RROT));
    else if (streq(op,"jump")){
        int ci = const_find(ct,ka);
        if (ci < 0) ci = const_add(ct, ka);
        ilist_add(il,MAKE_OPK(OP_JUMP,ci));
    } 
    else if (streq(op,"hop"))  ilist_add(il,MAKE_OP(OP_HOP));
    else if (streq(op,"gt"))   ilist_add(il,MAKE_OP(OP_GT));
    else if (streq(op,"lt"))   ilist_add(il,MAKE_OP(OP_LT));
    else if (streq(op,"ge"))   ilist_add(il,MAKE_OP(OP_GE));
    else if (streq(op,"le"))   ilist_add(il,MAKE_OP(OP_LE));
    else if (streq(op,"eq"))   ilist_add(il,MAKE_OP(OP_EQ));
    else if (streq(op,"neq"))  ilist_add(il,MAKE_OP(OP_NEQ));
    else if (streq(op,"pow"))  ilist_add(il,MAKE_OP(OP_POW));
    else if (streq(op,"sqr"))  ilist_add(il,MAKE_OP(OP_SQR));
    else if (streq(op,"sqrt")) ilist_add(il,MAKE_OP(OP_SQRT));
    else if (streq(op,"echo")){
        int ci = const_find(ct,ka);
        if (ci < 0) ci = const_add(ct,ka);
        ilist_add(il,MAKE_OPK(OP_ECHO,ci));
    }
    else if (streq(op,"print")) ilist_add(il,MAKE_OP(OP_PRINT));
    else{
        int i = 0;
        if (isdigit(op[i])){
            int n = atoi(op);
            int ci = const_find(ct,n);
            if (ci < 0) ci = const_add(ct, n);
            ilist_add(il, MAKE_OPK(OP_PUSH,ci));
        }else return 1;
    }
    return 0;
}

void sblc_emit(FILE* fp, ilist_t* il, constabl_t* ct){
    // header
    fwrite("SB",2,1,fp);                            // magic number
    fwrite(&il->size,sizeof(uint16_t),1,fp);        // amount of instructions
    uint32_t ctoff = il->size * sizeof(*il->data);  // const table offset
    fwrite(&ctoff,sizeof(ctoff),1,fp);

    // instructions
    fwrite(il->data,sizeof(*il->data),il->size,fp);
    // const table
    fwrite(ct->data,sizeof(*ct->data),ct->size,fp);
}
