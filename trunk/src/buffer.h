#ifndef _SILENT_BUFFER_H_
#define _SILENT_BUFFER_H_	1


#include <list>


namespace silent {

class Buffer {
public:
	typedef size_t size_type;

	size_type	len;
	char *		buf;
};

class Package {
public:
	typedef std::list<Buffer*> Buffers;

	Buffers* bufs;
};



}



#endif