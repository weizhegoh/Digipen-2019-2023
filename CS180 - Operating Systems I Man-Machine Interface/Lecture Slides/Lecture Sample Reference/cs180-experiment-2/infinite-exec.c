#include "unistd.h"
#include <stdio.h>
int main()
{
	/* 
		Calling the ls utility program:
		Comment out the others and keep only one 
		un-commented for the experiments 
	*/
	printf("My process ID is %d\n", getpid());
	execl("./infinite-exec.exe", "./infinite-exec.exe",  0);	
}