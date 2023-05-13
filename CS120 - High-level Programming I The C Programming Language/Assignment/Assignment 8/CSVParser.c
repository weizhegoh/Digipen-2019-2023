/******************************************************************************
filename CSVParserDriver.c
author Goh Wei Zhe
email weizhe.goh@digipen.edu
date created 5th March 2019
Brief Description: Read contents of CSV file, display, copy, update all entries,
delete & insert row then write content of CSV Data to a file.
deallocate all entries
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "CSVParserV2.h"  

int ReadCSVFile(struct CSVData* result, const char* filePath)
{ 
  int fileLength = 0;
  int  row = 0, column = 0, rowCount = 0, colCount = 0, i = 0;
  FILE* inputFile = NULL; 
  char character, *fileBuffer = NULL, *token = NULL, *newline;

  if(result == NULL || filePath == NULL)
    return FAIL;
  
  inputFile = fopen(filePath,"r");  
  
  if(inputFile == NULL)
  {
    printf("InputFile does not exist\n");
    return FAIL;
  } 
  
  ++row;
  fseek(inputFile,0,SEEK_END);
  fileLength = (int)(ftell(inputFile));
  fseek(inputFile, 0, SEEK_SET);
    
  if(fileLength == 0)
  {
    printf("FilePath is Empty\n");
    return FAIL;
  }
  
  while(!feof(inputFile))
  {
    character = (char)fgetc(inputFile);
    if(character == '\n')
      row++;
  } 
  
  fseek(inputFile, 0, SEEK_SET);
  result->size = row;
  
  result->rowData = 
  (struct CSVRow*)malloc(sizeof(struct CSVRow)*(unsigned long)(row));
  
  if(result->rowData == NULL)
  {
    printf("Fail to malloc number of rowCounts\n");
    fclose(inputFile);
    return FAIL;
  }
  
  fileBuffer = (char*)malloc(sizeof(char)*(unsigned long)(fileLength));

  if(fileBuffer == NULL)
  {
    printf("Fail to malloc fileBuffer\n");
    free(result->rowData);
    fclose(inputFile);
    return FAIL;
  }
    
  while(fgets(fileBuffer,fileLength, inputFile))
  {
    for(i=0;i<(int)(strlen(fileBuffer));++i)
    {
      if(fileBuffer[i] == ',')
      {
        column++;
      }
    }
    column+=1;
    
    result->rowData[rowCount].colData = 
    (char**)malloc(sizeof(char*)*(unsigned long)(column));
    
    if(result->rowData[rowCount].colData == NULL)
    {
      printf("Unable to malloc number of coloumns\n");
      free(fileBuffer);
      free(result->rowData);
      fclose(inputFile);
      return FAIL;
    }
    result->rowData[rowCount].size = column;
    rowCount++;
    column = 0;
  }
  
  fseek(inputFile, 0, SEEK_SET);
  rowCount = 0;

  while(fgets(fileBuffer, fileLength, inputFile))
  {
    token = strtok(fileBuffer, ",");
    
    while(token)
    {
      newline = strchr(token, '\n');
      
      if(newline)
      {
        *newline = 0;
      }
      result->rowData[rowCount].colData[colCount]=
      (char*)malloc(sizeof(char)*(strlen(token)+1));
     
      if(result->rowData[rowCount].colData[colCount] == NULL)
      {
        printf("Unable to malloc result->rowData[%d].colData[%d]\n",rowCount,colCount);
        free(fileBuffer);
        free(result->rowData);
        fclose(inputFile);
        return FAIL;
      }
      strcpy(result->rowData[rowCount].colData[colCount],token);
      
      token = strtok(NULL, ",");
      colCount++;
    }
    ++rowCount;
    colCount = 0;
  }
  
  free(fileBuffer);
  fclose(inputFile);
  return OK;
}

const char* GetEntry(const struct CSVData* data, int rowCountIndex, int colIndex)
{
  if(data == NULL)
    return NULL;
  
  if(rowCountIndex < 0  || (rowCountIndex >= data->size))
    return NULL;
  
  if(colIndex < 0 || colIndex >= data->rowData[rowCountIndex].size)
    return NULL;

  return (data->rowData[rowCountIndex].colData[colIndex]);
}

void FreeCSVData(struct CSVData* target)
{
  int i = 0, j = 0;
  
  if(target == NULL)
  {
    printf("Data File is empty\n");
    return;
  }
  
  for(i = 0; i< target->size;++i)
  {
    for(j = 0; j< target->rowData[i].size; ++j)
    {
      free(target->rowData[i].colData[j]);
    }
    free(target->rowData[i].colData);
  }
  free(target->rowData);
  target->size = -1;
}

void CopyCSVData(struct CSVData* result, const struct CSVData* source)
{
  int i = 0, j = 0;
  if(result == NULL || source == NULL)
    return;
  
  result->size = source->size;

  result->rowData = 
  (struct CSVRow*)malloc(sizeof(struct CSVRow)*(unsigned long)(source->size));
  
  if(result->rowData == NULL)
    return; 
  
  for(i=0;i<source->size;++i)
  {
    result->rowData[i].colData = 
    (char**)malloc(sizeof(char*)*(unsigned long)(source->rowData[i].size));
    
    if(result->rowData[i].colData == NULL)
    {
      free(result->rowData[i].colData);
    }
    result->rowData[i].size = source->rowData[i].size;
    
    for(j=0;j<source->rowData[i].size;++j)
    {
      result->rowData[i].colData[j] = 
      (char*)malloc(sizeof(char)*strlen(source->rowData[i].colData[j])+1);
      
      if(result->rowData[i].colData[j] == NULL)
      {
        for(i=0;i<source->size;++i)
        {
          free(result->rowData[i].colData);
        }
        free(result->rowData);
      }
      strcpy(result->rowData[i].colData[j],source->rowData[i].colData[j]);
    }
  }
}

void UpdateEntry(struct CSVData* target, int rowIndex, int colIndex, const char* newData)
{
  if(target == NULL || newData == NULL)
  {
    printf("target or newData is null\n");
    return;
  }
  
  if(rowIndex < 0 || rowIndex >= target->size)
  {
    printf("row < 0 || rowIndex >= target->size\n");
    return;
  }
  
  if(colIndex < 0 || colIndex >= (target->rowData[rowIndex].size))
  {
    printf("col < 0 || colIndex >= target->rowData[rowIndex].size]\n");
    return;
  }
  
  free(target->rowData[rowIndex].colData[colIndex]);
  
  target->rowData[rowIndex].colData[colIndex] = 
  (char*)malloc(sizeof(char)*strlen(newData)+1);
  
  strcpy(target->rowData[rowIndex].colData[colIndex],newData);
}

void DeleteRow(struct CSVData* target, int rowIndex)
{
  int i = 0, j = 0;
  struct CSVData* newTarget = NULL;
  
  if(target == NULL)
    return;
  
  if(rowIndex < 0 || rowIndex >= target->size)
    return;

  newTarget = (struct CSVData*)malloc(sizeof(struct CSVData)); 

  newTarget->size = (target->size)-1;

  newTarget->rowData = 
  (struct CSVRow*)malloc(sizeof(struct CSVRow)*(unsigned long)(newTarget->size));

  if(newTarget->rowData == NULL)
    return; 

  for(i=0;i<newTarget->size;++i)
  { 
    if(i >= rowIndex)
    {
      newTarget->rowData[i].colData = 
      (char**)malloc(sizeof(char*)*(unsigned long)(target->rowData[i+1].size));
        
      if(newTarget->rowData[i].colData == NULL)
      {
        free(newTarget->rowData);
      }
      
      newTarget->rowData[i].size = target->rowData[i+1].size;
            
      for(j=0; j<newTarget->rowData[i].size;++j)
      {
        newTarget->rowData[i].colData[j] = target->rowData[i+1].colData[j];
        target->rowData[i+1].colData[j] = NULL;
      }
    }
    else
    {
      newTarget->rowData[i].colData = 
      (char**)malloc(sizeof(char*)*(unsigned long)(target->rowData[i].size));
    
      if(newTarget->rowData[i].colData == NULL)
      {
        free(newTarget->rowData);
      }
    
      newTarget->rowData[i].size = target->rowData[i].size;
    
      for(j=0;j<newTarget->rowData[i].size;++j)
      {
        newTarget->rowData[i].colData[j] = target->rowData[i].colData[j];
        target->rowData[i].colData[j] = NULL;
      }
    }
  }
  FreeCSVData(target);
  target->rowData = newTarget->rowData;
  target->size = newTarget->size;
  free(newTarget);
}

void InsertRow(struct CSVData* target, int colSize, ...)
{
  int i = 0, j = 0;
  struct CSVData* newTarget = NULL;
  char* string = NULL; char* newline = NULL;
  va_list list; 
 
  if(target == NULL || colSize <= 0)
    return;
  
  va_start(list, colSize);
   
  newTarget = (struct CSVData*)malloc(sizeof(struct CSVData)); 

  newTarget->size = (target->size)+1;

  newTarget->rowData = 
  (struct CSVRow*)malloc(sizeof(struct CSVRow)*(unsigned long)(newTarget->size));

  if(newTarget->rowData == NULL)
    return; 

  for(i=0;i<newTarget->size;++i)
  {
    if(i == newTarget->size-1)
    {            
      newTarget->rowData[i].colData = 
      malloc(sizeof(char*)*(unsigned long)colSize);

      newTarget->rowData[i].size = colSize;
      
      if(newTarget->rowData[i].colData == NULL)
      {
        free(newTarget->rowData);
      }
      
      for(j=0;j<colSize;++j)
      {
        string = va_arg(list, char*);
      
        newline = strchr(string, '\n');
      
        if(newline)
        {
          *newline = 0;
        }
       
        newTarget->rowData[i].colData[j]= 
        (char*)malloc(sizeof(char)*strlen(string)+1);  
        
        strcpy(newTarget->rowData[i].colData[j],string);  
      }
    }
    else
    {
      newTarget->rowData[i].colData = 
      (char**)malloc(sizeof(char*)*(unsigned long)(target->rowData[i].size));

      if(newTarget->rowData[i].colData == NULL)
      {
        free(newTarget->rowData);
      }
   
      newTarget->rowData[i].size = target->rowData[i].size;
    
      for(j=0;j<newTarget->rowData[i].size;++j)
      {
        newTarget->rowData[i].colData[j] = target->rowData[i].colData[j];
        target->rowData[i].colData[j] = NULL;
      }  
    }
  }
  
  FreeCSVData(target);
  target->rowData = newTarget->rowData;
  target->size = newTarget->size;
  free(newTarget);
  va_end(list);
}

int WriteCSVFile(const struct CSVData* source, const char* filePath)
{ 
  int i = 0, j = 0;
  FILE* outputFile = NULL;
 
  if(source == NULL || filePath == NULL)
    return FAIL;
  
  outputFile = fopen(filePath, "w");
  
  if(outputFile == NULL)
  {
    printf("Output.csv not created\n");
    return FAIL; 
  }

  for(i=0;i<source->size;++i)
  {
    for(j=0;j<source->rowData[i].size;++j)
    {
      if(j == source->rowData[i].size-1 && i == source->size-1)
      {
        fprintf(outputFile,"%s", source->rowData[i].colData[j]);      
      }
      else
        if(j == source->rowData[i].size-1)
        {
        fprintf(outputFile,"%s\n", source->rowData[i].colData[j]);
        }
        else
        {
          fprintf(outputFile,"%s,", source->rowData[i].colData[j]);      
        }
    }
  }
  fclose(outputFile);
  
  return OK;
}