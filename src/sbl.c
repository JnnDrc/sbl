// SBL Virtual Machine
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "sbldef.h"
#include "sblop.h"
#include "sblinst.h"
#include "sblvm.h"

#define F_DEBUG   1<<0
#define F_EXCTVER 1<<1

int main(int argc, char* argv[]){
    uint8_t flags = 0;
    if (argc < 2){
        fprintf(stderr,"USAGE: %s input [flags]", argv[0]);
        return -1;
    }
    if (streq(argv[1],"--version")){
        fprintf(stdout,"SBL %d.%d\n",SBL_MAJOR,SBL_MINOR);
        return 0;
    }
    FILE* in = fopen(argv[1],"rb");
    for (int i = 2; i < argc; i++){
        if(streq(argv[i],"--debug")) SETF(flags,F_DEBUG);
        if(streq(argv[i],"--exact-version")) SETF(flags,F_EXCTVER);
    }
    
    sblvm_t vm = {0};
    int r = sblvm_load(&vm,in);
    if (r == 1){
        fprintf(stderr,"ERROR: can't run file, not .bl");
        return -1;
    }
    uint8_t binmj = SBLVER_MAJOR(vm.bin_info.version);
    uint8_t binmn = SBLVER_MINOR(vm.bin_info.version);
    if (binmj > SBL_MAJOR || binmn > SBL_MINOR){
        fprintf(stderr,"ERROR: code compiled for newer version of sbl, mat be incomaptible, %d.%d > %d.%d",binmj,binmn,SBL_MAJOR,SBL_MINOR);
        return -1;
    }
    if (binmj < SBL_MAJOR){
        fprintf(stderr,"ERROR: code compiled for older incompatible version, %d.%d < %d.%d",binmj,binmn, SBL_MAJOR,SBL_MINOR);
        return -1;
    }
    if (binmn < SBL_MINOR){
        fprintf(stderr,"ERROR: code compiled for older version, may be incompatible, %d.%d < %d.%d",binmj,binmn,SBL_MAJOR,SBL_MINOR);
        if(HASF(flags,F_EXCTVER)) return -1;
    }
    
    while(!vm.halt){
        inst_t inst = vm.insts.data[vm.ip];
        uint16_t op = DEC_OP(inst);
        if (HASF(flags,F_DEBUG)){
            printf("ip: %zu | op: %s \n",vm.ip,sblo_op_string(op));
            printf("data:   ");
            stk_trace(&vm.data);
            printf("return: ");
            stk_trace(&vm.call);
            fflush(stdout);
            switch(getc(stdin)){
                case 'q':
                    vm.halt = true;
                    continue;
                case 'n':
                case 's':
                case ' ':
                    break;
            }
        }

        r = sblvm_exec(&vm);

        switch (r){
            case 0: break;
            case 1: 
                fprintf(stderr,"ERROR: unknown opcode %d at %zu", op, vm.ip);
                return -1;
            default:
                fprintf(stderr,"ERROR: unknown error");
                return -1;
        }
        fflush(stdout);
    }

    return 0;
}
