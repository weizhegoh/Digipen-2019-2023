/******************************************************************************
filename MemoryManagerDriverV2.c
author Howard Sin
email howard.sin@digipen.edu
date created 26 Mar 2020
Brief Description: A driver file for MemoryManager
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "MemoryManagerV2.h"

int main(void)
{
	/* Variable Declaration */
	struct MemoryPool intPool;
	int* ptrArray[4];
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
	
	printf("Using up all the memory in our pool by assigning to our 4 pointers 4 elements each...\n");
	for (i = 0; i < 4; ++i)
	{
		ptrArray[i] = MemoryMgrArrayAllocate(&intPool, 4);
	}
	printf("There are currently %u inactive elements in our pool...\n", MemoryMgrInactiveCount(&intPool));


	printf("Deallocate the middle 2 blocks...\n");
/* 	MemoryMgrArrayDeallocate(&intPool, ptrArray[0]);
 */
	MemoryMgrArrayDeallocate(&intPool, ptrArray[1]);
	MemoryMgrArrayDeallocate(&intPool, ptrArray[2]);
	ptrArray[1] = NULL;
	ptrArray[2] = NULL;
	printf("There are currently %u inactive elements in our pool...\n", MemoryMgrInactiveCount(&intPool));

	printf("Allocate an array of 8 to use up all the memory again...\n");
	ptrArray[1] = MemoryMgrArrayAllocate(&intPool, 8);
	printf("There are currently %u inactive elements in our pool...\n", MemoryMgrInactiveCount(&intPool));

	printf("Attempt to allocate another (it should fail and return NULL)...\n");
	ptrArray[2] = MemoryMgrArrayAllocate(&intPool, 8);
	printf("Over-allocation result : %s\n", ptrArray[2] ? "INCORRECT":"CORRECT");

	printf("Deallocate all arrays...\n");
	for (i = 0; i < 4; ++i)
	{
		if (ptrArray[i] != NULL)
		{
			MemoryMgrArrayDeallocate(&intPool, ptrArray[i]);
			ptrArray[i] = NULL;
		}
	}
	printf("There are currently %u inactive elements in our pool...\n", MemoryMgrInactiveCount(&intPool));

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

	/* New Invalid Usage Tests */
	MemoryMgrArrayAllocate(NULL, 3);
  printf("9\n");
	MemoryMgrArrayAllocate(&intPool, 0);
  printf("10\n");
	MemoryMgrArrayDeallocate(NULL, NULL);
  printf("11\n");

	return 0;
}