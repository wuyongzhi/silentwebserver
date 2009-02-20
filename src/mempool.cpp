#include "stdafx.h"
#include "mempool.h"


mempool*  mempool_create() {
	
	mempool* p = (mempool*)malloc(sizeof(mempool));
	return p;
}



void* mempool_allocate(mempool* pool, size_t size) {
	return malloc(size);
}



void mempool_deallocate(mempool* pool, void* mem) {
	free(mem);
}


void mempool_destroy(mempool* pool) {
	free(pool);
}


