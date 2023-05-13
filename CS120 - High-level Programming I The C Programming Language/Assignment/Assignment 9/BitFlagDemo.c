/******************************************************************************
filename BitFlagDemo.c
author Howard Sin
email howard.sin@digipen.edu
date created 23 Mar 2020
Brief Description: A demo showcasing bit flags and bitwise operators
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* Bitwise Operators + Bit Flags */
typedef unsigned int FLAG;
const unsigned short FLAG_LIMIT = sizeof(FLAG) * 8;

/* Macro to disable the flag limit check in release mode */
#ifdef _DEBUG
#define FLAG_LIMIT_CHECK(x) do { assert(x < FLAG_LIMIT); } while(0)
#else
#define FLAG_LIMIT_CHECK(x) do {} while(0)
#endif 

bool CheckFlag(const FLAG flag, unsigned short index)
{
	FLAG_LIMIT_CHECK(index);
	return flag & (1 << index);
}

void SetFlag(FLAG* flag, unsigned short index)
{
	FLAG_LIMIT_CHECK(index);
	*flag |= (1 << index);
}

void ClearFlag(FLAG* flag, unsigned short index)
{
	FLAG_LIMIT_CHECK(index);
	*flag &= ~(1 << index);
}

void ToggleFlag(FLAG* flag, unsigned short index)
{
	FLAG_LIMIT_CHECK(index);
	*flag ^= (1 << index);
}

enum PLAYER_STATUS
{
	POISONED,
	BURNING,
	BLEEDING,
	FROZEN,
	SLOWED,
	PLAYER_STATUS_COUNT
};

const char* statusText[PLAYER_STATUS_COUNT] =
{ "Poisoned", "Burning", "Bleeding", "Frozen", "Slowed" };

void PrintPlayerStatus(FLAG flag)
{
	unsigned short i = 0;
	for (i = 0; i < PLAYER_STATUS_COUNT; ++i)
	{
		if (CheckFlag(flag, i))
		{
			printf("Player is %s\n", statusText[i]);
		}
	}
}

int main(void)
{
	FLAG playerFlags = 0;
	unsigned short i = 0;

	/*Start of bit flag demo */
	printf("Player got hit by an arrow in the knee!\n");
	SetFlag(&playerFlags, BLEEDING);
	SetFlag(&playerFlags, SLOWED);
	PrintPlayerStatus(playerFlags);

	printf("Healing player from bleeding\n");
	ClearFlag(&playerFlags, BLEEDING);
	PrintPlayerStatus(playerFlags);

	printf("Flipping all statuses on player\n");
	for (i = 0; i < PLAYER_STATUS_COUNT; ++i)
	{
		ToggleFlag(&playerFlags, i);
	}
	PrintPlayerStatus(playerFlags);

	printf("Flipping all statuses on player again...\n");
	for (i = 0; i < PLAYER_STATUS_COUNT; ++i)
	{
		ToggleFlag(&playerFlags, i);
	}
	PrintPlayerStatus(playerFlags);
	printf("Player Flag Value = %u\n", playerFlags);

	return 0;
}