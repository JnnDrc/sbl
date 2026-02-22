#include <string.h>
#include <ctype.h>

#include "utils.h"


char* strgets(char* buf, size_t max, char** cur, char* end){
    if(*cur >= end || max == 0) return NULL;

    size_t remain = end - *cur;
    size_t maxcpy = max - 1;
    if(remain < maxcpy) maxcpy = remain;

    char* l = memchr(*cur,'\n',maxcpy);
    size_t llen = l ? (size_t)((l - *cur) + 1) : maxcpy;
    
    memcpy(buf,*cur,llen);
    buf[llen] = '\0';
    *cur += llen;
    
    return buf;
}

void strtrim(char* str){
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
