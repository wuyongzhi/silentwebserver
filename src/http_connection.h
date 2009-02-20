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
		自定义的 new operator 
		附带一个分配器参数. 
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
