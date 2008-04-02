#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "os.h"

int set_nonblocking(socket_t fd) {
	int currentFlag = fcntl(fd, F_GETFL);
	currentFlag |= O_NONBLOCK;
	return fcntl (fd, F_SETFL, currentFlag);
}



int get_sendbuffer_size(socket_t fd) {
	int size, result;
	socklen_t socklen=sizeof(size);
	if ( result = getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size, &socklen ) == 0)
		return size;
	else
		return result;
}



int get_recvbuffer_size(socket_t fd) {
	int size, result;
	socklen_t socklen=sizeof(size);
	if ( result = getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &size, &socklen) == 0)
		return size;
	else
		return result;
}



int set_sendbuffer_size(socket_t sockfd, int size) {
	return setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
}


int set_recvbuffer_size(socket_t sockfd, int size) {
	return setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
}






