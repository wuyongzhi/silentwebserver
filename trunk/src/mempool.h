#ifndef _XF_MEM_POOL_H_
#define _XF_MEM_POOL_H_

#include <stdlib.h>

typedef struct _mempool_t {
	int mark;
}mempool;


mempool* mempool_create();
void* mempool_allocate(mempool* pool, size_t size);
void mempool_deallocate(mempool* pool, void* mem);
void mempool_destroy(mempool* pool);



#endif
