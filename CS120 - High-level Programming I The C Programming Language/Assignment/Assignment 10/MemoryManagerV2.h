/******************************************************************************
filename MemoryManagerV2.h
author Howard Sin
email howard.sin@digipen.edu
date created 26 Mar 2020
Brief Description: A header file for a simple memory manager
******************************************************************************/
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

struct ArrayAllocNode
{
	/* Pointer to the allocated array */
	void* arrayPtr;
	
	/* The length of the allocation in elements */
	unsigned int length;
	
	/* The pointer to the next node in the link-list */
	struct ArrayAllocNode* next;
};

struct MemoryPool
{
	/* Pointer to the pool of allocated memory */
	char* allocatedMemory;

	/* Pointer to an array of bits denoting active/inactive */
	char* isActive;

	/* Memory size of each element */
	unsigned int unitSize;

	/* Total number of elements allowed in the pool */
	unsigned int count;

	/* Seeker index to resume search */
	unsigned int seekerIndex;

	/* NEW! Array allocation link-list to keep track of array allocations */
	struct ArrayAllocNode* arrAllocList;
};

enum MEMORYMGR_RESULT_CODES
{
	MM_OK,
	MM_FAILED
};

/* Allocates an element from the pool, returning the address. NULL if failed. */
void* MemoryMgrAllocate(struct MemoryPool* pool);

/* Deallocates an element from the pool, updating the bit vector to allow reuse. */
void MemoryMgrDeallocate(struct MemoryPool* pool, void* target);

/* Scans the target memory pool for unused elements and return that as a result */
unsigned int MemoryMgrInactiveCount(struct MemoryPool* target);

/* 
New functions to implement & old functions to update in V2 below!
*/

/*
Allocates resources for the memory pool (result) given.
The memory pool allocated will be for elements of (unitSize) bytes.
The memory pool will contain (count rounded up to a multiple of 8) number of elements in an array.
You are also to allocate the bit vector (isActive) and initialize them all to 1 (available)
NEW: You are to initalize the array allocation link-list to NULL
Returns MM_OK if allocation & initialization is successful, MM_FAILED otherwise
*/
int MemoryMgrCreatePool(struct MemoryPool* result, unsigned int unitSize, unsigned int count);

/* 
Frees all memory allocated in the target memory pool and defaults all values to 0 
NEW: You have to also free all memory used in the array allocation link-list
*/
void MemoryMgrFreePool(struct MemoryPool* target);

/* 
Allocates an array of elements from the pool, returning the address to the start of the array.
The memory issued must be contiguous and you will have to keep track of the allocation in the
pool's array allocation link-list.
The bit vector has to be updated too to ensure correct behaviour with single allocations.
*/
void* MemoryMgrArrayAllocate(struct MemoryPool* pool, unsigned int count);

/*
Deallocates an array allocation from the pool, removing the entry from the pool's array allocation
link-list and updating the bit vector to allow the memory to be reused.
*/
void MemoryMgrArrayDeallocate(struct MemoryPool* pool, void* target);

#endif /* MEMORY_MANAGER_H */