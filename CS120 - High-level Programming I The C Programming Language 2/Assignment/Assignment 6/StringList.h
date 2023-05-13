/******************************************************************************
filename StringList.h
author Howard Sin
email howard.sin@digipen.edu
date created 22 May 2019
Brief Description: A header file containing Node struct and LinkList Functions
******************************************************************************/
#ifndef STRING_LIST_H
#define STRING_LIST_H

/* Node Struct with Value and Pointer */
struct StringNode 
{
  char* value;
  struct StringNode* next;
};

/* Adds a node to the end of the list */
void AddStringToEnd(struct StringNode** ppList, const char* value);

/* Adds a node to the front of the list */
void AddStringToFront(struct StringNode** ppList, const char* value);

/* Inserts a node in front of given index node */
void InsertStringAt(struct StringNode** ppList, const char* value, int index);

/* Returns number of nodes in the list */
int CountString(struct StringNode* list);

/* Frees (deletes) all nodes in the list */
void FreeStringList(struct StringNode** pList);

/* Prints values of all nodes in list */
void PrintStringList(struct StringNode* list);

/* Erases a node at the given index node */
void EraseStringNode(struct StringNode** ppList, int index);

/* Erase all nodes with the given value in the list */
void EraseStringValue(struct StringNode** ppList, const char* value);

/* Reverses the list */
void ReverseStringList(struct StringNode** ppList);

/* Copies the list to another target */
void CopyStringList(struct StringNode** target, struct StringNode** source);

#endif /* STRING_LIST_H */
/*****************************************************************************/