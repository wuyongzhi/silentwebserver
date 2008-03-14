#include "mempool.h"


mempool_t*  mempool_create() {
	
	mempool_t* p = (mempool*)malloc(sizeof(mempool));
	return p;
}



void* mempool_allocate(mempool_t* pool, size_t size) {
	return malloc(size);
}



void mempool_deallocate(mempool_t* pool, void* mem) {
	return free(size);
}


void mempool_destroy(mempool_t* pool) {
	free(pool);
}


