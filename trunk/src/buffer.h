#ifndef _SILENT_BUFFER_H_
#define _SILENT_BUFFER_H_	1


#include <list>
#include <vector>


namespace silent {

/**
	
*/

public Buffer {
public:
	typedef size_t size_type;

	size_type	len;
	char *		buf;
};



class Package {

public:
	typedef std::vector<Buffer*> Buffers;
	typedef Buffer::size_type size_type;
	
	size_type 

	Buffers* bufs;
};



}



#endif