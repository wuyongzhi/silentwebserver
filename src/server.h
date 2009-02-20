#ifndef _SILENT_SERVER_H_
#define _SILENT_SERVER_H_

#if defined(__linux__)
#include <sys/socket.h>
#include <netinet/in.h>
#endif




/**
 * 
 *
 */
struct server {
	
	int listener_socket;
	int epoll;
	
};

#endif

