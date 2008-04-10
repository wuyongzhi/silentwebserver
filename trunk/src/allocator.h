#ifndef _SILENT_ALLOCATOR_H_
#define _SILENT_ALLOCATOR_H_

#include <cstddef>
#include <cstdlib>
#include <list>
#include <vector>
#include <cassert>

#include <http_connection.h>
//#include "buffer.h"

namespace silent {

/*
	非线程安全的, 固定大小分配器. 

	模板参数: block_size, 指定每次分配的大小，一个常量
			 blocks_per_chunk，告诉分配器当没有可用的块时，每次向系统申请多少块.
*/
template <std::size_t REQUEST_BLOCK_SIZE, std::size_t BLOCKS_PER_CHUNK>
class FixedAllocator {

public:

////在64位的机器上, 8应换成16
#define MY_ALIGN 8		

	typedef std::size_t size_type;
	const static size_type block_size = (((REQUEST_BLOCK_SIZE) + MY_ALIGN-1) & ~(MY_ALIGN - 1) );

	struct ChunkNode {
		ChunkNode(): 
			used(0) {
		}
		size_type	used;
		char		padding[8 - sizeof(size_type)]; 
		char		blocks[BLOCKS_PER_CHUNK][block_size];
	};

	struct FreeBlock {
		 FreeBlock *next;
	};


public:

	FixedAllocator():
		freeList(0),currentChunk(0) {
	}

	size_type getBlockSize() const {
		return block_size;
	}

	bool canAllocateOrdered() const {
		return false;
	}

	void* allocate(size_t size) {
		void* result;
		
		if (freeList) {
			result = freeList;
			freeList = freeList->next;
			return result;
		} else if (currentChunk){
			assert (currentChunk->used < BLOCKS_PER_CHUNK);
			result = currentChunk->blocks[++currentChunk->used];
			if (currentChunk->used ==BLOCKS_PER_CHUNK) {
				currentChunk = 0;
			}
			return result;
		} else {
			currentChunk = new ChunkNode;
			if (currentChunk) {
				chunks.push_back(currentChunk);
				result = currentChunk->blocks[++currentChunk->used];
				return result;
			}
		}
	}
	
	/* 将数据加入自由列表 */
	void deallocate(void* p) {
		if (p) {
			FreeBlock* theBlock = reinterpret_cast<FreeBlock*>(p);
			theBlock->next = this->freeList;
			freeList->next = theBlock;
			return;
		}
	}

	
private:
	ChunkNode* currentChunk;
	std::vector<ChunkNode*> chunks;
	FreeBlock* freeList;
};


typedef FixedAllocator<sizeof(HttpConnection), 32> HttpConnectionAllocator;


}

#endif
