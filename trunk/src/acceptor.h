#ifndef _SILENT_ACCEPTOR_H_
#define _SILENT_ACCEPTOR_H_


#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>


#include <config.h>

#include "socket_control.h"
#include "handler.h"
#include "os.h"
#include "io.h"


namespace silent {

template<class _ConnectionType, class _HandlerType=Handler<_ConnectionType> >
class Acceptor {
	typedef  _ConnectionType	ConnectionType;
	typedef  _HandlerType		HandlerType;

public:
	Acceptor(HandlerType& _handler)
		: handler(_handler),epoll_fd(INVALID_HANDLE),listener(INVALID_SOCKET),epoll_size(50000) {
	}
	
	~Acceptor() {
		destroy();
	}
	
	// both are host byte order.
	bool start(uint32_t address, uint16_t port, int backlog=128) {
		if (listener > 0)	//already bind
			return false;

		bool result = bind_internal(address, port, backlog);
		if (!result) {
			close_FDs();
		}
		return result;
	}
	

	HandlerType setHandler(HandlerType& handler) {
		this->handler = handler;
	}

	int getEpollSize() {
		return this->epoll_size;
	}

	bool setEpollSize(int size) {
		
	}

protected:

	void mainloop() {

	}

private:
	HandlerType& handler;

	handle_t epoll_fd;	
	socket_t listener;	

	int epoll_size;	// first parameter of epoll_create function

	
	bool isBinding() {
		return isValidSocket(listener);
	}
	bool isEpollCreated() {
		return isValidHandle(epoll_fd);
	}
	
	void close_FDs () {
		if (isValidSocket(listener)) {
			close(listener);listener = 0;
		}
		if (epoll_fd > 0) {
			close(epoll_fd); epoll_fd = 0;
		}
	}

	bool bind_internal(uint32_t address, uint16_t port, int backlog=128) {
		int error;
		listener = ::socket(AF_INET, SOCK_STREAM, 0);
		if (listener < 0) {
			puts ("socket create failed");
			return false;
		}

		sockaddr_in sock_addr = {0};
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_port = htons(port);
		sock_addr.sin_addr.s_addr = htonl(address);

		if ( error=::bind(listener, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr)) < 0)  {
			printf ("Can NOT bind port %d \n", port);
			return false;
		}
	
		if ( error=::listen(listener, backlog) < 0) {
			printf ("Can NOT listen socket. backlog=%d \n", backlog);
			return false;
		}

		error = set_nonblocking(listener);
		if (error < 0) {
			puts ("Can NOT configure listening socket");
			return false;
		}

		return true;
	}
	
	bool epoll_init() {
		if (isEpollCreated())
			return true;
		
		epoll_fd = epoll_create(epoll_size);
		if (isValidHandle(epoll_fd)) {
			
		}

		return true;
	}
	
	

	void destroy() {
		close_FDs();
	}



};


}	// namespace silent;

#endif


