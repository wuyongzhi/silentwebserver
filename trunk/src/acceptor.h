#ifndef _SILENT_ACCEPTOR_H_
#define _SILENT_ACCEPTOR_H_


#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <cassert>
#include <vector>

#include <config.h>

#include "socket_control.h"
#include "handler.h"
#include "os.h"
#include "io.h"

using namespace std;

namespace silent {

template<class _ConnectionType, class _HandlerType=Handler<_ConnectionType> >
class Acceptor {
	typedef _ConnectionType	ConnectionType;
	typedef _HandlerType	HandlerType;

	typedef std::vector<ConnectionType*> Connections;

public:
	Acceptor(HandlerType& _handler)
		: handler(_handler),reactor(INVALID_HANDLE),listener(INVALID_SOCKET),
			concurrentConnections(50000),backlog(128) {
		epollEvents.reserve (128);

	}

	~Acceptor() {
		destroy();
	}

	// both are host byte order.
	bool start(uint32_t address, uint16_t port);

	int wait(Connections& opened,
			 Connections& received,
			 Connections& sent,
			 Connections& closed,
			 int timeout);

	HandlerType setHandler(HandlerType& handler) {
		this->handler = handler;
	}

	int getConcurrentConnections() {
		return this->concurrentConnections;
	}

	bool setConcurrentConnections(int size) {
		if (!isValidHandle(reactor)) {
			if (size > 0) {
				this->concurrentConnections = size;
				return true;
			}
		}
		return false;
	}

	int getConcurrentThreads() {
		return this->concurrentConnections;
	}

	bool setConcurrentThreads(int size) {
		if (!isValidHandle(reactor)) {
			if (size > 0) {
				this->concurrentConnections = size;
				return true;
			}
		}
		return false;
	}



protected:

	void mainloop() {

	}

private:

	bool isRunning() {
		return isBinding() && isReactorCreated();
	}

	bool isBinding() {
		return isValidSocket(listener);
	}

	bool isReactorCreated() {
		return isValidHandle(reactor);
	}

	void closeHandles () {
		if (isValidSocket(listener)) {
			closeHandle(listener);listener = INVALID_SOCKET;
		}
		if (isValidHandle(reactor)) {
			closeHandle(reactor); reactor = INVALID_HANDLE;
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
			puts ("Can NOT set listening socket to nonblocking");
			return false;
		}

		return true;
	}




	void destroy() {
		closeHandles();
	}


	/**
		以下成员实现在别的文件, 不同平台是不一样的.
		因此放在后面包含进来
	*/
	bool createReactor();
	bool addListenerToReactor();
	void growEpollEventSize();

private:
	HandlerType& handler;

	handle_t reactor;
	socket_t listener;
	int backlog;
	int concurrentConnections;
	int concurrentThreads;

	typedef std::vector<epoll_event> EpollEvents;

	EpollEvents epollEvents;
};


}	// namespace silent;

#if defined(__linux__)
	#include "acceptor_linux.h"
#elif defined(_WIN32)
	#include "acceptor_win32.h"
#endif


#endif // _SILENT_ACCEPTOR_H_
