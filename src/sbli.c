// SBL Inspector
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

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
    for(uint i = 0; i < vm.consts.size; i++){
        sblcell_t k = vm.consts.data[i];
        switch(k.type){
            case CELL_FLOAT:
                printf("%0*d:    float  %.02f\n",width,i, k.as.f);
                break;
            case CELL_UINT:
                printf("%0*d:    uint   %zu\n",width,i, k.as.u);
                break;
            case CELL_INT:
                printf("%0*d:    int    %zd\n",width,i, k.as.i);
                break;
            case CELL_BOOL:
                printf("%0*d:    bool   %s\n",width,i, k.as.u ? "true" : "false");
                break;
            case CELL_PTR:
                printf("%0*d:    ptr    %p\n",width,i, (void*)k.as.u);
                break;
            default:
                fprintf(stderr, "ERROR: Unknown value on constants table");
                return 1;
        }
    }
    printf("STATIC---------------------------------*\n");
    for (size_t i = 0; i < vm.statmem.size; i += 16) {

            /* Print offset */
            printf("%04zx:  ", i);

            /* Print hex bytes */
            for (size_t j = 0; j < 16; j++) {
                if (i + j < vm.statmem.size)
                    printf("%02x ", vm.statmem.data[i + j]);
                else
                    printf("   ");  // padding for last row
            }

            printf(" | ");

            /* Print ASCII representation */
            for (size_t j = 0; j < 16; j++) {
                if (i + j < vm.statmem.size) {
                    unsigned char c = vm.statmem.data[i + j];
                    printf("%c", isprint(c) ? c : '.');
                }
            }

            printf("\n");
        }
    printf("*--------------------------------------*\n");
    fflush(stdout);

    return 0;
}
