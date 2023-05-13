/******************************************************************************
filename ListSample.c
author Howard Sin
email howard.sin@digipen.edu
date created 7 Feb 2020
Brief Description: A sample partial List implementation 
******************************************************************************/
#include "List.h"
#include <stdlib.h>
#include <stdio.h>

/* Adds a node to the end of the list */
void AddToEnd(struct Node** ppList, int value)
{
	/* Temp pointer to store new node */
	struct Node* newNode = NULL;
	/* Serves as our runner to find the last node */
	struct Node* curr = NULL;

	/* Check ppList is valid */
	if (ppList == NULL)
	{
		/* Invalid, return don't do anything */
		return;
	}

	/* Allocate the Node memory via malloc */
	newNode = (struct Node*)malloc(sizeof(struct Node));
	/* Failed allocation, can't do anything else */
	if (newNode == NULL)
		return;

	/* Assign the values to the new Node */
	newNode->value = value;
	newNode->next = NULL;

	/* Find the location to add */
	/* Special case : Head is empty! */
	if (*ppList == NULL)
	{
		*ppList = newNode;
		return;
	}
	
	/* There are already nodes in the list, need to find the last spot */
	curr = *ppList;
	while (curr->next != NULL)
	{
		/* Keep advancing the runner while the next is not empty */
		curr = curr->next;
	}
	/* Here, we have curr at the last position, append the new node */
	curr->next = newNode;
}

/* Adds a node to the front of the list */
void AddToFront(struct Node** ppList, int value)
{
	/* Temp pointer to store new node */
	struct Node* newNode = NULL;

	/* Check ppList is valid */
	if (ppList == NULL)
	{
		/* Invalid, return don't do anything */
		return;
	}

	/* Allocate the Node memory via malloc */
	newNode = (struct Node*)malloc(sizeof(struct Node));
	/* Failed allocation, can't do anything else */
	if (newNode == NULL)
		return;

	/* Assign the values to the new Node */
	newNode->value = value;
	/* New node should point to whatever the head had */
	newNode->next = *ppList;

	/* Replace the head with the new node */
	*ppList = newNode;
}

/* Inserts a node in front of given index node */
void InsertAt(struct Node** ppList, int value, int index)
{
}

/* Returns number of nodes in the list */
int Count(struct Node* list)
{
	return 0;
}

/* Frees (deletes) all nodes in the list */
void FreeList(struct Node** pList)
{
}

/* Prints values of all nodes in list */
void PrintList(struct Node* list)
{
	/* Declare variable to iterate through the list*/
	struct Node* curr = list;

	/* Check if the list is empty */
	if (list == NULL)
	{
		printf("List is Empty");
		return;
	}

	/* Loop while current is not NULL */
	while (curr != NULL)
	{
		/* Print the current value */
		printf("%i->", curr->value);
		/* Update current to next */
		curr = curr->next;
	}
	/* Print NULL to showcase end point */
	printf("NULL");
}

/* Erases a node at the given index node */
void EraseNode(struct Node** ppList, int index)
{
}

/* Erase all nodes with the given value in the list */
void EraseValue(struct Node** ppList, int value)
{
}

/* Reverses the list */
void ReverseList(struct Node** ppList)
{
}