#ifndef _XF_OS_H_
#define _XF_OS_H_	1

namespace xf {
#ifdef WIN32 

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


#else
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

	const int INVALID_SOCKET = -1;
	const int INVALID_HANDLE = -1;

#endif
	



}



#endif
