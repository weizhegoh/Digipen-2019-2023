/******************************************************************************
filename StringNum.h
author Goh Wei Zhe
email weizhe.goh@digipen.edu
date created 24 Jan 2020
Brief Description: A header file containing string and number functions
******************************************************************************/
#ifndef STRINGNUM_H
#define STRINGNUM_H

/* Finds and return the length of given string */
int StringLength(const char* s1);

/* Converts the alphabet letters to all upper-case for given string */
void StringToUpper(char* s1);

/* Converts the alphabet letters to all lower-case for given string */
void StringToLower(char* s1);

/* Copies a string from a source to a given destination up to length */
void StringCopy(char* dest, const char* source, int length);

/* Compares 2 strings, return 1 for match, 0 otherwise */
int StringIsEqual(const char* s1, const char* s2);

/* Finds the value string in target string, returns 1 for match, 0 otherwise */
int StringFind(const char* target, const char* value);

/* Finds of any of the values are in target, return 1 for match, 0 otherwise */
int StringContains(const char* target, const char* values);

/* Converts the string to an integer output */
int ArrayToInt(const char* input);

/* Converts an integer to a string and write to result with size limit */
void IntToString(char* result, int input, int limit);

#endif /* STRINGNUM_H*/
/*****************************************************************************/