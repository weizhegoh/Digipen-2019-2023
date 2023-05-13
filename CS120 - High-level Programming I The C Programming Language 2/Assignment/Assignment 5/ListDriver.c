/******************************************************************************
filename ListDriver.c
author Howard Sin
email howard.sin@digipen.edu
date created 22 May 2019
Brief Description: A sample driver file for testing List implementation
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "List.h"

void PrintListNewLine(struct Node* list)
{
	PrintList(list);
	printf("\n");
}

int main(void)
{
	/* Variable Declaration */
	struct Node* listHead = NULL;

	/* Functionality Tests */
	printf("Start of Functionality Tests\n");
	printf("Print of empty list...\n");
	PrintListNewLine(listHead);

	printf("Adding 1, 2, 3 to the end of the list...\n");
	AddToEnd(&listHead, 1);
	AddToEnd(&listHead, 2);
	AddToEnd(&listHead, 3);
	PrintListNewLine(listHead);

	printf("Adding 4, 5, 6 to the front of the list...\n");
	AddToFront(&listHead, 4);
	AddToFront(&listHead, 5);
	AddToFront(&listHead, 6);
	PrintListNewLine(listHead);

	printf("Inserting 7 at index 0 (in front)...\n");
	InsertAt(&listHead, 7, 0);
	PrintListNewLine(listHead);
	printf("Inserting 8 at index 3...\n");
	InsertAt(&listHead, 8, 3);
	PrintListNewLine(listHead);

	printf("There are %i nodes in the list currently\n", Count(listHead));
	printf("Reversing the list...\n");
	ReverseList(&listHead);
	PrintListNewLine(listHead);

	printf("Erasing node at index 3...\n");
	EraseNode(&listHead, 3);
	PrintListNewLine(listHead);

	printf("Adding 5 at front and end, then removing all 5s...\n");
	AddToFront(&listHead, 5);
	AddToEnd(&listHead, 5);
	EraseValue(&listHead, 5);
	PrintListNewLine(listHead);

	printf("Removing all nodes from the list...\n");
	FreeList(&listHead);
	PrintListNewLine(listHead);

	printf("\nEnd of Functionality Tests\n");
  
	/* Invalid Usage Tests */
  AddToEnd(NULL, 0);
  printf("Done\n");
	AddToFront(NULL, 0);
  printf("Done1\n");
	InsertAt(NULL, 0, 0);
  printf("Done2\n");
	InsertAt(&listHead, 0, -1);
  printf("Done3\n");
	InsertAt(&listHead, 0, 1000);
  printf("Done4\n");
	Count(NULL);
  printf("Done5\n");
	FreeList(NULL);
  printf("Done6\n");
	PrintList(NULL);
  printf("Done7\n");
	EraseNode(NULL, 0);
  printf("Done8\n");
	EraseNode(&listHead, -1);
  printf("Done9\n");
	EraseNode(&listHead, 1000);
  printf("Done10\n");
	EraseValue(NULL, 0);
  printf("Done11\n");
	ReverseList(NULL);
  printf("Done12\n");
  
	return 0;
}