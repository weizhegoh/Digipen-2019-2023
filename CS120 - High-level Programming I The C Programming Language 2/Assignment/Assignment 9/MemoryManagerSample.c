#include "MemoryManagerV2.h"
#include <stdlib.h>

/*
Allocates resources for the memory pool (result) given.
The memory pool allocated will be for elements of (unitSize) bytes.
The memory pool will contain (count rounded up to a multiple of 8) number of elements in an array.
You are also to allocate the bit vector (isActive) and initialize them all to 1 (available)
NEW: You are to initalize the array allocation link-list to NULL
Returns MM_OK if allocation & initialization is successful, MM_FAILED otherwise
*/
int MemoryMgrCreatePool(struct MemoryPool* result, unsigned int unitSize, unsigned int count)
{
	/* Variable declaration */
	unsigned int i = 0;
	unsigned int bitVecSize = 0;

	/* Invalid usage checks */
	if (result == NULL || unitSize == 0 || count == 0) 
		return MM_FAILED;

	/* Figure out the correct size rounded up to multiples of 8 */
	count += count % 8 ? (8 - (count % 8)) : 0;

	/* Assign data over to the MemoryPool target */
	result->count = count;
	result->unitSize = unitSize;
	result->arrAllocList = NULL;
	result->seekerIndex = 0;

	/* Allocate the memory pool itself */
	result->allocatedMemory = (char*)malloc(unitSize * count);
	if (result->allocatedMemory == NULL)
	{
		result->count = 0;
		result->unitSize = 0;
		return MM_FAILED;
	}

	/* Allocate the bit vector for book keeping */
	bitVecSize = count / 8;
	result->isActive = (char*)malloc(bitVecSize);
	if (result->isActive == NULL)
	{
		result->count = 0;
		result->unitSize = 0;
		free(result->allocatedMemory);
		result->allocatedMemory = NULL; /* NULL the pointer so that it won't be "dangling" to our freed memory */
		return MM_FAILED;
	}

	/* Initialize the bit vector to all 1 */
	for (i = 0; i < bitVecSize; ++i)
	{
		result->isActive[i] |= 0xFF;
	}

	return MM_OK;
}

/* Allocates an element from the pool, returning the address. NULL if failed. */
void* MemoryMgrAllocate(struct MemoryPool* pool)
{
	/* Variable declaration */
	unsigned int bitIndex = 0;
	unsigned int blockIndex = 0;
	unsigned int bitVecSize = 0;

	/* Invalid usage checks */
	if (pool == NULL)
		return NULL;

	/* Find a valid slot via the bit vector */
	bitVecSize = pool->count / 8;
	for (blockIndex = 0; blockIndex < bitVecSize; ++blockIndex)
	{
		/* Increment blockIndex until we found a slot that has available bits */
		if (pool->isActive[blockIndex] != 0)
			break;
	}

	/* i is now pointing to the block with available elements UNLESS i == bitVecSize */
	if (blockIndex == bitVecSize)
		return NULL;

	/* We need to find the actual slot inside the bit vector */
	for (bitIndex = 0; bitIndex < 8; ++bitIndex)
	{
		/* Increment bitIndex until we found an available slot */
		if (pool->isActive[blockIndex] & (1 << bitIndex))
			break;
	}

	/* We have found the bit index where we have a free slot, update the bit vector */
	pool->isActive[blockIndex] &= ~(1 << bitIndex);

	/* blockIndex increments element by 8s */
	/* bitIndex increments element by 1s */
	/* Element number need to multiply by unit size to get address offset in bytes */
	return (void*)(pool->allocatedMemory + ((blockIndex * 8 + bitIndex) * pool->unitSize));
}

/* Deallocates an element from the pool, updating the bit vector to allow reuse. */
void MemoryMgrDeallocate(struct MemoryPool* pool, void* target)
{
	/* NOTE: You do not need to call free in this function!!! */
}

/* Frees all memory allocated in the target memory pool and defaults all values to 0 */
void MemoryMgrFreePool(struct MemoryPool* target)
{
	/* Invalid usage check */
	if (target == NULL) 
		return;

	/* Free the memory allocated for both the pool and the bit vector */
	if (target->isActive != NULL)
	{
		free(target->isActive);
		target->isActive = NULL;
	}

	if (target->allocatedMemory != NULL)
	{
		free(target->allocatedMemory);
		target->allocatedMemory = NULL;
	}

	/* Lab 10 : Need to deallocate the entire link-list for arrAllocList (TODO) */

	/* Set all variables to default 0 */
	target->count = 0;
	target->unitSize = 0;
	target->seekerIndex = 0;
}

/* Scans the target memory pool for unused elements and return that as a result */
unsigned int MemoryMgrInactiveCount(struct MemoryPool* target)
{
	return 0;
}

void* MemoryMgrArrayAllocate(struct MemoryPool* pool, unsigned int count)
{
	return NULL;
}

void MemoryMgrArrayDeallocate(struct MemoryPool* pool, void* target)
{
}