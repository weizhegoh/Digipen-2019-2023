/******************************************************************************
filename CSVParser.h
author Howard Sin
email howard.sin@digipen.edu
date created 22 May 2019
Brief Description: A header file containing CSV file parsing struct & functions
******************************************************************************/
#ifndef CSVPARSER_H
#define CSVPARSER_H

struct CSVRow
{
	char** colData;
	int size;
};

struct CSVData
{
	struct CSVRow* rowData;
	int size;
};

enum CSV_RETURN_VALUES
{
    OK,
    FAIL
};

/* Reads the CSV contents of a file, writes to result and return OK/FAIL */
int ReadCSVFile(struct CSVData* result, const char* filePath);

/* Gets an entry in the CSV data given the row and column index */
char* GetEntry(struct CSVData* data, int rowIndex, int colIndex);

/* Deallocates all entries of the given CSV Data */
void FreeCSVData(struct CSVData* target);

#endif /* CSVPARSER_H */
/*****************************************************************************/
