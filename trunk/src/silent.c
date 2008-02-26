#include <stdio.h>
#include <config.h>

#include "mempool.h"




int main() {

	mempool_t *p = mempool_create();

	puts ("This is " PACKAGE_STRING ".");
	return 0;
}
