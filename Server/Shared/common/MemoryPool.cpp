#include "MemoryPool.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>

namespace SWA
{
#define mem2block(mem)	(malloc_block*)((char*)mem - sizeof(muint32))
#define block2mem(b)	(void*)((char*)b + sizeof(muint32))

	void malloc_chunk::construct()
	{
		_ptr = NULL;
		_chunkSize = 0;
		_blockSize = 0;
		_blockCount = 0;
		_blockHead = NULL;
		_blockFoot = NULL;
		_next = NULL;
	}

	void malloc_chunk::deconstruct()
	{
		::free(_ptr);
		_ptr = NULL;
		_chunkSize = 0;
		_blockSize = 0;
		_blockCount = 0;
		_blockHead = NULL;
		_blockFoot = NULL;
		_next = NULL;
	}

	bool malloc_chunk::InitializeChunk( muint32 chunkSize /*= 0x1000*/, muint32 blockSize /*= 0x10*/ )
	{
		void *ptr = ::malloc(chunkSize);
		if (NULL == ptr)
		{
			return false;
		}

		if (DEBUG_SWITCH_INIT_MEMORY)	//debug调试
		{
			memset(ptr, 0, chunkSize);
		}

		_ptr = ptr;
		_chunkSize = chunkSize;
		_blockSize = blockSize;
		_blockCount = 0;
		_blockHead = NULL;
		_blockFoot = NULL;

		// 头内存块
		malloc_block* prevBlock = NULL;
		malloc_block* currentBlock = NULL;

		muint32 haveInitSize = 0;
		// 循环构造内存块
		while (haveInitSize + _blockSize <= _chunkSize)
		{
			if (NULL == currentBlock)
			{
				// 设置头内存块指针
				currentBlock = _blockHead = (malloc_block*)ptr;
			}
			else
			{
				// 移动到下一个内存块
				currentBlock = (malloc_block*)((char*)currentBlock + _blockSize);
			}

			// 已初始化内存大小累加
			haveInitSize += _blockSize;
			++_blockCount;	//内存块数量累加

			currentBlock->_head = SET_UNUSE_BLOCK_HEAD(_blockSize);
			currentBlock->_next = NULL;

			if (NULL != prevBlock)
			{
				prevBlock->_next = currentBlock;
			}
			prevBlock = currentBlock;
		}

		// 设置尾内存块指针
		_blockFoot = currentBlock;

		return true;
	}

	malloc_block * malloc_chunk::JoinChunk( malloc_block *&head )
	{
		head = _blockHead;
		return _blockFoot;
	}

	SWA::ChunkPool::ChunkPool()
	{
		m_blockSize = 0;
		m_chunkSize = 0;
		m_pMallocChunk = NULL;
		m_pBlockHead = NULL;
		m_pBlockFoot = NULL;
	}

	SWA::ChunkPool::~ChunkPool()
	{
		m_blockSize = 0;
		m_chunkSize = 0;

		while (NULL != m_pMallocChunk)
		{
			malloc_chunk* pDelChunk = m_pMallocChunk;
			m_pMallocChunk = m_pMallocChunk->_next;

			pDelChunk->deconstruct();
			::free(pDelChunk);
			pDelChunk = NULL;	
		}

		m_pBlockHead = NULL;
		m_pBlockFoot = NULL;
	}

	bool ChunkPool::InitializeMallocChunk( muint32 chunkSize /*= 0x1000*/, muint32 blockSize /*= 0x10*/ )
	{
		m_chunkSize = chunkSize;// 总大小
		m_blockSize = blockSize;// 单个大小
		// 数量 = 总/单位大小
		return true;
	}

	void * ChunkPool::malloc( void )
	{
		if (NULL == m_pBlockHead)
		{
			// 分配一个单元的所有内存，并初始化了块与块之前的链
			realloc();
		}

		if (NULL == m_pBlockHead)
		{
			// 内存申请失败，可能是分配不出来，可能是已经达到最后一个
			return NULL;
		}

		malloc_block* pMallocBlock = m_pBlockHead;
		m_pBlockHead = m_pBlockHead->_next;

		if (NULL == m_pBlockHead)
		{
			m_pBlockFoot = NULL;
		}

		// 重置数据
		EXCHANGE_BLOCK_INUSE(pMallocBlock->_head);
		pMallocBlock->_next = NULL;

		return block2mem(pMallocBlock);
	}

	void ChunkPool::free( void *ptr )
	{
		if (NULL == ptr) return ;

		malloc_block* pDelMallocBlock = mem2block(ptr);

		if (!IS_MASK_OK(pDelMallocBlock->_head))
		{
			// 校验内存块错误
			return ;
		}

		if (m_blockSize != GET_BLOCK_LENGTH(pDelMallocBlock->_head))
		{
			// 内存块释放长度问题
			return ;
		}

		if (DEBUG_SWITCH_INIT_MEMORY)	//debug调试
		{
			memset(ptr, 0, m_blockSize - sizeof(muint32));
		}

		// 更改使用状态
		EXCHANGE_BLOCK_UNUSE(pDelMallocBlock->_head);
		pDelMallocBlock->_next = m_pBlockHead;
		m_pBlockHead = pDelMallocBlock;

		// 可以尝试释放内存单元
	}

	void ChunkPool::realloc()
	{
		// 从heap分配内存
		malloc_chunk *pNewMallocChunk = /*new malloc_chunk*/(malloc_chunk *)::malloc(sizeof(malloc_chunk));
		if (NULL == pNewMallocChunk)
		{
			return ;
		}
		pNewMallocChunk->construct();

		if (!pNewMallocChunk->InitializeChunk(m_chunkSize, m_blockSize))
		{
			pNewMallocChunk->deconstruct();
			::free(pNewMallocChunk);
			pNewMallocChunk = NULL;
			return ;
		}

		if (NULL == m_pMallocChunk)
		{
			m_pMallocChunk = pNewMallocChunk;
		}
		else
		{
			// 新创建内存单元链接到链表头部,往前堆的感觉
			pNewMallocChunk->_next = m_pMallocChunk;
			m_pMallocChunk = pNewMallocChunk;
		}

		// Join内存块链表
		if (NULL == m_pBlockHead)
		{
			m_pBlockFoot = pNewMallocChunk->JoinChunk(m_pBlockHead);
		}
		else
		{
			m_pBlockFoot = pNewMallocChunk->JoinChunk(m_pBlockFoot);
		}
	}


	MemoryPool::MemoryPool()
	{
		Init();
	}

	MemoryPool::~MemoryPool()
	{

	}

	bool MemoryPool::Init()
	{
		for (int i=0; i< CHUNK_POOL_SIZE; ++i)
		{
			m_pChunkPool[i].InitializeMallocChunk(0x1000, BLOCK_MIN_SIZE*(i+1));
		}

		return true;
	}

	void* MemoryPool::malloc( muint32 size )
	{
		muint32 index = GET_CHUNKPOOL_INDEX(size);

		if (index < CHUNK_POOL_SIZE)
		{
			return m_pChunkPool[index].malloc();
		}
		else
		{
			// 从进程堆分配内存
			// 提示内存爆掉的可能
			std::cout << "Warring:Memory is too mush,please check your promager,size:" << size << std::endl;
			return ::malloc(size);
		}
	}

	void MemoryPool::free( void *p, muint32 size )
	{
		if (NULL == p)	return ;

		malloc_block* pMallocBlock = mem2block(p);
		muint32 length = GET_BLOCK_LENGTH(pMallocBlock->_head);

		muint32 index = GET_CHUNKPOOL_INDEX(length-sizeof(muint32));

		if (index < CHUNK_POOL_SIZE)
		{
			m_pChunkPool[index].free(p);
		}
		else
		{
			// 从进程堆分配内存
			::free(p);
			p = NULL;
		}
	}

}