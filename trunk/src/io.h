#ifndef _SILENT_IO_H_
#define _SILENT_IO_H_	1

#include "os.h"

#if  defined(_WIN32)
#include <winsock2.h>


#elif defined(__linux__)	//linux
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

	handle_t io_create_handle(int flag);

	handle_t io_attach_listener_socket(handle_t h, socket_t listener);
	int io_attach_socket(handle_t h, socket_t sock);
	handle_t io_remove_socket(handle_t h, int index);
	handle_t io_destroy_handle(handle_t h);


}	// namespace silent
#endif // _SILENT_IO_H_

