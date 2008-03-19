#include <stdio.h>
#include <config.h>

#include "mempool.h"
#include "server.h"
#include "server_config.h"


int main() {
	
	puts ("This is " PACKAGE_STRING );
	return 0;
}



int create_listener(server_config config) {
	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) 
		return listener;
	sockaddr_in addr = {0};
	
	
	return listener;

}

