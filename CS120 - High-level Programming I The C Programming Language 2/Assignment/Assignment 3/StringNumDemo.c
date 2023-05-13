/******************************************************************************
filename StringNum.c
author Howard Sin
email howard.sin@digipen.edu
date created 24 Jan 2020
Brief Description: A partially completed StringNum function definition
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "StringNum.h"

/* Finds and return the length of given string */
int StringLength(const char* s1)
{
	/* variable to store resultant size */
	int result = 0;
	
	/* NULL pointer check, return 0 if true */
	if (s1 == NULL)
		return 0;
	
	/* Loop and increment result until we found NULL string terminator */
	while(s1[result] != 0)
		++result;
	
	/* Return counted result variable */
	return result;
}

/* Converts the alphabet letters to all upper-case for given string */
void StringToUpper(char* s1)
{
	/* Variable to get our current index */
	int curr = 0;
	
	/* NULL pointer check, return if so */
	if (s1 == NULL)
		return;
	
	/* Loop until the end of the string */
	while(s1[curr] != 0)
	{
		/* Check if it is within lower case range */
		if (s1[curr] >= 'a' && s1[curr] <= 'z')
		{
			/* Add offset value to get upper case */
			s1[curr] += 'A' - 'a';
		}
		/* Increment the index variable */
		++curr;
	}
}

/* Converts the alphabet letters to all lower-case for given string */
void StringToLower(char* s1)
{
	/* Variable to get our current index */
	int curr = 0;
	
	/* NULL pointer check, return if so */
	if (s1 == NULL)
		return;
	
	/* Loop until the end of the string */
	while(s1[curr] != 0)
	{
		/* Check if it is within lower case range */
		if (s1[curr] >= 'A' && s1[curr] <= 'Z')
		{
			/* Add offset value to get upper case */
			s1[curr] += 'a' - 'A';
		}
		/* Increment the index variable */
		++curr;
	}
}

/* Copies a string from a source to a given destination up to length */
void StringCopy(char* dest, const char* source, int length)
{
	char* srcCpy = NULL;
	int srcLength = 0;
	int curr = 0;
	
	/* NULL and invalid length check */
	if (dest == NULL || source == NULL || length <= 0)
		return;
	
	/* Get the string length of source */
	srcLength = StringLength(source);
	
	/* Allocate another memory region to temporarily store the source */
	srcCpy = malloc(sizeof(char)*(srcLength+1));
	/* Check if allocation is successful */
	if (srcCpy == NULL)
		return;
	
	/* Loop the source, copy to srcCpy */
	for(curr = 0; curr < srcLength; ++curr)
		srcCpy[curr] = source[curr];
	
	/* Must remember null terminator for srcCpy */
	srcCpy[srcLength] = 0;
	
	/* Reset the curr back to 0 */
	curr = 0;
	/* Loop until the end of the srcCpy string */
	while (srcCpy[curr] != 0)
	{
		/* We will stop if we exceed the buffer length limit */
		if (curr >= length-1)
			break;
		
		/* Copy from srcCpy to destination */
		dest[curr] = srcCpy[curr];
		
		/* Increment curr */
		++curr;
	}
	/* Remember to add the NULL terminator at the end */
	dest[curr] = 0;
	
	/* Free allocated temporary memory */
	free(srcCpy);
}

/* Compares 2 strings, return 1 for match, 0 otherwise */
int StringIsEqual(const char* s1, const char* s2)
{
	/* TODO */
	return 0;
}

/* Finds the value string in target string, returns 1 for match, 0 otherwise */
int StringFind(const char* target, const char* value)
{
	/* TODO */
	return 0;
}

/* Finds of any of the values are in target, return 1 for match, 0 otherwise */
int StringContains(const char* target, const char* values)
{
	/* TODO */
	return 0;
}

/* Converts the string to an integer output */
int ArrayToInt(const char* input)
{
	/* TODO */
	return 0;
}

/* Converts an integer to a string and write to result with size limit */
void IntToString(char* result, int input, int limit)
{
	/* TODO */
}

/*****************************************************************************/