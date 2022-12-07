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
typedef struct __lmmp{
    int s;
} LMMP,*PLMMP;
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
    PLMMB t=c;
    while (t!=(void *)0)
    {
        free(t);
        t=t->n;
    }
    c=(void *)0;
}
void *lmm_alloc(int size){
    void *r;
    if(size>=lmm_block_size){
        return (void *)0;
    }
    PLMMB t=c;
    int rs=(sizeof(LMMP)+size);
    while(t!=(void *)0){
        if(t->s>rs){
            r=t->d+t->s-rs;
            t->s-=rs;
            ((PLMMP)r)->s=size;
            return r;
        }else if(t->e+rs<lmm_block_size){
            r=t->d+t->s-rs;
            t->e+=rs;
            ((PLMMP)r)->s=size;
            return r;
        }else if(t->n==(void *)0){
            t=(PLMMB)malloc(sizeof(LMMB)+lmm_block_size);
            if(t==(void *)0){
                return (void *)0;
            }
            t->n=c;
            t->s=0;
            t->e=rs;
            t->o=0;
            t->n=c;
            t->d=t+sizeof(LMMB);
            c=t;
            return r;
        }else{
            t=t->n;
        }
    }
    return (void *)0;
}
void lmm_free(void *p){
    
    
}