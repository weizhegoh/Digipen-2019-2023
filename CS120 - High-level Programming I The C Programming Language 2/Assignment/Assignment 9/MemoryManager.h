/******************************************************************************
filename MemoryManager.h
author Howard Sin
email howard.sin@digipen.edu
date created 12 Mar 2020
Brief Description: A header file for a simple memory manager
******************************************************************************/
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

struct MemoryPool
{
	/* Pointer to the pool of allocated memory */
  /*malloc(unitSize*count)*/
	char* allocatedMemory;

	/* Pointer to an array of bits denoting active/inactive */
  /*Array size  = count(rounded to mutiple of 8)/8 */
	/*Set each element in array to 255 (11111111)*/
  /*One char is mapped to 0 element in allocatedMemory*/
  /*use bitwise operators to check the bits: <<, >>, |, &, ^ */
  char* isActive;

	/* Memory size of each element */
	unsigned int unitSize;

	/* Total number of elements allowed in the pool */
  /*count = input + input%8*/  
  /*seekerIndex - find the last place when it is not being used*/
	unsigned int count;

	/* Seeker index to resume search */
  /*increament when an element in isActive is 0*/
  /*wrap around if last index is 0?*/
	unsigned int seekerIndex;
};

enum MEMORYMGR_RESULT_CODES
{
	MM_OK,
	MM_FAILED
};

/* 
Allocates resources for the memory pool (result) given.
The memory pool allocated will be for elements of (unitSize) bytes.
The memory pool will contain (count rounded up to a multiple of 8) number of elements in an array.
You are also to allocate the bit vector (isActive) and initialize them all to 1 (available)
Returns MM_OK if allocation & initialization is successful, MM_FAILED otherwise
*/
int MemoryMgrCreatePool(struct MemoryPool* result, unsigned int unitSize, unsigned int count);

/* Allocates an element from the pool, returning the address. NULL if failed. */
void* MemoryMgrAllocate(struct MemoryPool* pool);

/* Deallocates an element from the pool, updating the bit vector to allow reuse. */
void MemoryMgrDeallocate(struct MemoryPool* pool, void* target);

/* Frees all memory allocated in the target memory pool and defaults all values to 0 */
void MemoryMgrFreePool(struct MemoryPool* target);

/* Scans the target memory pool for unused elements and return that as a result */
unsigned int MemoryMgrInactiveCount(struct MemoryPool* target);

#endif /* MEMORY_MANAGER_H */