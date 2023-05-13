/******************************************************************************
filename StringNum.c
author Goh Wei Zhe
email weizhe.goh@digipen.edu
date created 24 Jan 2020
Brief Description: Understand how string library works in C
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "StringNum.h"
#define _CRT_SECURE_NO_WARNINGS


/* Finds and return the length of given string */
int StringLength(const char* s1)
{
  int count = 0;
  
  if(s1 == NULL)
    return 0;
  
  while(*s1)
  {
    count++;
    s1++;
  }
  return count;
}

/* Converts the alphabet letters to all upper-case for given string */
void StringToUpper(char* s1)
{
  if(s1 == NULL)
    return;
  
  while(*s1)
  {
    if(*s1>='a' && *s1<='z')
    {
      *s1= (char)(*s1+('A'-'a'));
    }
    s1++;
  }
}

/* Converts the alphabet letters to all lower-case for given string */
void StringToLower(char* s1)
{
  if(s1 == NULL)
    return;
  
  while(*s1)
  {
    if(*s1>='A' && *s1<='Z')
    {
      *s1 = (char)(*s1-('A'-'a'));
    }
    s1++;
  }
}

/* Copies a string from a source to a given destination up to length */
void StringCopy(char* dest, const char* source, int length)
{
  char* srcCpy = NULL;
  int srcLength = 0, curr = 0;
  
  if (dest == NULL || source == NULL || length <= 0)
    return;
  
  srcLength = StringLength(source);
  srcCpy = (char*)(malloc(sizeof(char)*(unsigned int)(srcLength+1)));
  
  if (srcCpy == NULL)
    return;
  
  for(curr = 0; curr < srcLength; ++curr)
    srcCpy[curr] = source[curr];
  
  srcCpy[srcLength] = 0;
  curr = 0;
  
  while (srcCpy[curr])
  {
    if (curr >= length-1)
      break;
    
    dest[curr] = srcCpy[curr];
    ++curr;
  }
  dest[curr] = 0;
  
  free(srcCpy);
} 

/* Compares 2 strings, return 1 for match, 0 otherwise */
int StringIsEqual(const char* s1, const char* s2)
{
  if(s1 == NULL || s2 == NULL)
    return 0;
  
  while((*s1 && *s2) && (*s1==*s2))
  {
    ++s1;
    ++s2;
  }
  
  if(*s1 == *s2)
    return 1;
  else 
    return 0;
}

/* Finds the value string in target string, returns 1 for match, 0 otherwise */
int StringFind(const char* target, const char* value)
{
  if(target == NULL || value == NULL)
    return 0;
  
  while(*target)
  {
    if(*value==*target)
    {
      ++target;
      ++value;
      
      if(*value==0)
        return 1;
    }
    else
      ++target;
  }
  return 0;
}

/* Finds of any of the values are in target, return 1 for match, 0 otherwise */
int StringContains(const char* target, const char* values)
{
  const char* tempValues = values;
  
  if(target == NULL || values == NULL)
    return 0;
  
  while(*target)
  {
    while(*values)
    {
      if(*values == *target)
        return 1;
      else
        ++values;
    }
    values = tempValues;
    ++target;
  }
  return 0;
}

/* Converts the string to an integer output */
int ArrayToInt(const char* input)
{
  int result = 0, sign = 1, i = 0;
  
  if(input == NULL)
    return 0;
  
  if(input[0] == '-')
  {
    sign = -1;
    ++i;
  }
  
  for (;input[i]!='\0';++i)
  {
    if(input[i]>='0' && input[i] <='9')
      result = result * 10 + input[i] - '0';
    else
      return result*sign;
  }
  return result*sign;
}

/* Converts an integer to a string and write to result with size limit */
void IntToString(char* result, int input, int limit)
{
  int isNegative = 0, i = 0, remainder = 0, curr = 0, count = 0, srcLength = 0;
  char* srcCpy = NULL;
  
  if (result == NULL || input == 0)
    return;
  
  if(input < 0)
  {
    isNegative = 1;
    input = -input;
  }
  
  while(input)
  {
    remainder = input % 10;
    result[i++] = (char)(remainder +'0');
    input = input / 10;
  }
  
  if(isNegative == 1)
    result[i++]= '-';
  
  result[i] = '\0';
  
  srcLength = StringLength(result);
  srcCpy = (char*)(malloc((sizeof(char)*(unsigned int)(srcLength+1))));
  
  if (srcCpy == NULL)
    return;
  
  for(curr = srcLength-1; curr >= 0; --curr)
    srcCpy[count++] = result[curr];
  
  srcCpy[srcLength] = 0;
  curr = 0;
  
  while (srcCpy[curr])
  {
    if (curr >= limit-1)
      break;
    
    result[curr] = srcCpy[curr];
    ++curr;
  }
  result[curr] = 0;
  
  free(srcCpy);
}

/*****************************************************************************/