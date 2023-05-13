#include <stdio.h>

/* tag a id for each type */
enum Bird_Type
{
  CROW = 0,
  DUCK,
  CHICKEN,
  SPARROW,
  PENGUIN,
  TOTAL_BIRD_TYPES
};

/* simple structure for a bird */
struct Bird
{
  enum Bird_Type TypeID;
  const char* Name;
};

/* names of all bird */
const char* BirdNames[TOTAL_BIRD_TYPES] = {"Crow", "Duck", "Chicken", "Sparrow", "Penguin"};

/*
each of this function can be split into different file,
therefore allowing the used of the term "one file one purpose",
where each file is specifically contains only functions related to that species
simply include the file and this will still work
*/
void Crow(const char* name)
{
  printf("%s can fly\n",name);
}

void Duck(const char* name)
{
  printf("%s can fly\n",name);
}

void Chicken(const char* name)
{
  printf("%s cannot fly\n",name);
}

void Sparrow(const char* name)
{
  printf("%s can fly\n",name);
}

void Penguin(const char* name)
{
  printf("%s cannot fly\n",name);
}

void InvokeFly(struct Bird* species)
{
  /* an array of function pointers for each species */
  static void (*fp[TOTAL_BIRD_TYPES])(const char*) = {Crow, Duck, Chicken, Sparrow, Penguin};
  
  /* invoke the function for the selected species */
  fp[species->TypeID](species->Name);
}