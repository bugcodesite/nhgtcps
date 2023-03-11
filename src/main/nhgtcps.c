#include <stdlib.h>
#include <stdio.h>
#include "../inc/config.h"

int main(int argc,char**argv){
    printf("%s\n",ver);
    /*test();
    void *p=(void *)0;
    for(int i=0;i<12;i++){
		printf("alloc p\n");
        p=lmm_alloc(1024);
        printf("%d %d\n",i,p);
		lmm_free(p);
		printf("free p=%d ok\n",p);
    }
	for(int i=0;i<20;i++){
	    p=lmm_alloc(1024);
        printf("%d %d\n",i,p);
    }
	printf("start free\n");
    lmm_end();
	printf("endend\n");*/
    void *oldp=NULL;
    void *p=NULL;
    for(int i=0;i<13;i++){
        p=lmm_get(100);
        printf("%d  %d\n",p,p-oldp);
        oldp=p;
    }
}