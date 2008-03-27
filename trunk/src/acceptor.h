#ifndef _XF_ACCEPTOR_H_
#define _XF_ACCEPTOR_H_


#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#include <config.h>



#include "socket_control.h"
#include "handler.h"


namespace xf {

template<class _ConnectionType, class _HandlerType=Handler<_ConnectionType> >
class Acceptor {
	typedef  _ConnectionType ConnectionType;
	typedef  _HandlerType HandlerType;

private:
	HandlerType handler;
	
public:
	Acceptor(_HandlerType _handler): handler(_handler) {
	}
	
	/**	
	*	address, port both are host byte order.
	*	
	*/
	bool bind(uint32_t address, uint16_t port, int backlog=128) {
		int error;
		int listener = ::socket(AF_INET, SOCK_STREAM, 0);
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
	
	HandlerType setHandler(HandlerType handler) {
		this->handler = handler;
	}
	
/*	int start() {

	}

	
	bool suspend() {
	}

	bool resume() {

	}

	bool stop() {

	}
*/

protected:

	void mainloop() {

	}
};


}	// namespace xf;

#endif


