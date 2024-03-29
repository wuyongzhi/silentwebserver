
#include "stdafx.h"



#if defined(__linux__)

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#include <fcntl.h>

#include <config.h>
#endif



#include "mempool.h"
#include "server.h"
#include "server_config.h"
#include "socket_control.h"
#include "acceptor.h"
#include "http_connection.h"
#include "http_handler.h"

using namespace silent;

int config_listening_socket(int sockfd, server_config &config);
int mainloop (server& srv, server_config& cfg);
int create_listener(server_config& cfg);

static server_config g_config;
static server g_server;




int main() {
	
	g_config.port = 8080;
	g_config.address = 0;
	g_config.backlog = 128;
	
	typedef Acceptor<HttpConnection, Handler<HttpConnection> > HttpAcceptor;

	Handler<HttpConnection> handler;
	HttpAcceptor acceptor(handler);

	if (acceptor.bind(g_config.address,g_config.port)) {

		printf ("%s is running at %d port\n", PACKAGE_STRING, g_config.port);

		acceptor.start();
	}
	
	

	//mainloop(g_server, g_config);

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

	error = config_listening_socket(listener, config);
	if (error < 0) {
		puts ("Can NOT configure listening socket");
		return error;
	}

	return listener;
}


int config_listening_socket(int sockfd, server_config& config) {
	int result = set_nonblocking(sockfd);
	return result;
}



int mainloop (server& srv, server_config& cfg) {
	close(srv.listener_socket);
	return 0;
}


