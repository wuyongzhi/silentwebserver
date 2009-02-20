#include <ctime>
#include <iostream>
#include <cstdlib>

#if 1
/*
#define USE_DL_PREFIX 1
#include "dlmalloc.c"
#define MALLOC(x) dlmalloc(x)
#define FREE(x) dlfree(x)
*/
#include <obstack.h>
#define obstack_chunk_alloc malloc
#define obstack_chunk_free free
#define MALLOC(x) obstack_alloc(&myobstack, x)
#define FREE(x) obstack_free(&myobstack, x)
#else

#define MALLOC(x) malloc(x)
#define FREE(x) free(x)
#endif

using namespace std;

int main() {
	
	const int SIZE = 16;
	const int TOTAL = 1024 * 1;
	
	obstack myobstack;
	obstack_init(&myobstack);

	void** p = 0;
	
	p = (void**)MALLOC(sizeof(void*) * TOTAL);
	cout << "ok" << endl;
	int i,start,end;

	start = clock();
	for (i=0; i<TOTAL; ++i) {
		*(p+i) = MALLOC(SIZE + (i%32));
		cout << i << endl;
	}
	for (i=0; i<TOTAL; ++i) {
		FREE(*(p+i));
	}
	end = clock();

	cout << (((double)(end-start)) / CLOCKS_PER_SEC) << endl;

	start = clock();
	for (i=0; i<TOTAL; ++i) {
		*(p+i) = MALLOC(SIZE  + (i%32));
	}
	for (i=0; i<TOTAL; ++i) {
		FREE(*(p+i));
	}
	end = clock();

	cout << (((double)(end-start)) / CLOCKS_PER_SEC) << endl;


	return 0;
}
