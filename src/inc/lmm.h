
#define lmm_block_size  10240

int lmm_start();
int lmm_end();
void * lmm_alloc();
void lmm_free(void *p);