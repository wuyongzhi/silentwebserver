#ifndef _SILENT_IO_REACTOR_H_
#define _SILENT_IO_REACTOR_H_

#include "os.h"

namespace silent {

enum IOAdapterPlatformType {
	WIN32_ADAPTER = 1,
	LINUX_EPOLL_ADAPTER = 2
}

template<class _ConnectionType>
class IOReactorAdapter {
public:
	typedef _ConnectionType ConnectionType;
	typedef vector<ConnectionType> Connections;
	

public:
	bool init(int flag, int concurrent);
	bool addSocket(socket_t s);
	
};


}	// namespace silent 

#endif	// _SILENT_IO_REACTOR_H_