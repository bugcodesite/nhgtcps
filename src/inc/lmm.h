
#define lmm_block_size  10240
#ifndef __LMM_H
#define __LMM_H
int lmm_end();
void * lmm_alloc();
int lmm_free(void *p);



/**
    alloc memory block
*/
extern void * lmm_get(int size);
/*
    free memory
    if(p==NULL){
        //really free
    }else{
        //mark p
        //can reuse p
    }
*/
extern void lmm_rm(void *p);
#endif