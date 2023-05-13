/******************************************************************************
filename CS120_Quiz5Answer.h
author Goh Wei ZHe
email weizhe.goh@digipen.edu
date created 30 Mar 2020
Brief Description: Quiz 5
******************************************************************************/
/*Assign members of TypeID, Name into Struct Bird*/
struct Bird
{
  int TypeID; 
  char* Name; 
};

/*Initialise number to each bird and count of total bird*/ 
enum BirdNames
{
  Crow,
  Duck,
  Chicken, 
  Sparrow, 
  Penguin, 
  TOTAL_BIRD_TYPES
};

/*Initialise BirdName array of 5 birds*/
char* BirdNames[TOTAL_BIRD_TYPES] = {"Crow", "Duck", "Chicken", "Sparrow", " Penguin"};

/*Check birdID if it corresponds to the number assigned to them and print*/
void InvokeFly(struct Bird* birds)
{
  switch(birds->TypeID)
  {
    case 0: printf("Crow can fly\n"); break;
    case 1: printf("Duck can fly\n"); break;
    case 2: printf("Chicken cannot fly\n"); break; 
    case 3: printf("Sparrow can fly\n"); break;
    case 4: printf("Penguin cannot fly\n"); break;
  }
}