#ifndef _SILENT_CONNECTION_H_
#define _SILENT_CONNECTION_H_

#include "os.h"


namespace silent {


/**
 *	Connection ��
 *
 */
class Connection {

private: 
	socket_t sock_fd;			//sock ������
	
protected:
	
	Connection() {
	}
	~Connection() {
		closeSocket(sock_fd);
	}
	
template<class,class> friend class Acceptor;	//acceptor �ɷ��� connection �����г�Ա
	
};


} // namespace silent;


#endif	// _SILENT_CONNECTION_H_

