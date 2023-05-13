/******************************************************************************
filename CS120_Quiz5Driver.c
author Howard Sin
email howard.sin@digipen.edu
date created 29 Mar 2020
Brief Description: The driver file for CS120 Quiz 5
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/* 
Instructions:
You are to provide the following file below to make the code compile
*/
#include "CS120_Quiz5Answer.h"

/* 
Grading Scheme upon 100 total score:
Code compiles - 50 points
Expected output achieved - 10 points
Use of function pointers - 20 points
Detailed comments about how your code works - 20 points
*/

/*
Expected Output:
Crow can fly
Duck can fly
Chicken cannot fly
Sparrow can fly
Penguin cannot fly
*/
int main(void)
{
	struct Bird birdArray[TOTAL_BIRD_TYPES];
	int i = 0;

	for (i = 0; i < TOTAL_BIRD_TYPES; ++i)
	{
		birdArray[i].TypeID = i;
		birdArray[i].Name = BirdNames[i];
	}

	for (i = 0; i < TOTAL_BIRD_TYPES; ++i)
	{
		InvokeFly(&birdArray[i]);
	}

	return 0;
}