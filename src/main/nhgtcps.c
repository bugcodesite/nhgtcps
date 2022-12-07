#include <stdlib.h>
#include <stdio.h>
#include "../inc/config.h"

int main(int argc,char**argv){
    lmm_start();
    printf("%s\n",ver);
    test();
    void *p=(void *)0;
    for(int i=0;i<100;i++){
        p=lmm_alloc(1024);
        printf("%d\n",i);
    }
    lmm_end();
}