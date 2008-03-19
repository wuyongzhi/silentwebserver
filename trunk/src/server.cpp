#include <stdio.h>
#include <config.h>

#include "mempool.h"
#include "server.h"
#include "server_config.h"


int main() {
	
	puts ("This is " PACKAGE_STRING );
	return 0;
}



int create_listener(server_config& config) {
	int error;
	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		puts ("socket create failed");
		return listener;
	}
	
	sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(config.port);
	addr.sin_addr.s_addr = htonl(config.address);


	if ( error=bind(listener, (sockaddr*)&addr, sizeof(addr)) < 0)  {
		printf ("Can NOT bind port %d \n", config.port);
		return error;
	}
	
	if ( error=listen(listener, config.backlog) < 0) {
		printf ("Can NOT listen socket. backlog=%d \n", config.backlog);
		return error;
	}
	
	
	return listener;
}

int config_listen_socket(int sockfd) {
	
}


