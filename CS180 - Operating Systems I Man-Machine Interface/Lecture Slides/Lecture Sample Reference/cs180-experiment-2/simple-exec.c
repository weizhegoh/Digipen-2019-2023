#include "unistd.h"

int main()
{
	/* 
		Calling the ls utility program:
		Comment out the others and keep only one 
		un-commented for the experiments 
	*/
	execl("/bin/ls", "ls", "-l", 0);
	//execl("ls", "ls", "-l", 0);
	//execlp("ls", "ls", "-l", 0);
	//execlp("ls", "ls", "-l");
	
	printf("If execution reaches here, exec has failed.\n");
}