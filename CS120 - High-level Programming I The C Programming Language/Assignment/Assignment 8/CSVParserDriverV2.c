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
#include "CSVParserV2.h"

/* #include "memcheck.h"
MEM_CHECK_MAIN;
 */
void PrintCSVData(const struct CSVData* csvFile)
{
	int i, j;
  
	for (i = 0; i < csvFile->size; ++i)
	{
		for (j = 0; j < csvFile->rowData[i].size; ++j)
		{
			printf("GetEntry[%i][%i] = %s\n", i, j, GetEntry(csvFile, i, j));
		}
	}
}

int main(void)
{
	/* Variable Declaration */
	struct CSVData dataFile;
	struct CSVData copyFile;
	int result = FAIL;
	
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

	printf("Printing entire read CSV data file...\n");
	PrintCSVData(&dataFile);
	
  printf("Copying to another CSV data file and printing it...\n");
	CopyCSVData(&copyFile, &dataFile);
	PrintCSVData(&copyFile);
	printf("Modifying \"Parser\" to \"Writer\"...\n");
	UpdateEntry(&dataFile, 1, 1, "Writer");
	PrintCSVData(&dataFile);

	printf("Deleting row 2...\n");
	DeleteRow(&dataFile, 2);

	PrintCSVData(&dataFile);

	printf("Inserting a new row at the end...\n");
	InsertRow(&dataFile, 5, "SA", "250", "10", "130", "20");
	PrintCSVData(&dataFile);

	printf("Writing the modified CSV data to Output.csv...\n");
	result = WriteCSVFile(&dataFile, "Output.csv");
	if (result == FAIL)
	{
		printf("Unable to write file...\n");
	}

	printf("Verify that the copy file is untouched...\n");
	PrintCSVData(&copyFile);

	printf("Attempting to Free CSV Data...\n");
	FreeCSVData(&dataFile);
	FreeCSVData(&copyFile);

	printf("Reading from Output.csv to ensure it is proper...\n");
	result = ReadCSVFile(&dataFile, "Output.csv");
	if (result == FAIL)
	{
		printf("Unable to read file...\n");
	}
	else
	{
		printf("Printing entire read CSV data file...\n");
		PrintCSVData(&dataFile);

		printf("Attempting to Free CSV Data...\n");
		FreeCSVData(&dataFile);
	}

	printf("End of Functionality Tests \n");
  
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

	CopyCSVData(NULL, &dataFile);
  printf("7\n");
	CopyCSVData(&copyFile, NULL);
  printf("8\n");
	UpdateEntry(NULL, 0, 0, "NewData");
  printf("9\n");
	UpdateEntry(&dataFile, 100, 0, "NewData");
  printf("10\n");
	UpdateEntry(&dataFile, 0, 100, "NewData");
  printf("11\n"); 
	UpdateEntry(&dataFile, 0, 0, NULL);
  printf("12\n");
	DeleteRow(NULL, 0);
  printf("13\n");
	DeleteRow(&dataFile, 100);
  printf("14\n");
	InsertRow(NULL, 3, "A", "B", "C");
  printf("15\n");
	InsertRow(&dataFile, 0);
  printf("16\n");
	WriteCSVFile(NULL, "Invalid.csv");
  printf("17\n");
	WriteCSVFile(&dataFile, NULL);
  printf("18\n");

	return 0;
}