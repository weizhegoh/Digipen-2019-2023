/******************************************************************************
filename StringListDriver.c
author Howard Sin
email howard.sin@digipen.edu
date created 22 May 2019
Brief Description: A sample driver file for testing List implementation
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "StringList.h"

void PrintListNewLine(struct StringNode* list)
{
	PrintStringList(list);
	printf("\n");
}

int main(void)
{
	/* Variable Declaration */
	struct StringNode* listHead = NULL;
	struct StringNode* listCopy = NULL;
	char buffer[32] = "BufferString";

	/* Functionality Tests */
	printf("Start of Functionality Tests\n");
	printf("Print of empty list...\n");
	PrintListNewLine(listHead);

	printf("Adding First, Second, \"buffer\" to the end of the list...\n");
	AddStringToEnd(&listHead, "First");
	AddStringToEnd(&listHead, "Second");
	AddStringToEnd(&listHead, buffer);
	PrintListNewLine(listHead);
  
	printf("Adding Fourth, Fifth, \"buffer\" to the front of the list...\n");
	AddStringToFront(&listHead, "Fourth");
	AddStringToFront(&listHead, "Fifth");
	AddStringToFront(&listHead, buffer);
	PrintListNewLine(listHead);
	
  printf("Inserting Seventh at index 0 (in front)...\n");
	InsertStringAt(&listHead, "Seventh", 0);
	PrintListNewLine(listHead);
	printf("Inserting \"buffer\" at index 3...\n");
	InsertStringAt(&listHead, buffer, 3);
	PrintListNewLine(listHead);

	printf("There are %i nodes in the list currently\n", CountString(listHead));
	printf("Reversing the list...\n");
  ReverseStringList(&listHead);
	PrintListNewLine(listHead);

	printf("Erasing node at index 3...\n");
	EraseStringNode(&listHead, 3);
	PrintListNewLine(listHead);
  
	printf("Adding Fifth at front and end, then removing all Fifths...\n");
	AddStringToFront(&listHead, "Fifth");
	AddStringToEnd(&listHead, "Fifth");
	EraseStringValue(&listHead, "Fifth");
	PrintListNewLine(listHead);

	printf("Modifying buffer, your list data should not change...\n");
	strcpy(buffer, "Incorrect");
	PrintListNewLine(listHead);

	printf("Copy from listHead to listCopy...\n");
	CopyStringList(&listCopy, &listHead);
	PrintListNewLine(listCopy);

	printf("Removing all nodes from the copy list...\n");
	FreeStringList(&listCopy);
	PrintListNewLine(listCopy);

	printf("Adding dummy data into listCopy, then attempt to copy from listHead to listCopy"); 
  printf("(Dummy should be removed)...\n");
	AddStringToFront(&listCopy, "Dummy");
	AddStringToFront(&listCopy, "Dummy");
	AddStringToFront(&listCopy, "Dummy");

  CopyStringList(&listCopy, &listHead);
	PrintListNewLine(listCopy);

	printf("Removing all nodes from the list...\n");
	FreeStringList(&listHead);
	PrintListNewLine(listHead);
	printf("Copied list should still be intact...\n");
	PrintListNewLine(listCopy);
	printf("Removing all nodes from the copy list...\n");
	FreeStringList(&listCopy);
	PrintListNewLine(listCopy);

	printf("End of Functionality Tests\n");

	/* Invalid Usage Tests */
	AddStringToEnd(NULL, buffer);
  printf("1\n");
	AddStringToEnd(&listHead, NULL);
  printf("2\n");
	AddStringToFront(NULL, buffer);
  printf("3\n");
	AddStringToFront(&listHead, NULL);
  printf("4\n");
	InsertStringAt(NULL, buffer, 0);
  printf("5\n");
	InsertStringAt(&listHead, NULL, 0);
  printf("6\n");
	InsertStringAt(&listHead, buffer, -1);
  printf("7\n");
  InsertStringAt(&listHead, buffer, 1000);
  printf("8\n");
	CountString(NULL);
  printf("9\n");
	FreeStringList(NULL);
  printf("10\n");
	PrintStringList(NULL);
  printf("11\n");
	EraseStringNode(NULL, 0);
  printf("12\n");
	EraseStringNode(&listHead, -1);
  printf("13\n");
	EraseStringNode(&listHead, 1000);
  printf("14\n");
	EraseStringValue(NULL, buffer);
  printf("15\n");
	EraseStringValue(&listHead, NULL);
  printf("16\n");
	ReverseStringList(NULL);
  printf("17\n");
	CopyStringList(NULL, &listCopy);
  printf("18\n");
	CopyStringList(&listHead, NULL);
  printf("19\n");
	return 0;
}