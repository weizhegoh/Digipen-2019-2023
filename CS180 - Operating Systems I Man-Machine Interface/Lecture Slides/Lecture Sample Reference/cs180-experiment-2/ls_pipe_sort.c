#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main()
{
	int pid;
	char buffer[1024];
	int descriptor[2];
	int i;
	char c;
	
	/* descriptor[0] - for reading
	** descriptor[1] - for writing
	*/
	pipe(descriptor);

	printf("Parent's PID is %d\n", getpid());

	pid = fork();
	
	if (pid == 0)
	{
		//Child's code
		printf("1st child PID is %d\n", getpid());
		
		//close the reading end of the pipe for safety's sake. 
		close(descriptor[READ_END]);
	
		//close 1, which is a file descriptor pointing to stdout
		close(1);
	
		//duplicate the writing end. Since 1 is the smallest file descriptor
		//available, 1 will point to the writing end after this.
		dup(descriptor[WRITE_END]);

		//exec call to run ls
		execlp("ls", "ls", "-l", NULL);
	}
	else
	{
		//Here we perform another fork to show that
		//a pipe can be shared between any 2 processes of the same ANCESTRY
		if(fork()==0)
		{
			//2nd child of the parent run here.
			printf("2nd child PID is %d\n", getpid());
			// Again, all file descriptors are inherited in this child.
			
			//close the writing end of the pipe for safety's sake
			close(descriptor[WRITE_END]);
	
			//close 0, a file descriptor originally pointing to stdin
			close(0);
	
			//duplicate the reading end, since 0 is smallest file decsriptor
			//available, 0 will point to the reading end after this. 
			dup(descriptor[READ_END]);
	
			//exec call to run sort.
			execlp("sort", "sort", NULL);
		}	
	
		// Parent can wait for both child to complete. No need to use the pipes
		// in parent. Parent can close the pipes while the pipes are 
		// still being used in the children processes. 
		close(descriptor[WRITE_END]);
		close(descriptor[READ_END]);
		wait(NULL);
		wait(NULL);
	}
}
