#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <config.h>

#include "mempool.h"
#include "server.h"
#include "server_config.h"
#include "socket_control.h"


int config_listening_socket(int sockfd, server_config &config);
int mainloop (server& srv, server_config& cfg);
int create_listener(server_config& cfg);

static server_config g_config;
static server g_server;

int main() {
	
	g_config.port = 8080;
	g_config.address = 0;
	g_config.backlog = 128;
	
	g_server.listener_socket = create_listener(g_config);
	
	if (g_server.listener_socket < 0 ) {
		return 1;
	} else {
		printf ("%s is running at %d port\n", PACKAGE_STRING, g_config.port);
	}

	mainloop(g_server, g_config);

	return 0;
}



int create_listener(server_config& config) {
	int error;
	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		puts ("socket create failed");
		return listener;
	}

	error = config_listening_socket(listener, config);
	if (error < 0) {
		puts ("Can NOT configure listening socket");
		return error;
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

int config_listening_socket(int sockfd, server_config& config) {
	int result = set_nonblock(sockfd);
	return result;
}




int mainloop (server& srv, server_config& cfg) {
	close(srv.listener_socket);
	return 0;
}


