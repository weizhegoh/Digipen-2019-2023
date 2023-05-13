/******************************************************************************
filename XORCipher.c
author Howard Sin
email howard.sin@digipen.edu
date created 23 Mar 2020
Brief Description: A Simple XOR Cipher Encryption Program
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	FILE* inputFile = NULL, * outputFile = NULL;
	char* password = NULL;
	unsigned int passwordLength = 0;
	unsigned int fileLength = 0;
	char* buffer = NULL;
	unsigned int i = 0;

	/* Basic input argument check */
	if (argc != 4)
	{
		printf("ERROR: Invalid usage - <ProgramName>.exe <InputPath> <OutputPath> <Password>");
		return 1;
	}

	/* Attempt to open file for reading */
	inputFile = fopen(argv[1], "rb");
	if (inputFile == NULL)
	{
		printf("ERROR: Unable to open %s for reading\n", argv[1]);
		return 1;
	}

	/* Figure out the length of the file */
	fseek(inputFile, 0, SEEK_END);
	fileLength = ftell(inputFile);
	fseek(inputFile, 0, SEEK_SET);

	/* Allocate memory for the buffer to read the file */
	buffer = malloc(fileLength);
	if (buffer == NULL)
	{
		printf("ERROR: Unable to allocate %u bytes for reading file\n", fileLength);
		fclose(inputFile);
		return 1;
	}

	/* Read the file to the buffer, then close the file */
	fread(buffer, sizeof(char), fileLength, inputFile);
	fclose(inputFile);

	/* Figure out the password length */
	password = argv[3];
	passwordLength = strlen(password);

	/* Apply XOR cipher on the data */
	for (i = 0; i < fileLength; ++i)
	{
		buffer[i] ^= password[i % passwordLength];
	}

	/* Open output file for writing */
	outputFile = fopen(argv[2], "wb");
	if (outputFile == NULL)
	{
		printf("ERROR: Unable to open %s for writing\n", argv[2]);
		free(buffer);
		return 1;
	}

	/* Write the data to file */
	fwrite(buffer, sizeof(char), fileLength, outputFile);

	/* Clean up */
	printf("SUCCESS!\n");
	fclose(outputFile);
	free(buffer);
}