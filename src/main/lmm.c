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


//////////////////////////////////////////////////
typedef struct __lmm_item{
	struct __lmm_item *next;
	int pos;
	int size;
} LMM_ITEM,*PLMM_ITEM;
typedef struct __lmm_store{
	struct __lmm_store *next;
	PLMM_ITEM item;
} LMM_S,*PLMM_S;
static PLMM_S lmm_store_cache=NULL;
#define __lmm_block_size	1024
									//102400
#define __lmm_block_data_size	__lmm_block_size-sizeof(LMM_S)
//////////////////////////////////////////////////
void * lmm_get_(PLMM_S p,int size){
	if(NULL==p){
		return NULL;
	}
	void *p0=(void *)p+sizeof(LMM_S);
	PLMM_ITEM i=p->item;
	int o=0;
	int allocblocksize=sizeof(LMM_ITEM)+size;
	if(__lmm_block_data_size<allocblocksize){
		return NULL;
	}
	if(i!=NULL){
		o=sizeof(LMM_ITEM)+i->size;
		if(o+allocblocksize<__lmm_block_data_size){
			//append
			i=(PLMM_ITEM)(((void *)i)+i->size+sizeof(LMM_ITEM));
			if(((void *)i)>=(p0+__lmm_block_data_size-allocblocksize)){
				return NULL;
			}
			i->next=p->item;
			i->size=size;
			p->item=i;
			return ((void *)i)+sizeof(LMM_ITEM);
		}
		while(NULL!=i->next){
			//insert
			p0=((void *)i)+sizeof(LMM_ITEM)+i->size;
			if(p0+allocblocksize<((void *)i->next)){
				PLMM_ITEM p1=(PLMM_ITEM)p0;
				p1->next=i->next;
				p1->size=size;
				i->next=p1;
				return ((void *)p1)+sizeof(LMM_ITEM);
			}
		}
	}else{
		//create first
		i=(PLMM_ITEM)(p0+o);
		i->next=p->item;
		i->size=size;
		p->item=i;
		return ((void *)i)+sizeof(LMM_ITEM);
	}
	return NULL;
}
void * lmm_get(int size){
	PLMM_S p=lmm_store_cache;
	void *r;
	if(NULL==p){
		p=lmm_store_cache=(PLMM_S)malloc(__lmm_block_size);
		if(NULL==p){
			return NULL;
		}
		p->item=NULL;
		p->next=NULL;
		return lmm_get_(p,size);
	}else{
		while(NULL!=p){
			if((r=lmm_get_(p,size))!=NULL){
				return r;
			}else if(NULL==p->next){
				p->next=(PLMM_S)malloc(__lmm_block_size);
				if(NULL==p->next){
					return NULL;
				}
				p=p->next;
				p->next=NULL;
				p->item=NULL;
				return lmm_get_(p,size);
			}
			p=p->next;
		}
	}
	return NULL;
}

void lmm_rm_(PLMM_S p,void *d){
	if(NULL==p){
		return NULL;
	}
	void *p0=(void *)p+sizeof(LMM_S);
	PLMM_ITEM i=p->item;
	int o=0;

}
void lmm_rm(void *d){
	PLMM_S p=lmm_store_cache;
	if(NULL==p){
		return;
	}else if(NULL==d){
		while(NULL!=p){
			void *p0=p;
			p=p->next;
			free(p0);
		}
	}else{
		while(NULL!=p){
			if(p<d&&d<(((void *)p)+__lmm_block_data_size)){
				lmm_rm_(p,d);
				return;
			}
		}
	}
	return NULL;
}