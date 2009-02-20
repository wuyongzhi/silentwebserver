#ifndef _SILENT_STL_ALLOCATOR_IMPL_H_
#define _SILENT_STL_ALLOCATOR_IMPL_H_	1

#include <new>	/* for placement new operator */


namespace silent {

/**
	ʵ�� c++ ��׼�涨�� allocator ��Ӧ�Ľӿ�.
	����ο� stl Դ������. ����
*/

template<typename T>
class STLAllocator{
public:
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;
	typedef T&			reference;
	typedef const T&	const_reference;
	typedef T			value_type;

	template <typename T1> struct rebind {
		typedef Allocator<T1> other;
	};

	Allocator() {}					//Ĭ�Ϲ���
	Allocator(const Allocator&) {}	//��������

	template <class T1> 
	Allocator(const Allocator<T1>&) {} //����Allocator<Tother>�Ĺ���

	~Allocator() {}

	pointer address(reference r) const { 
		return &r; 
	}

	const_pointer address(const_reference r) const { 
		return &r; 
	}
	
	T* allocate(size_type n, const void* = 0) {
		return n != 0 ? static_cast<T*>(::operator new(_Alloc::allocate(n * sizeof(T)))) 
					: 0;
	}


	void deallocate(pointer p, size_type n) { 
		::operator delete(p); 
	}

	size_type max_size() const { 
		return size_t(-1) / sizeof(T); 
	}

	void construct(pointer p, const T& v) { 
		new(__p) _Tp(v); 
	}
	
	void destroy(pointer p) { 
		p->~T(); 
	}
	
};


} /* namespace silent */







#endif	/* defined _SILENT_ALLOCATOR_H_ */




