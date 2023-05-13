/******************************************************************************
filename StringNumDriver.cpp
author Howard Sin
email howard.sin@digipen.edu
date created 22 May 2019
Brief Description: A sample driver file for StringNum
******************************************************************************/
#include <stdio.h>
#include "StringNum.h"

#define BUFFER_SIZE 32
#define SM_BUFFER_SIZE 4

int main(void)
{
	/* Variable declaration */
	char buffer[BUFFER_SIZE] = {0};
	char smBuffer[SM_BUFFER_SIZE] = {0};
	char properString[] = "Proper String Here";
	char trickyString[8] = {'T', 'r', 'i', 'c', 'k', 0, 'y'};
	char* ptrToString = "Pointer To String";
	
	/* Functionality Tests */
	printf("StringLength of %s is %d\n", "StringLiteral", StringLength("StringLiteral"));
	printf("StringLength of %s is %d\n", properString, StringLength(properString));
	printf("StringLength of %s is %d\n", trickyString, StringLength(trickyString));
	printf("StringLength of %s is %d\n", ptrToString, StringLength(ptrToString));
	printf("\n");
	
	StringToUpper(properString);
	printf("Result of StringToUpper for properString: %s\n", properString);
	StringToUpper(trickyString);
	printf("Result of StringToUpper for trickyString: %s\n", trickyString);
	printf("\n");
	
	StringToLower(properString);
	printf("Result of StringToLower for properString: %s\n", properString);
	StringToLower(trickyString);
	printf("Result of StringToLower for trickyString: %s\n", trickyString);
	printf("\n");
	
	StringCopy(buffer, trickyString, BUFFER_SIZE);
	printf("Result of StringCopy from \"%s\" to \"%s\"\n", trickyString, buffer);
	StringCopy(smBuffer, ptrToString, SM_BUFFER_SIZE);
	printf("Result of StringCopy from \"%s\" to \"%s\"\n", ptrToString, smBuffer);
	StringCopy(buffer, "Difficult one coming up!", BUFFER_SIZE);
	printf("Result of StringCopy from \"%s\" to \"%s\"\n", "Difficult one coming up!", buffer);
	StringCopy(&buffer[4], buffer, BUFFER_SIZE-5);
	printf("Result of StringCopy Overlapped Result \"%s\"\n", buffer);
	printf("\n");
	
	printf("StringIsEqual \"%s\" vs \"%s\" : %s\n", "trick", trickyString, StringIsEqual("trick", trickyString)? "YES":"NO");
	printf("StringIsEqual \"%s\" vs \"%s\" : %s\n", "tricky", trickyString, StringIsEqual("tricky", trickyString)? "YES":"NO");
	printf("StringIsEqual \"%s\" vs \"%s\" : %s\n", "Trick", trickyString, StringIsEqual("Trick", trickyString)? "YES":"NO");
	printf("StringIsEqual \"%s\" vs \"%s\" : %s\n", "rick", trickyString, StringIsEqual("rick", trickyString)? "YES":"NO");
	printf("\n");
	
	printf("StringFind \"%s\" in \"%s\" : %s\n", "Proper", properString, StringFind(properString, "Proper")? "YES":"NO");
	printf("StringFind \"%s\" in \"%s\" : %s\n", "prop", properString, StringFind(properString, "prop")? "YES":"NO");
	printf("StringFind \"%s\" in \"%s\" : %s\n", "r Str", properString, StringFind(properString, "r Str")? "YES":"NO");
	printf("StringFind \"%s\" in \"%s\" : %s\n", "ring", properString, StringFind(properString, "ring")? "YES":"NO");
	printf("\n");
	
	printf("StringContains \"%s\" in \"%s\" : %s\n", "aeiou", properString, StringContains(properString, "aeiou")? "YES":"NO");
	printf("StringContains \"%s\" in \"%s\" : %s\n", "S", properString, StringContains(properString, "S")? "YES":"NO");
	printf("StringContains \"%s\" in \"%s\" : %s\n", "Se", properString, StringContains(properString, "Se")? "YES":"NO");
	printf("StringContains \"%s\" in \"%s\" : %s\n", "S !,+-", properString, StringContains(properString, "S !,+-")? "YES":"NO");
	printf("\n");
	
	printf("ArrayToInt from \"%s\" to %i\n", "1337", ArrayToInt("1337"));
	printf("ArrayToInt from \"%s\" to %i\n", "13a37", ArrayToInt("13a37"));
	printf("ArrayToInt from \"%s\" to %i\n", "abcd", ArrayToInt("abcd"));
	printf("ArrayToInt from \"%s\" to %i\n", "-9876", ArrayToInt("-9876"));
	printf("ArrayToInt from \"%s\" to %i\n", "-0", ArrayToInt("-0"));
	printf("\n");
	
	IntToString(buffer, 1337, BUFFER_SIZE);
	printf("IntToString from %i to \"%s\"\n", 1337, buffer);
	IntToString(buffer, -7331, BUFFER_SIZE);
	printf("IntToString from %i to \"%s\"\n", -7331, buffer);
	IntToString(smBuffer, 13333337, SM_BUFFER_SIZE);
	printf("IntToString from %i to \"%s\"\n", 13333337, smBuffer);
	printf("End of Functionality Tests\n");
	
	/* Invalid Usage Tests */
	StringLength(NULL);
	StringToUpper(NULL);
	StringToLower(NULL);
	StringCopy(NULL, NULL, 0);
	StringCopy(buffer, NULL, 0);
	StringCopy(NULL, buffer, 0);
	StringIsEqual(NULL, NULL);
	StringIsEqual(buffer, NULL);
	StringIsEqual(NULL, buffer);
	StringFind(NULL, NULL);
	StringFind(buffer, NULL);
	StringFind(NULL, buffer);
	StringContains(NULL, NULL);
	StringContains(buffer, NULL);
	StringContains(NULL, buffer);
	ArrayToInt(NULL);
	IntToString(NULL, 0, BUFFER_SIZE);
	
	return 0;
}
/*****************************************************************************/