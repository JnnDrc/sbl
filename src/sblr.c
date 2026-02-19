// SBL REPL
#include <stdio.h>

#include "sblcc.h"
#include "sblvm.h"

int main(void){
    
    sblvm_t vm;
    stack_init(&vm.data);
    ilist_init(&vm.insts);
    constabl_init(&vm.consts);
    vm.ip = 0;
    vm.halt = false;

    lablist_t ll;
    lablist_init(&ll);

    char line[128];
    for(;;){
        printf("~ ");
        fgets(line,sizeof(line),stdin);
        
        sblc_compile_line(line,&vm.insts,&vm.consts,&ll);
        sblvm_exec(&vm);
        printf("\\-> ");
        stk_trace(&vm.data);
    }

    return 0;
}
