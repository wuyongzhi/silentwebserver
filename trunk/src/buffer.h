#ifndef _SILENT_BUFFER_H_
#define _SILENT_BUFFER_H_	1


#include <list>
#include <vector>


namespace silent {

/**
	
*/

#if defined(__linux__)

/* linux */
class Buffer : protected iovec {
public:
	typedef size_t size_type;
	typedef void*  buffer_type;
	typedef const void*  const_buffer_type;

public:
	buffer_type buffer() { return iov_base; }
	const_buffer_type buffer() const { return iov_base; }
	size_type size() const { return iov_len; }

#elif defined(_WIN32) 

/* win32 */
class Buffer : protected WSABUF {
public:
	typedef u_long		size_type;
	typedef char FAR *	buffer_type;
	typedef const char FAR * const_buffer_type;

public:
	buffer_type buffer() { return buf; }
	const_buffer_type buffer() const { return buf; }
	size_type size() const { return len; }
#endif

public:
	Buffer(Buffer& other) {
		this->size = other.size;
		this->buf = other.buf;
	}

	Buffer(char* _buf, size_type _size) :buf(_buf),size(_size)
	{}
	
	buffer_type buffer() {
		return buf;
	}
	
	const_buffer_type buffer() const {
		return buf;
	}

	size_type size() const {
		return size;
	}

private:
	Buffer() {}
};




class Package {

public:
	typedef Buffer::size_type size_type;

	typedef std::vector<Buffer> Buffers;
	//typedef Buffers::iterator iterator_type;

	class PackagePointer {

		PackagePointer(Buffers& bufs, Buffer& buf, int offset) {
			this->buf = buf;
			this->pos = pos;
		}

	public:
		size_type size() {
			return buf.size;
		}

		char* buf() {
			return buf.buf;
		}	
		
		Buffer& buffer() {
			return buf;
		}

		size_type pos() {
			return pos;
		}
		
		PackagePointer& next() {
			return *this;
		}

		void add(size_type size) {
			pos += size;
			if (pos >= buf.size) {
			}
		}
	private:

		Buffers& bufs; 
		Buffer& buf;
		char* end;
		size_type pos;	/* 从 0 开始算起 */
	};


public:

	Package() : 
		capacity(0),size(0) {
	}

	size_type size() const {
		return size;
	}

	size_type capacity() const {
		return capacity;
	}

	Buffers& buffers() {
		return bufs;
	}

	const Buffers& buffers() const {
		return bufs;
	}
	
	/**
		分配一个Buffer
	*/
	void allocate() {
		
	}
	
	
	

private:
	size_type capacity;		/* 此包的容量 */
	size_type size;			/* 描述数据包的长度．这个长度不是容量的长度．而是实际数据的长度 */
	Buffers  bufs;
};


class ReadPackage : public Package {
public:
	const unsigned char readCount() const {
		return readCount;
	}
	

private:
	unsigned char readCount;

friend template<class,class> Acceptor;
};


}



#endif