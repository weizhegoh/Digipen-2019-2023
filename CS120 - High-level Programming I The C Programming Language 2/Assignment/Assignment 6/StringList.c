/******************************************************************************
filename StringList.c
author Goh Wei Zhe
email weizhe.goh@digipen.edu
date created 22 Feb 2020
Brief Description: 
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include "StringList.h"

int CountString(struct StringNode* list);

struct StringNode* createNode(const char* string)
{
  struct StringNode* newString = NULL;
  newString = (struct StringNode*)malloc(sizeof(struct StringNode));
  
   if(newString == NULL)
    return NULL;
  
  newString->value = (char*)malloc(sizeof(char)*(strlen(string)+1));
  
   strcpy(newString->value, string);
   newString->next = NULL;
  
  return newString;
}

void AddStringToEnd(struct StringNode** ppList, const char* value)
{
  struct StringNode* current = NULL;
  
  if(ppList == NULL || value == NULL)
    return;
  
  if(*ppList == NULL)
  {
    *ppList = createNode(value);
    return;
  }
  
  current = *ppList;
  while(current->next!= NULL)
  {
    current = current->next;
  }
  current->next = createNode(value);
}

void AddStringToFront(struct StringNode** ppList, const char* value)
{
  struct StringNode* newString = NULL;

  if(ppList == NULL || value == NULL)
    return;
  
  newString = createNode(value);
  newString->next = *ppList;
  *ppList = newString;
}

void InsertStringAt(struct StringNode** ppList, const char* value, int index)
{
  struct StringNode *current = NULL;
  struct StringNode *newString = NULL;
  struct StringNode *temp = NULL;
  int count = 0;
  
  if(ppList == NULL || value == NULL)
    return;
  
  if(index < 0 || index >=CountString(*ppList))
    return;
  
  if(index == 0)
  {
    AddStringToFront(ppList, value);
    return;
  }
  
  newString = createNode(value);
  current = *ppList;
  
  while(current)
  {
    if(count == index-1)
    {
      temp = current->next;
      current->next = newString;
      newString->next = temp;
      break;
    }
    current = current->next;
    count++;
  }
}

int CountString(struct StringNode* list)
{
  int count = 0;
  
  if(list == NULL)
    return 1;
  
  while(list)
  {
    list = list->next;
    count++;
  }
  return count;
}

void FreeStringList(struct StringNode** pList)
{
  struct StringNode* current = NULL;
  struct StringNode* delete = NULL;
  if(pList == NULL)
    return;
  
  current = *pList;
  while(current)
  {
    delete = current;
    current = current->next;
    free(delete->value);
    free(delete);
  }
  *pList = NULL;
}

void PrintStringList(struct StringNode* list)
{
  if(list == NULL)
  {
    printf("List is Empty\n");
    return;
  }
  
  while(list)
  {
    printf("%s -> ",list->value);
    list = list->next; 
  }
  
  printf("NULL\n");
}

void EraseStringNode(struct StringNode** ppList, int index)
{
  struct StringNode* current = NULL;
  struct StringNode* delete = NULL;
  int count = 0;
  
  if(ppList == NULL)
    return;
  
  if(index < 0 || index >=CountString(*ppList))
    return;
  
  current = *ppList;
  
  if (index == 0)
  {
    *ppList = current->next;
    free(current->value);
    free(current);
    return;
  }
  
  while(current->next)
  {
    if(count == index-1)
    {
      delete = current->next;
      current->next = current->next->next;
      free (delete->value);
      free(delete);
      break;
    }
    current = current->next;
    count++;
  }
}

void EraseStringValue(struct StringNode** ppList, const char* value)
{
  struct StringNode* current = NULL;
  struct StringNode* delete = NULL;
  
  if(ppList == NULL || value == NULL)
    return;
  
  if(strcmp((*ppList)->value,value)==0)
  {
    delete = *ppList;
    *ppList = (*ppList)->next;
    free(delete->value);
    free(delete);
  }
  
  current = *ppList;
  
  while(current->next)
  {
    if(strcmp(current->next->value,value)==0)
    {
      delete = current->next;
      current->next = current->next->next;
      free(delete->value);
      free(delete);
      continue;
    }
    current = current->next;
  }
}

void ReverseStringList(struct StringNode** ppList)
{
  struct StringNode* previous = NULL;
  struct StringNode* current = NULL;
  struct StringNode* next = NULL;
  
  if(ppList == NULL)
    return;
  
  current = *ppList;
  
  while(current)
  {
    next = current->next;
    current->next = previous;
    previous = current;
    current = next;
  }
  *ppList = previous;
}

void CopyStringList(struct StringNode** target, struct StringNode** source)
{
  struct StringNode* copy = NULL;
  struct StringNode* current = NULL;
  
  if (target == NULL || source == NULL)
    return;
  
  FreeStringList(target);
  
  current = *source;
  
  *target = (struct StringNode*)malloc(sizeof(struct StringNode));
  
  if (*target == NULL)
    return;
  
  (*target)->value = (char*)malloc(sizeof(char)*(strlen((*source)->value) + 1));
  strcpy((*target)->value, current->value);
  (*target)->next = NULL;
  
  current = current->next;
  copy = (*target);
  
  while (current)
  {
    copy->next = (struct StringNode*)malloc(sizeof(struct StringNode));
    copy = copy->next;
    copy->value = (char*)malloc(sizeof(char)*(strlen((*source)->value) + 1));
    copy->next = NULL;
    
    strcpy(copy->value, current->value);
    current = current->next;
  }
}