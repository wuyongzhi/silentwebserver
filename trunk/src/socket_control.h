#ifndef _SILENT_SOCK_CONTROL_H_
#define _SILENT_SOCK_CONTROL_H_

#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

#include "os.h"

#ifndef __cplusplus
extern "C" {
#endif


inline int set_nonblocking(socket_t fd) {
	int currentFlag = fcntl(fd, F_GETFL);
	currentFlag |= O_NONBLOCK;
	return fcntl (fd, F_SETFL, currentFlag);
}


inline int get_sendbuffer_size(socket_t fd, int &size) {
	socklen_t socklen=sizeof(size);
	return getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size, &socklen );
}



inline int get_recvbuffer_size(socket_t fd, int &size) {
	socklen_t socklen=sizeof(size);
	return getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &size, &socklen);
}



inline int set_sendbuffer_size(socket_t sockfd, int size) {
	return setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
}


inline int set_recvbuffer_size(socket_t sockfd, int size) {
	return setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
}



/* 禁用 Nagle 算法*/
inline int set_nodelay(socket_t s) {
	int value=1;
	return setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
}

inline int set_delay(socket_t s) {
	int value=0;
	return setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
}



#ifndef __cplusplus
}
#endif



#endif



