#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(int argc, char*argv[])
{
  int fileLength = 0, sqRoot = 0, length = 0, i = 0, j = 0, count = 0;
  FILE* inputFile = NULL;
  FILE* outputFile = NULL;
  char temp;
  char* buffer;
  char** transpose;
  
  if(argc!=4 || (strcmp(argv[1],"-e")!=0))
  {
    if(strcmp(argv[1],"-d")!=0)
    {
      printf("Error: Invalid usage\n");
      return 1;
    }
  }
  
if(strcmp(argv[1],"-e")==0)
{  
  inputFile = fopen(argv[2],"r");
  if(inputFile == NULL)
  {
    printf("Error: File not found\n");
    return 1;
  }
  fseek(inputFile, 0, SEEK_END);
  fileLength = (int)(ftell(inputFile));
  fseek(inputFile, 0, SEEK_SET); 
  
  printf("fileLength: %i\n", fileLength);
  
  sqRoot = (int)((sqrt(fileLength)+1));
  length = sqRoot*sqRoot;
  
  printf("root of File Length:  %d\n", sqRoot);
  printf("Next smallest square (length):  %d\n", length);
  
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

  printf("Buffer: %s\n",buffer);
  
  transpose = malloc(sizeof(char*)*(unsigned long)(sqRoot));
  for (i=0; i<sqRoot;++i)
  {
    transpose[i] = malloc(sizeof(char)*(unsigned long)(sqRoot));
  }
  
  printf("Before Transpose: "); 
   
  for(i=0;i<sqRoot;++i)
  {
    for(j=0;j<sqRoot;++j)
    {
      transpose[i][j] = buffer[i*sqRoot+j];      
      printf("%c", transpose[i][j]);
    }
  }
  
  printf("\nAfter Transpose: "); 
   
  for(i=1;i<sqRoot;++i)
  {
    for(j=0;j<i;++j)
    {
      temp = transpose[i][j];
      transpose[i][j] = transpose[j][i];
      transpose[j][i] = temp;
    }
  }
  
  for(i=0;i<sqRoot;++i)
  {
    for(j=0;j<sqRoot;++j)
    {
      printf("%c", transpose[i][j]);
    }
  }
  
  outputFile = fopen(argv[3],"w");
  
  if(outputFile==NULL)
  {
    printf("Error: Output Failed\n");
  }
  
  printf("\nOutput File Created\n");

  for(i=0;i<sqRoot;++i)
  {
    for(j=0;j<sqRoot;++j)
    {       
      fprintf(outputFile,"%c", transpose[i][j]);
    }
  }
  fclose(outputFile);
  
  for(i=0;i<sqRoot;++i)
    free(transpose[i]);
  free(transpose);
  
  free(buffer);
}

  if(strcmp(argv[1],"-d")==0)
  {  
    inputFile = fopen(argv[2],"r");
    if(inputFile == NULL)
    {
      printf("Error: File not found\n");
      return 1;
    }
    fseek(inputFile, 0, SEEK_END);
    fileLength = (int)(ftell(inputFile));
    fseek(inputFile, 0, SEEK_SET); 
  
    printf("fileLength: %i\n", fileLength);
  
    buffer = malloc(sizeof(char)*(unsigned long)(fileLength));

    while(!feof(inputFile))
    {
      fgets(buffer,fileLength+1,inputFile);
    }
    fclose(inputFile);

    printf("Buffer: %s\n",buffer);
    
    sqRoot = (int)sqrt(fileLength);
    printf("root of File Length:  %d\n", sqRoot);
    
    transpose = malloc(sizeof(char*)*(unsigned long)(sqRoot));
    for (i=0; i<sqRoot;++i)
    {
      transpose[i] = malloc(sizeof(char)*(unsigned long)(sqRoot));
    }
    
    printf("Before Transpose: "); 
   
    for(i=0;i<sqRoot;++i)
    {
      for(j=0;j<sqRoot;++j)
      {
        transpose[i][j] = buffer[i*sqRoot+j];      
        printf("%c", transpose[i][j]);
      }
    }
  
    printf("\nAfter Transpose: "); 
  
    for(i=1;i<sqRoot;++i)
    {
      for(j=0;j<i;++j)
      {
        temp = transpose[i][j];
        transpose[i][j] = transpose[j][i];
        transpose[j][i] = temp;
      }
    }
  
    for(i=0;i<sqRoot;++i)
    {
      for(j=0;j<sqRoot;++j)
      {
        printf("%c", transpose[i][j]);
      }
    }
    
    printf("\n");
  
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
  
    printf("After deleting duplicate last values: ");
    
    for(i=0;i<sqRoot;++i)
    {
      for(j=0;j<sqRoot;++j)
      {
        printf("%c", transpose[i][j]);
      }
    }
    
    outputFile = fopen(argv[3],"w");
  
    if(outputFile==NULL)
    {
      printf("Error: Output Failed\n");
    }
  
    printf("\nOutput File Created\n");

    for(i=0;i<sqRoot;++i)
    {
      for(j=0;j<sqRoot;++j)
      {        
        fprintf(outputFile,"%c", transpose[i][j]);
      }
    }
    fclose(outputFile);
  
    for(i=0;i<sqRoot;++i)
      free(transpose[i]);
    free(transpose);
  
    free(buffer);
  }
  return 0;
}  
