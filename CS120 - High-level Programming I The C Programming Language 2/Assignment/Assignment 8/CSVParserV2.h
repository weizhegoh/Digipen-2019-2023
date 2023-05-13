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
const char* GetEntry(const struct CSVData* data, int rowIndex, int colIndex);

/* Deallocates all entries of the given CSV Data */
void FreeCSVData(struct CSVData* target);

/* New Functions to Implement -----------------------------------------------*/

/* Deep copy the data from the source to the result */
void CopyCSVData(struct CSVData* result, const struct CSVData* source);

/* Updates the entry at a given row and column index */
/* Note: Don't update/create if the entry does not exist! */
void UpdateEntry(struct CSVData* target, int rowIndex, int colIndex, const char* newData);

/* Deletes the entire row in the CSV Data */
void DeleteRow(struct CSVData* target, int rowIndex);

/* Inserts a row into the CSV Data at the end */
void InsertRow(struct CSVData* target, int colSize, ...);

/* Writes the contents of the CSV Data to a file, returns OK/FAIL */
int WriteCSVFile(const struct CSVData* source, const char* filePath);


#endif /* CSVPARSER_H */
/*****************************************************************************/
