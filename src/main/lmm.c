#include "../inc/lmm.h"
#include "../inc/config.h"
#include <stdlib.h>
#include <stdio.h>
#ifndef NULL
#define NULL	((void *)0)
#endif
typedef struct _lmm_s{
		int offset;
		int size;
} LMMS,*PLMMS;
typedef struct __lmm_b_s{
		int count;
		struct __lmm_b_s *n;
} LMMB,*PLMMB;

static PLMMB lmm_cache=NULL;

extern int lmm_end(){
    PLMMB t=lmm_cache;
	while(t!=NULL){
		PLMMB p=t;
		t=t->n;
		free(p);
	}
}
int _lmm_free_1(PLMMB t,void *p){
	int r=0;
	int c=t->count;
	void *t0=(void *)t;
	if(p<=t0||p>=(t0+lmm_block_size)){
		return 0;
	}
	PLMMS p0=(PLMMS)(t0+sizeof(LMMB));
	for(int i=0;i<c;i++){
		if(t0+p0->offset==p){
			*p0=*(p0+1);
			t->count=c-1;
			r=1;
		}else if(0!=r){
			*p0=*(p0+1);
		}
		p0++;
	}
	return r;
}
extern int lmm_free(void *p){	
	int r=0;
    PLMMB t=lmm_cache;
	while(NULL!=t){
		r=_lmm_free_1(t,p);
		if(r==0){
			t=t->n;
		}else{
			break;
		}
	}
	return r;
}
void * _lmm_alloc_1(PLMMB t,int size){
	void *r=NULL;
	int sp=sizeof(LMMB);
	int lmmss=sizeof(LMMS);
	int ep=lmm_block_size;
	int np=ep;
	int c=t->count;
	PLMMS p0=(PLMMS)(((void *)t)+sizeof(LMMB));
	PLMMS p1=p0+c;
	for(int i=0;i<c;i++){
		np=p0->offset+p0->size;
		if(sp-np>=size){
			np=sp-size;
			p0++;
			while(p1>p0){
				*(p1+1)=*p1;
				p1--;
			}
			p0->size=size;
			p0->offset=np;
			r=((void *)t)+np;
			t->count=c+1;
			return r;
		}
		sp+=lmmss;
		ep=p0->offset;
		p0++;
	}
	if(ep-sp<size){
		return NULL;
	}
	t->count=c+1;
	p0->size=size;
	p0->offset=ep-size;	
	r=((void *)t)+(ep-size);
	return r;
}
extern void * lmm_alloc(int size){
	void *r=NULL;
    PLMMB t=lmm_cache;
	while(NULL!=t){
		r=_lmm_alloc_1(t,size);
		if(NULL==r){
			t=t->n;
		}else{
			return r;
		}
	}
	if(NULL==lmm_cache){
		lmm_cache=(PLMMB)malloc(lmm_block_size);
		lmm_cache->count=0;
		lmm_cache->n=NULL;
	}else{
		t=lmm_cache;
		lmm_cache=(PLMMB)malloc(lmm_block_size);
		lmm_cache->count=0;
		lmm_cache->n=t;
	}
	return _lmm_alloc_1(lmm_cache,size);
}



typedef struct __lmm_store{


} LMM_S,PLMM_S;
void * lmm_get(int size){
	return NULL;
}

void lmm_rm(void *p){
	return p;
}