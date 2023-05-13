/******************************************************************************
filename CSVParserDriver.c
author Howard Sin
email howard.sin@digipen.edu
date created 22 May 2019
Brief Description: A sample driver for testing CSV Parser
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include "CSVParser.h"

int main(void)
{
	/* Variable Declaration */
	struct CSVData dataFile;
	int result = FAIL;
	int i, j;
	dataFile.rowData = NULL;
	dataFile.size = 0;

	/* Functionality Tests */
	printf("Start of Functionality Tests\n");
	result = ReadCSVFile(&dataFile, "SampleCSVFile.csv");
	if (result == FAIL)
	{
		printf("Unable to read file, terminating...\n");
		return 1;
	}

  /* printf("content in row 9 column 0: %s\n\n", dataFile.rowData[8].colData[0]);
  printf("dataFile.size is: %d\n", dataFile.size); */

	printf("Printing entire read CSV data file...\n");
	for (i = 0; i < dataFile.size; ++i)
	{
		for (j = 0; j < dataFile.rowData[i].size; ++j)
		{
			printf("GetEntry[%i][%i] = %s\n", i, j, GetEntry(&dataFile, i, j));
		}
	}
	printf("End of Functionality Tests (FreeCSVData test delayed)\n");

	/* Invalid Usage Tests */
	ReadCSVFile(NULL, "SampleCSVFile.csv");
  printf("1\n");
	ReadCSVFile(&dataFile, NULL);
  printf("2\n");
	GetEntry(NULL, 0, 0);
  printf("3\n");
	GetEntry(&dataFile, 100, 0);
  printf("4\n");
	GetEntry(&dataFile, 0, 100);
  printf("5\n");
	FreeCSVData(NULL);
  printf("6\n");

	/* Post-Invalid Usage Test (FreeCSVData) */
	printf("Attempting to Free CSV Data...\n");
	FreeCSVData(&dataFile);

	return 0;
}