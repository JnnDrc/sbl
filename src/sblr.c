// SBL REPL
#include <stdio.h>

#include "sblcc.h"
#include "sblvm.h"

int main(void){
    
    sblvm_t vm;
    stack_init(&vm.stack);
    ilist_init(&vm.insts);
    constabl_init(&vm.consts);
    vm.ip = 0;
    vm.halt = false;

    char line[128];
    for(;;){
        printf("~ ");
        fgets(line,sizeof(line),stdin);
        
        sblc_compile_line(line,&vm.insts,&vm.consts);
        sblvm_exec(&vm);
        printf("\\-> ");
        stk_trace(&vm.stack);
    }

    return 0;
}
