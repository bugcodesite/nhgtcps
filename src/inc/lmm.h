
#define lmm_block_size  10240

int lmm_end();
void * lmm_alloc();
int lmm_free(void *p);


typedef struct __lmm_store{


} LMMS,PLMMS;
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
extern void * lmm_free(void *p);