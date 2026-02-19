// SBL Inspector
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "sblop.h"
#include "sblinst.h"
#include "sblconst.h"
#include "sblvm.h"
#include "sbldef.h"

int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr,"USAGE: %s file.slb", argv[0]);
        return -1;
    }

    FILE* in = fopen(argv[1],"rb");
    if(!in){
        fprintf(stderr,"ERROR: can't open file %s", argv[1]);
        return -1;
    }

    sblvm_t vm;
    sblvm_load(&vm,in);

    printf("INFO-----------------------------------*\n");
    printf("SBL Bytecode Binary - Version %d.%d \n", SBLVER_MAJOR(vm.bin_info.version),SBLVER_MINOR(vm.bin_info.version));
    printf("%hu instructions\n",vm.bin_info.insts);
    printf("%hu constants, table at <instructions>+%u\n",vm.consts.size,vm.bin_info.ctb_off);
    printf("program start at %u\n",vm.bin_info.start);

    int width = (int)roundf(1+logf(vm.insts.size)/logf(10.0f));
    printf("INSTRUCTIONS---------------------------*\n");
    // printf("i: OP   K | A B\n");
    for(uint i = 0; i < vm.insts.size; i++){
        uint32_t it = vm.insts.data[i];
        printf("%0*d: (%03d) %s\t    %d (%hu %hu) \n",width,i,DEC_OP(it),sblo_op_string(DEC_OP(it)),cast(uint32_t,DEC_K(it)),cast(uint16_t,DEC_A(it)),cast(uint16_t,DEC_B(it)));
    }
    printf("CONSTANTS------------------------------*\n");
    // printf("i:    K\n");
    for(uint i = 0; i < vm.consts.size; i++){
        stkobj_t k = vm.consts.data[i];
        printf("%0*d:    %.02f | %d\n",width,i, k.as_float,k.as_int);
    }
    printf("*--------------------------------------*\n");
    fflush(stdout);

    return 0;
}
