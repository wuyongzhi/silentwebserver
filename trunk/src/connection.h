#ifndef _SILENT_CONNECTION_H_
#define _SILENT_CONNECTION_H_

#include "os.h"


namespace silent {


/**
 *	Connection 类
 *
 */
class Connection {

private: 
	socket_t sock_fd;			//sock 描述符
	
protected:
	
	Connection() {
	}
	~Connection() {
		closeSocket(sock_fd);
	}
	
template<class,class> friend class Acceptor;	//acceptor 可访问 connection 的所有成员
	
};


} // namespace silent;


#endif	// _SILENT_CONNECTION_H_

