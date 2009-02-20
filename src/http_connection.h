#ifndef _SILENT_HTTP_CONNECTION_H_
#define _SILENT_HTTP_CONNECTION_H_	1

#include <cstddef>

#include "connection.h"
#include "allocator.h"

namespace silent {

class HttpConnection : public Connection {
public:
	HttpConnection() {}
	~HttpConnection() {}
	
public:


	/** 
		�Զ���� new operator 
		����һ������������. 
	*/
public:
	template<typename AllocatorType>
	void* operator new (size_t size, AllocatorType& alloc) {
		return alloc.allocate(size);
	}

	template<typename AllocatorType>
	void operator delete(void* p, AllocatorType& alloc) {
		alloc.deallocate(p);
	}
};


}

#endif
