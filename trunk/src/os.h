#ifndef _SILENT_OS_H_
#define _SILENT_OS_H_	1

namespace silent {

#if defined(_WIN32)		/* win32 */

	#include <windows.h>
	#include <winsock2.h>

	typedef HANDLE handle_t;
	typedef SOCKET socket_t;

	inline bool isValidHandle(handle_t x) {
		return (x!=INVALID_HANDLE);
	}
	inline bool isInvalidHandle(handle_t x) {
		return (x==INVALID_HANDLE);
	}
	inline bool isValidSocket(socket_t x) {
		return (x!=INVALID_SOCKET);
	}
	inline bool isInvalidSocket(socket_t x) {
		return (x==INVALID_SOCKET);
	}
	
	inline bool closeHandle(handle_t h) {
		return ::CloseHandle(h) != FALSE;
	}

#elif defined(__linux__)	/* linux */

	#include <unistd.h>

	const int INVALID_SOCKET = -1;
	const int INVALID_HANDLE = -1;
	
	typedef int handle_t;
	typedef int socket_t;

	inline bool isValidHandle(handle_t x) {
		return x >= 0;
	} 
	inline bool isInvalidHandle(handle_t x) {
		return x < 0;
	}
	inline bool isValidSocket(socket_t x) {
		return isValidHandle(x);
	}
	inline bool isInvalidSocket(socket_t x) {
		return isInvalidHandle(x);
	}

	inline bool closeHandle(handle_t h) {
		return close(h)==0;
	}
	inline bool closeSocket(socket_t s) {
		return close(s)==0;	
	}


#endif 



}

#endif
