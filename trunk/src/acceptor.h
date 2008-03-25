#ifndef _SILENT_ACCEPTOR_H_
#define _SILENT_ACCEPTOR_H_


#include <stdint.h>
 

template<class Implement, class Handler>
class Acceptor {
private:
	Implement implement;
	Handler handler;

public:
	Acceptor() {
	}
	
	/**	
	*	address, port both are host byte order.
	*	
	*/
	bool bind(uint32_t ip_address, uint16_t port) {
		return implement.bind(ip_address, port);
	}
	
	/**
	*
	*	
	*/
	int start() {
		
	}

	
	bool suspend() {

	}

	bool resume() {

	}

	bool stop() {

	}

};




#endif


