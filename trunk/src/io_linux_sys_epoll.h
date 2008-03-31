#ifndef _SILENT_IO_LINUX_SYS_EPOLL_H_
#define _SILENT_IO_LINUX_SYS_EPOLL_H_	1

#if defined(__linux__)
#if defined(HAVE_SYS_EPOLL_H)

#include <sys/epoll.h>

#include "os.h"




handle_t io_engineer_create_handle(int flag){
	return INVALID_HANDLE;
}







#endif
#endif	// __linux__


#endif	// _SILENT_IO_LINUX_SYS_EPOLL_H_