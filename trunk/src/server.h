#ifndef _XF_SERVER_H_
#define _XF_SERVER_H_

#include <sys/socket.h>
#include <netinet/in.h>




/**
 * 
 *
 */
struct server {
	
	int listener_socket;
	int epoll;
	
};

#endif

