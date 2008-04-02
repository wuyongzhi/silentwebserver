#ifndef _SILENT_ACCEPTOR_H_
#define _SILENT_ACCEPTOR_H_

/**
	cat /proc/sys/fs/file-max

*/


#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <cerrno>
#include <cassert>
#include <string.h>
#include <vector>
#include <iostream>

#include <config.h>

#include "socket_control.h"
#include "handler.h"
#include "os.h"
#include "io.h"

using namespace std;

namespace silent {

template<class _ConnectionType, class _HandlerType=Handler<_ConnectionType> >
class Acceptor {
public:
	typedef _ConnectionType		ConnectionType;
	typedef _ConnectionType*	ConnectionPointer;
	typedef _HandlerType	HandlerType;

	typedef std::vector<ConnectionType*> Connections;

public:
	Acceptor(HandlerType& _handler)
		: handler(_handler),reactor(INVALID_HANDLE),listener(INVALID_SOCKET),
		  concurrentConnections(50000),backlog(128),
		  lastActivelyConnections(0),epollEvents(128) {
		
	}

	~Acceptor() {
		destroy();
	}

	// both are host byte order.


	int start();
	bool bind(uint32_t address, uint16_t port);
	
	
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

	int wait(Connections& opened,
			 Connections& received,
			 Connections& sent,
			 Connections& closed,
			 int timeout);



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
		int err;
		listener = ::socket(AF_INET, SOCK_STREAM, 0);
		if (listener < 0) {
			puts ("socket create failed");
			return false;
		}

		sockaddr_in sock_addr = {0};
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_port = htons(port);
		sock_addr.sin_addr.s_addr = htonl(address);

		if ( err=::bind(listener, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr)) < 0)  {
			printf ("Can NOT bind port %d \n", port);
			return false;
		}

		if ( err=::listen(listener, backlog) < 0) {
			printf ("Can NOT listen socket. backlog=%d \n", backlog);
			return false;
		}

		err = set_nonblocking(listener);
		if (err < 0) {
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
	bool initSocket(socket_t,ConnectionType*);
	bool createReactor();
	bool addListenerToReactor();
	void growEpollEventSize();
	void processClient(ConnectionType *c);


private:

	HandlerType& handler;

	handle_t reactor;
	socket_t listener;
	int backlog;
	int concurrentConnections;

	int concurrentThreads;
	
	typedef std::vector<epoll_event> EpollEvents;
	EpollEvents epollEvents;
	int lastActivelyConnections;
};

/**
*	非内联的成员定义
*/

// both are host byte order.
template<class C, class H>
bool Acceptor<C,H>::bind(uint32_t address, uint16_t port) {
	bool retval = false;

	//已经绑定,并且reactor已创建,则认为已经启动
	if (isRunning())
		return true;

	if (bind_internal(address, port, backlog) &&
		createReactor() &&
		addListenerToReactor()) {

		return true;
	}

	closeHandles();
	return retval;
}


template<class C, class H>
int Acceptor<C,H>::start() {
	Connections opened;
	Connections received;
	Connections sent;
	Connections closed;

	do {
		if (wait(opened, received, sent, closed, -1) < 0) {
			cout << "wait failed. errno=" << (strerror(errno)) << endl;
			return errno;
		}
		opened.clear(); received.clear(); sent.clear(); closed.clear();
	} while (true);

	return 0;
}


template<class C, class H>
void Acceptor<C,H>::growEpollEventSize() {
	if (epollEvents.capacity() < concurrentConnections) {
		EpollEvents::size_type old_capacity = epollEvents.capacity();
		EpollEvents::size_type new_capacity = old_capacity != 0 ? 2 * old_capacity : 1;
		if (new_capacity <= old_capacity)
			new_capacity = epollEvents.max_size();
		if (new_capacity > concurrentConnections)
			new_capacity = concurrentConnections;
	}
}




}	// namespace silent;

#if defined(__linux__)
	#include "acceptor_linux.h"
#elif defined(_WIN32)
	#include "acceptor_win32.h"
#endif


#endif // _SILENT_ACCEPTOR_H_
