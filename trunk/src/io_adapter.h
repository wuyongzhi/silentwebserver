#ifndef _SILENT_IO_H_
#define _SILENT_IO_H_	1

#include "os.h"

#if  defined(_WIN32)
#include <winsock2.h>


#elif defined(__linux__)	// linux
	#if defined(HAVE_SYS_EPOLL_H)
		#include <sys/epoll.h>
		#include <poll.h>
		
		#ifndef EPOLLRDHUP
		#define EPOLLRDHUP POLLRDHUP
		#endif
	#endif

#elif
	#error "Can NOT support your OS!"
#endif




namespace silent {

enum IOAdapterPlatformType {
	WIN32_ADAPTER = 1,
	LINUX_EPOLL_ADAPTER = 2
}

template<class _HandlerType, IOAdapterPlatformType platformType>
class IOReactorAdapter {
	bool init(int flag, int concurrent);
	bool 
};


}	// namespace silent 





#endif // _SILENT_IO_H_
