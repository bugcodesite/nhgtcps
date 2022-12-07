#include "../inc/lmm.h"
#include "../inc/config.h"
#include <stdlib.h>
typedef struct __lmmblock{
    int o;
    int s;
    int e;
    struct __lmmblock *n;
    void *d;
} LMMB,*PLMMB;
static PLMMB c=(void *)0;
int lmm_start(){
    c=(PLMMB)malloc(sizeof(LMMB)+lmm_block_size);
    if(c==(void *)0){
        return 0;
    }
    c->s=0;
    c->e=0;
    c->o=0;
    c->n=(void *)0;
    c->d=c+sizeof(LMMB);
    return 1;
}
int lmm_end(){
    while(c!=(void *)0){
        free(c);
        c=c->n;
    }
    return 0;
}
void *lmm_alloc(){
    return (void *)0;
}
void lmm_free(void *p){
    
}