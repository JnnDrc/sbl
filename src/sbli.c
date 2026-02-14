// SBL Inspector
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "sblop.h"
#include "sblinst.h"
#include "sblconst.h"

#define cast(T, v) (T)(v)

typedef struct sbl_bin_header{
    char magic[2];  // SB
    uint16_t insts;  //
    uint32_t ctb_off;
}sblh_t;

typedef struct sbl_machine{
    // program data
    sblh_t      header;
    ilist_t     insts;
    constabl_t  consts;
}sbl_info_t;

int main(int argc, char* argv[]){
    if(argc < 2) return -1;

    FILE* in = fopen(argv[1],"rb");
    if(!in) return -2;
    
    sbl_info_t M = {0};
    fread(&M.header,sizeof(M.header),1,in);

    if(strncmp(M.header.magic,"SB",2)) return -1;

    M.insts.size = M.header.insts;
    fread(M.insts.data,sizeof(inst_t),M.header.insts,in);

    long cur = ftell(in);
    fseek(in,0L,SEEK_END);
    long end = ftell(in);

    long remain = end - cur;
    fseek(in,cur,SEEK_SET);

    M.consts.size = remain / sizeof(float);
    fread(M.consts.data,1,remain,in);

    
    printf("INFO--------------\n");
    printf("SBL Bytecode Binary\n");
    printf("%hu instructions\n",M.header.insts);
    printf("%hu constants, table at <instructions>+%d\n",M.consts.size,M.header.ctb_off);

    int width = (int)roundf(1+logf(M.insts.size)/logf(10.0f));
    printf("INSTRUCTIONS-------\n");
    // printf("i: OP   K | A B\n");
    for(int i = 0; i < M.insts.size; i++){
        uint32_t it = M.insts.data[i];
        printf("%0*d: (%03d) %s\t    %d (%hu %hu) \n",width,i,DEC_OP(it),sblo_op_string(DEC_OP(it)),cast(uint32_t,DEC_K(it)),cast(uint16_t,DEC_A(it)),cast(uint16_t,DEC_B(it)));
    }
    printf("CONSTANTS----------\n");
    // printf("i:    K\n");
    for(int i = 0; i < M.consts.size; i++){
        float k = M.consts.data[i];
        printf("%0*d:    %.02f\n",width,i, k);
    }
    printf("-------------------\n");
    fflush(stdout);

    return 0;
}
