// SBL Compiler
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sblop.h"
#include "sblconst.h"
#include "sblinst.h"

#define streq(s1,s2) !strcmp(s1,s2)

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
    char* op = strtok(line," ");
    trim(op);
    int32_t ka = atoi(strtok(NULL," "));
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
    else if (streq(op,"+rot"))  ilist_add(il,MAKE_OP(OP_LROT));
    else if (streq(op,"-rot"))  ilist_add(il,MAKE_OP(OP_RROT));
    else if (streq(op,"jump")){
        int ci = const_find(ct,ka);
        if (ci < 0) ci = const_add(ct, ka);
        ilist_add(il,MAKE_OPK(OP_JUMP,ci));
    } 
    else if (streq(op,"hop")) ilist_add(il,MAKE_OP(OP_HOP));
    else if (streq(op,"gt"))  ilist_add(il,MAKE_OP(OP_GT));
    else if (streq(op,"lt"))  ilist_add(il,MAKE_OP(OP_LT));
    else if (streq(op,"ge"))  ilist_add(il,MAKE_OP(OP_GE));
    else if (streq(op,"le"))  ilist_add(il,MAKE_OP(OP_LE));
    else if (streq(op,"eq"))  ilist_add(il,MAKE_OP(OP_EQ));
    else if (streq(op,"neq")) ilist_add(il,MAKE_OP(OP_NEQ));
    else if (streq(op,"echo")){
        int ci = const_find(ct,ka);
        if (ci < 0) ci = const_add(ct,ka);
        ilist_add(il,MAKE_OPK(OP_ECHO,ci));
    }
    else{
        return 1;
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

int main(int argc, char* argv[]){
    if(argc < 2) return -1;

    FILE* fin  = fopen(argv[1],"r");
    char* out  = argv[2] ? argv[2] : "sbl.out";
    FILE* fout = fopen(out,"wb");

    if(!fin){
        fprintf(stderr,"[ERROR]: failed to open file %s",argv[1]);
        return -1;
    }
    if(!fout){
        fprintf(stderr,"[ERROR]: can't open output file");
        return -1;
    }

    constabl_t ct = {.size = 0, .data = {0}};
    ilist_t    il = {.size = 0, .data = {0}};
    
    char line[128] = {0};
    size_t l = 0;
    while(fgets(line,sizeof(line),fin)){
        l++;
        if(strlen(line) == 0) continue;
        if(line[0] == '#') continue;
        
        int r = sblc_compile_line(line,&il,&ct);
        if (r){
            fprintf(stderr,"[ERROR]: unknown operation at line %zu",l);
            return -1;
        }
    }
    ilist_add(&il,MAKE_OP(OP_NONE));

    sblc_emit(fout,&il,&ct);

    return 0;
}
