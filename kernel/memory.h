#include "common.h"
#include "screen.h"

#define MEMORY_START 0x300000;

#ifndef MEMORY_FILE_DEFINED
	#define MEMORY_FILE_DEFINED 1
	void* allocMemory2(MemorySize);
	void freeMemory(void*, MemorySize);
	void initMemory(void);

	#define allocMemoryType(type) (type*)allocMemory2(sizeof(type))
	#define allocMemoryTypeSize(type, size) (type*)allocMemory2(sizeof(type) * (size))
	typedef struct FreeMemoryList{
		struct FreeMemoryList* next;
		struct FreeMemoryList* before;
		MemorySize size;
	} FreeMemoryList;
	
//	void putMemoryInfo(ImageSheet*);
#endif