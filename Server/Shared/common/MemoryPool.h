#ifndef SWA_MEMORY_POOL_H_
#define SWA_MEMORY_POOL_H_

#include "BaseSingle.h"

namespace SWA
{
	typedef unsigned int muint32;

#define DEBUG_SWITCH_INIT_MEMORY	true	//debug调试内存初始化

#define MEMORY_BLOCK_MASK	((muint32)0x59EF0000)
#define MEMORY_BLOCK_INUSE	((muint32)0x80000000)
#define MEMORY_BLOCK_LENGTH_SECTION	((muint32)0x0000FFFF)

#define SET_UNUSE_BLOCK_HEAD(length)		(((muint32)length) | MEMORY_BLOCK_MASK)
#define EXCHANGE_BLOCK_INUSE(head)			(((muint32)head) |= MEMORY_BLOCK_INUSE)
#define EXCHANGE_BLOCK_UNUSE(head)			(((muint32)head) &= ~MEMORY_BLOCK_INUSE)

#define GET_BLOCK_LENGTH(head)	(((muint32)head) & MEMORY_BLOCK_LENGTH_SECTION)
#define IS_INUSE(head)		(bool)(((muint32)head) & MEMORY_BLOCK_INUSE)
#define IS_MASK_OK(head)	(MEMORY_BLOCK_MASK == (((muint32)head) & ~(MEMORY_BLOCK_INUSE|MEMORY_BLOCK_LENGTH_SECTION)))

	// 总内存为 4096*16 = 
	// 对应的内存块大小 	4096	// *1 ...
	// 内存分段数量
#define CHUNK_POOL_SIZE		16		// 16、32、48、64、80、96、112、128、144、160、176、192、208、224、240、256
	// 对应的内存块数量             // 256、240、224、208、192、176、160、144、128、112、96、80、64、48、32、16
	// 最小内存块大小
#define BLOCK_MIN_SIZE		16		// +16、+16、+16
	// 内存大小获取ChunkPool索引
#define GET_CHUNKPOOL_INDEX(length)	(muint32)( ( length + sizeof(muint32) - 1 ) / BLOCK_MIN_SIZE)

	// 内存块
	struct malloc_block
	{
		muint32			_head;	//当前块数据
		malloc_block*	_next;	//下一块空闲内存块地址[空闲的时候使用]
	};

	struct malloc_chunk
	{
		void	*_ptr;	//申请的内存单元头指针
		muint32	_chunkSize;	//当前内存单元总大小

		muint32 _blockSize;	//每个内存块大小
		unsigned int	_blockCount;	//内存块数量
		malloc_block*	_blockHead;	//头
		malloc_block*	_blockFoot;	//尾

		malloc_chunk*	_next;	//下一个内存单元指针

		// 构造和析构
		void construct();
		void deconstruct();
		// 构建内存块
		bool InitializeChunk(muint32 chunkSize = 0x1000, muint32 blockSize = 0x10);

		// 联合内存单元数据块
		// 参  数：传入内存块链表尾指针
		// 返回值：返回新的内存块连接表尾指针
		malloc_block* JoinChunk(malloc_block *&head);
	};

	class ChunkPool
	{
	public:
		ChunkPool();
		~ChunkPool();

		// 初始化内存块
		// msize chunkSize = 0x1000	//默认内存单元大小4K
		// msize blockSize = 0x10,		//默认块内存大小16字节，可用内存12字节（头head占用4字节）
		bool InitializeMallocChunk(muint32 chunkSize = 0x1000, muint32 blockSize = 0x10);

		// 申请内存块
		void *malloc();
		// 释放内存块
		void free(void *ptr);

		// 分配内存从系统Heap
		void realloc();

	private:
		muint32			m_blockSize;
		muint32			m_chunkSize;

		malloc_chunk*	m_pMallocChunk;	//内存单元Chunk列表

		malloc_block*	m_pBlockHead;	//头
		malloc_block*	m_pBlockFoot;	//尾
	};

	class IMemoryPool
	{
	public:
		virtual ~IMemoryPool() {}
		virtual void* malloc(muint32 size) = 0;
		virtual void  free(void *p, muint32 size) = 0;
	};

	// 应用可以继承该类，扩展功能
	class MemoryPool : public IMemoryPool , public BaseSingle<MemoryPool>
	{
	public:
		MemoryPool();
		~MemoryPool();

		// 初始化内存池
		bool Init();

		void* malloc(muint32 size);
		void  free(void *p, muint32 size = 0);

	private:

		ChunkPool m_pChunkPool[CHUNK_POOL_SIZE];
		// 暂时无用到
		//malloc_chunk* m_pHeapChunk;	//进程堆使用内存链表
	};

#define gMemoryPool MemoryPool::Instance()

};// namespace

#endif