#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sblcc.h"

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

int sblc_compile_line(char* line, ilist_t* il, constabl_t* ct, lablist_t* ll){
    trim(line);
    if(strlen(line) <= 0)   return 0;
    if(line[0] == COMMENT)  return 0;

    if(line[0] == LABEL){
        char* l = line + 1;
        if (label_find(ll,l) < 0) label_add(ll,label(l,il->size));
        return 0;
    }

    char* op = strtok(line," ");
    char*   kas = strtok(NULL," ");
    stype_t ka = atoi(kas);
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
        // k is number
        if (isdigit(kas[0])){
            int ci = const_find(ct,ka);
            if (ci < 0) ci = const_add(ct, ka);
            ilist_add(il,MAKE_OPK(OP_JUMP,ci));
        }else {
            // k is label
            int l = label_find(ll,kas);
            if (l < 0) return -2;
            int jmp = l - il->size;

            int ci = const_find(ct,jmp);
            if (ci < 0) ci = const_add(ct, jmp);
            ilist_add(il,MAKE_OPK(OP_JUMP,ci));
        }
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
        }else return -1;
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
