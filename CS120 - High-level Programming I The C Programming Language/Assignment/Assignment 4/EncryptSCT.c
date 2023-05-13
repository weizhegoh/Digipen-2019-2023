/******************************************************************************
filename EncryptSCT.c
author Goh Wei Zhe
email weizhe.goh@digipen.edu
date created 31 Jan 2020
Brief Description: To create a program that will take in any file, read the
content and perform a simple columnar tranposition cipher on it. Then, write 
the result in a specified output file. 
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(int argc, char*argv[])
{ 
  int fileLength = 0, sqRoot = 0, length = 0, i = 0, j = 0, count = 0;
  FILE* inputFile = NULL, *outputFile = NULL;
  char temp = 0, *buffer = NULL, ** transpose = NULL;

  if(argc!=4)
  {
    printf("Error: Invalid usage\n");
    return 1;
  }
  
  if((strcmp(argv[1],"-e")!=0))
  {
    if(strcmp(argv[1],"-d")!=0)
    {
      printf("Error: Invalid usage\n");
      return 1;
    }
  }
  
  inputFile = fopen(argv[2],"r");
  if(inputFile == NULL)
  {
    printf("Error: File not found\n");
    return 1;
  }
  
  fseek(inputFile, 0, SEEK_END);
  fileLength = (int)(ftell(inputFile));
  fseek(inputFile, 0, SEEK_SET);   
  
  if(strcmp(argv[1],"-e")==0)
  {
    sqRoot = (int)((sqrt(fileLength)+1));
    length = sqRoot*sqRoot;
  
    buffer = malloc(sizeof(char)*(unsigned long)(length));
    while(!feof(inputFile))
    {
      fgets(buffer,length,inputFile);
    }
    fclose(inputFile);
  
    for(i=fileLength;i<length;++i)
    {
      buffer[i]= buffer[fileLength-1];
    }
    buffer[length] = 0;
  }

  if(strcmp(argv[1],"-d")==0)
  {
    buffer = malloc(sizeof(char)*(unsigned long)(fileLength));
    while(!feof(inputFile))
    {
      fgets(buffer,fileLength+1,inputFile);
    }
    fclose(inputFile);
  
    sqRoot = (int)sqrt(fileLength);
  }

  transpose = (char**)malloc(sizeof(char*)*(unsigned long)(sqRoot));
  
  for (i=0; i<sqRoot;++i)
  {
    transpose[i] = (char*)malloc(sizeof(char)*(unsigned long)(sqRoot));
  }
  
  /*Moving buffer 1D into transpose 2D*/
  for(i=0;i<sqRoot;++i)
  {
    for(j=0;j<sqRoot;++j)
    {
      transpose[i][j] = buffer[i*sqRoot+j];      
    }
  }
  
  /*Transpose*/
  for(i=1;i<sqRoot;++i)
  {
    for(j=0;j<i;++j)
    {
      temp = transpose[i][j];
      transpose[i][j] = transpose[j][i];
      transpose[j][i] = temp;
    }
  }

  if(strcmp(argv[1],"-d")==0)
  {
    for(i=sqRoot-1;i>=0;--i)
    {
      for(j=sqRoot-1;j>=0;--j)
      { 
        if(j==0 && transpose[i][j]==transpose[i-1][sqRoot-1])
        {
          transpose[i][j] = ' ';
        }
        else 
          if(transpose[i][j]==transpose[i][j-1])
          {
            transpose[i][j] = ' ';
          }
          else 
          {
            count = 1;
            break; 
          }
      }
      if(count == 1)
      break;
    }
  }
  
  outputFile = fopen(argv[3],"w");
  
  if(outputFile==NULL)
  {
    printf("Error: Output failed\n");
    return 1;
  }
  
  for(i=0;i<sqRoot;++i)
  {
    for(j=0;j<sqRoot;++j)
    {       
      fprintf(outputFile,"%c", transpose[i][j]);
    }
  }
  fclose(outputFile);
   
  for(i=0;i<sqRoot;++i)
  {
    free(transpose[i]);
  } 
  free(transpose);
  free(buffer);

  return 0;
}  
