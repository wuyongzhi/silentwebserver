#ifndef _SILENT_MEM_POOL_H_
#define _SILENT_MEM_POOL_H_

#include <stdlib.h>

typedef struct _mempool_t {
	int mark;
}mempool_t;


mempool_t* mempool_create();
void* mempool_alloc(mempool_t* pool, size_t size);
void mempool_dealloc(mempool_t* pool, void* mem);
void mempool_destroy(mempool_t* pool);



#endif
