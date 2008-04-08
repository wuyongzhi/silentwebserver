#ifndef _SILENT_OBJECT_BASE_H_
#define _SILENT_OBJECT_BASE_H_


namespace silent {

template<class Alloc=
class Base {

	/*static*/ void* operator new(size_t size) {
		return ::operator new (size);
	}

	/*static*/ void operator delete(void *pmem, size_t size) {
		::operator delete(pmem);
	}

private:
		
};

}


#endif

