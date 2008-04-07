#ifndef _SILENT_BUFFER_H_
#define _SILENT_BUFFER_H_	1


#include <list>
#include <vector>


namespace silent {

/**
	
*/

class Buffer {
public:
	typedef std::size_t size_type;

public:
	Buffer(char* _buf, size_type _size) :buf(_buf),size(_size) {
	}
	
	char* buffer() {
		return buf;
	}
	
	const char* buffer() const {
		return buf;
	}

	size_type size() const {
		return size;
	}

private:
	size_type	size;	
	char*		buf;

private:
	Buffer() {}
};



class Package {

public:
	typedef std::vector<Buffer*> Buffers;
	typedef Buffer::size_type size_type;
	

public:
	public 
	size_type size() const {
		return size;
	}

	size_type capacity() const {
		return capacity;
	}

	Buffers& buffers() {
		return bufs;
	}

private:
	size_type capacity;		/* 此包的容量 */
	size_type size;			/* 描述数据包的长度．这个长度不是容量的长度．而是实际数据的长度 */
	Buffers  bufs;

};



}



#endif