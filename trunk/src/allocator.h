#ifndef _SILENT_ALLOCATOR_H_
#define _SILENT_ALLOCATOR_H_

#include <cstddef>

namespace silent {
	
template <std::size_t block_size, std::size_t chunk_size>
class FixedAllocator {
public:
	typedef std::size_t size_type;
	size_type getBlockSize() const {
		return block_size;
	}

	size_type getChunkSize() const {
		return chunk_size;
	}
	
	bool canAllocateOrdered() const {
		return false;
	}
	

};


}

#endif
