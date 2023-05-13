#include <stdio.h>
static volatile int interested[2]={0};
static volatile int turn;
static int count=0;
void get_mutex(int pid)
{
    interested[pid]=1;
    int other = pid?0:1;	
    turn=pid;    
    __asm__ ("mfence");
    while(turn == pid && interested[other]);
}

void release_mutex(int pid)
{
  interested[pid]=0;
}
