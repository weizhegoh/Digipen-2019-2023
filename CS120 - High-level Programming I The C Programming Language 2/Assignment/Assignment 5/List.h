/******************************************************************************
filename List.h
author Howard Sin
email howard.sin@digipen.edu
date created 22 May 2019
Brief Description: A header file containing Node struct and LinkList Functions
******************************************************************************/
#ifndef LIST_H
#define LIST_H

/* Node Struct with Value and Pointer */
struct Node 
{
  int value;
  struct Node* next;
};

/* Adds a node to the end of the list */
void AddToEnd(struct Node** ppList, int value);

/* Adds a node to the front of the list */
void AddToFront(struct Node** ppList, int value);

/* Inserts a node in front of given index node */
void InsertAt(struct Node** ppList, int value, int index);

/* Returns number of nodes in the list */
int Count(struct Node* list);

/* Frees (deletes) all nodes in the list */
void FreeList(struct Node** pList);

/* Prints values of all nodes in list */
void PrintList(struct Node* list);

/* Erases a node at the given index node */
void EraseNode(struct Node** ppList, int index);

/* Erase all nodes with the given value in the list */
void EraseValue(struct Node** ppList, int value);

/* Reverses the list */
void ReverseList(struct Node **ppList);

#endif /* LIST_H */
/*****************************************************************************/