/******************************************************************************
filename MemoryManagerDriver.c
author Howard Sin
email howard.sin@digipen.edu
date created 12 Mar 2020
Brief Description: A driver file for MemoryManager
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "MemoryManager.h"

#define TEST_ARRAY_SIZE 10

int main(void)
{
	/* Variable Declaration */
	struct MemoryPool intPool;
	int* ptrArray[TEST_ARRAY_SIZE];
	int result, i;

	/* Functionality Tests */
	printf("Start of Functionality Tests\n");

	printf("Creating a pool of size 12 (will be rounded up to 16)...\n");
	result = MemoryMgrCreatePool(&intPool, sizeof(int), 12);
    
	if (result == MM_FAILED)
	{
		printf("ERROR: Failed to create pool\n");
		return 1;
	}
	
	printf("Allocating for each element in the pointer array...\n");
	for (i = 0; i < TEST_ARRAY_SIZE; ++i)
	{
		ptrArray[i] = MemoryMgrAllocate(&intPool);
		if (ptrArray[i] != NULL)
		{
			/* Set a value to the allocated integer */
			*ptrArray[i] = i;
		}
	}

	printf("Deallocating each element that is a multiple of 2...\n");
	for (i = 0; i < TEST_ARRAY_SIZE; i += 2)
	{
		/* Skip if element is NULL */
		if (ptrArray[i] == NULL)
			continue;

		MemoryMgrDeallocate(&intPool, (void*)ptrArray[i]);
		ptrArray[i] = NULL; /* Safe Deallocate, no dangling pointers */
	}

	printf("There are currently %u inactive elements in our pool...\n", MemoryMgrInactiveCount(&intPool));
  /*11*/
  
	printf("Filling up the pointer array again...\n");
	for (i = 0; i < TEST_ARRAY_SIZE; ++i)
	{
		/* Only allocate if the pointer is NULL */
		if (ptrArray[i] == NULL)
		{
			ptrArray[i] = MemoryMgrAllocate(&intPool);
			if (ptrArray[i] != NULL)
			{
				/* Set a value to the allocated integer */
				*ptrArray[i] = i;
			}
		}
	}

	printf("There are currently %u inactive elements in our pool...\n", MemoryMgrInactiveCount(&intPool));
  /*6*/
  
	printf("Deallocating every element...\n");
	for (i = 0; i < TEST_ARRAY_SIZE; ++i)
	{
		/* Skip if element is NULL */
		if (ptrArray[i] == NULL)
			continue;

		MemoryMgrDeallocate(&intPool, (void*)ptrArray[i]);
		ptrArray[i] = NULL; /* Safe Deallocate, no dangling pointers */
	}

	printf("There are currently %u inactive elements in our pool...\n", MemoryMgrInactiveCount(&intPool));
  /*16*/

	printf("Free the allocated pool...\n");
	MemoryMgrFreePool(&intPool);

	printf("End of Functionality Tests \n");

	/* Invalid Usage Tests */
	MemoryMgrCreatePool(NULL, sizeof(int), 12);
  printf("1\n");
	MemoryMgrCreatePool(&intPool, 0, 12);
  printf("2\n");
	MemoryMgrCreatePool(&intPool, sizeof(int), 0);
  printf("3\n");
	MemoryMgrAllocate(NULL);
  printf("4\n");
	MemoryMgrDeallocate(NULL, (void*)ptrArray[i]);
  printf("5\n");
	MemoryMgrDeallocate(&intPool, NULL);
  printf("6\n");
	MemoryMgrFreePool(NULL);
  printf("7\n");
	MemoryMgrInactiveCount(NULL);
  printf("8\n");

	return 0;
}