
#include "sblvm.h"
#include <string.h>
#include <math.h>

#define abs(x)   ((x) < 0 ? (-x) : (x))
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#define sign(x)  ((x) == 0 ? 0 : (x) < 0 ? -1 : 1)

void stk_push(stack_t *s,float v){
    s->data[++s->top] = v;
}

float stk_pop(stack_t *s){
    return s->data[s->top--];
}

void stk_trace(stack_t* s){
    float max = 0;
    int width = 0;
    for(int32_t i = s->top; i >= 0; i--) max = max(abs(s->data[i]),max);
    width = (int)roundf(1+logf(max)/logf(10.0f));
    for(int i = 0; i < width + 3 + 2 + 2; i++) putchar('-');
    putchar('\n');
    for(int32_t i = s->top; i >= 0; i--) printf("| %*.02f |\n",width+3,s->data[i]);
    for(int i = 0; i < width + 3 + 2 + 2; i++) putchar('-');
    putchar('\n');
}


int sblvm_load(sblvm_t* vm, FILE* fp){
    vm->stack  =  (stack_t){.top = -1, .data = {0}};
    vm->insts  =  (ilist_t){.size = 0, .data = {0}};
    vm->consts =  (constabl_t){.size = 0, .data = {0}};

    fread(&vm->bin_info,sizeof(sblbinh_t),1,fp);
    if (strncmp(vm->bin_info.magic,"SB",2)) return SBL_ERR_NOT_SLB;

    vm->insts.size = vm->bin_info.insts;
    fread(vm->insts.data,sizeof(inst_t),vm->bin_info.insts,fp);

    long cur = ftell(fp);
    fseek(fp,0L,SEEK_END);
    long end = ftell(fp);

    long remain = end - cur;
    fseek(fp,cur,SEEK_SET);

    vm->consts.size = remain / sizeof(float);
    fread(vm->consts.data,1,remain,fp);

    return 0;
}
