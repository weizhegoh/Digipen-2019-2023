/******************************************************************************
filename List.c
author Goh Wei Zhe
email weizhe.goh@digipen.edu
date created: 7 Feb 2020
Brief Description: Understand Linked List and Pointers
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "List.h"

int Count(struct Node* list);

void AddToEnd(struct Node** ppList, int value)
{
  struct Node* newNode = NULL;
  struct Node* current = NULL;
  
  if(ppList == NULL)
    return;
  
  newNode = (struct Node*)malloc(sizeof(struct Node));
  if (newNode == NULL)
    return;
  
  newNode->value = value;
  newNode->next = NULL;
  
  if(*ppList == NULL)
  {
    *ppList = newNode;
    return;
  }
  
  current = *ppList;
  while (current->next != NULL)
  {
    current = current->next;
  }
  current->next = newNode;
}

void AddToFront(struct Node** ppList, int value)
{
  struct Node* newNode = NULL;
  
  if (ppList == NULL)
    return;
  
  newNode = (struct Node*)malloc(sizeof(struct Node));
  
  if (newNode == NULL)
    return;
  
  newNode->value = value;
  newNode->next = *ppList;
  
  *ppList = newNode;
}

void InsertAt(struct Node** ppList, int value, int index)
{
  int count = 1;
  struct Node* newNode = NULL;
  struct Node* current = NULL;
  struct Node* temp = NULL;
  
  if(ppList == NULL)
    return;
  
  if(index < 0)
    return;
  
  if(index == 0)
  {
    AddToFront(ppList, value);
    return; 
  }
  if(index >= Count(*ppList))
    return;
  
  newNode = (struct Node*)malloc(sizeof(struct Node));
  
  if (newNode == NULL)
    return;
  
  newNode->value = value;
  newNode->next = NULL;
  current = *ppList;
  
  while(current)
  {
    if(index == count)
    {
      temp = current->next;
      current->next = newNode;
      newNode->next = temp;
      break;
    }
    current = current->next;
    count++;
  }
}

int Count(struct Node* list)
{
  int count = 1;
  struct Node* current = list;
  if(list == NULL)
    return 1;
  
  while(current->next!=NULL)
  {
    count++;
    current = current->next;
  }
  return count;
}

void FreeList(struct Node** pList)
{
  struct Node* current = NULL;
  struct Node* delete = NULL;
  
  if(pList == NULL)
    return;
  
  current = *pList;
  
  while(current!=NULL)
  {
    delete = current;
    current = current->next;
    free(delete);
  }
  *pList = NULL;
}

void PrintList(struct Node* list)
{
  struct Node* current = list;
  
  if (list == NULL)
  {
    printf("List is Empty\n");
    return;
  }
  
  while (current != NULL)
  {
    printf("%i->", current->value);
    current = current->next;
  }
  printf("NULL\n");
}

void EraseNode(struct Node** ppList, int index)
{
  int count = 1;
  struct Node* current = NULL;
  struct Node* delete = NULL;
  
  if(ppList == NULL)
    return;
  
  if(index < 0)
    return;
  
  if(index >= Count(*ppList))
    return;
  
  current = *ppList;
  
  while(current->next != NULL)
  {
    if(index == count)
    {
      delete = current->next;
      current->next = current->next->next;
      free(delete);
      break;
    }
    current = current->next;
    count++;
  }
}

void EraseValue(struct Node** ppList, int value)
{
  struct Node* current = NULL;
  struct Node* delete = NULL;
  
  if(ppList == NULL)
    return;
  
  if(value < 0)
    return;
  
  if(value>=Count(*ppList))
    return;
  
  if(value == (*ppList)->value)
  {
    delete = *ppList;
    *ppList = (*ppList)->next;
    free(delete);
  }
  
  current = *ppList;
  
  while(current->next!=NULL)
  {
    if(current->next->value == value)
    {
      delete = current->next;
      current->next = current->next->next;
      free(delete);
      continue;
    }
    current = current->next;
  }
}

void ReverseList(struct Node **ppList)
{
  struct Node* previous = NULL;
  struct Node* current = NULL;
  struct Node* next = NULL;
  
  if(ppList==NULL)
    return;
  
  current = *ppList;
  
  while(current!= NULL)
  {
    next = current->next;
    current->next = previous;
    previous = current;
    current = next;
  }
  *ppList = previous;
}