/* pipes.c
** -- using POSIX pipes
** cs180 1/08
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char** argv) 
{

	int pid;	
	int descriptor[2];
	char *buffer;
	int size_of_buffer, i;
		
	if(argc!=2)
	{
		fprintf(stderr, "Usage: %s <buffer-size>\n", argv[0]);
		return 1;
	}
	
	pipe(descriptor);
	pid = fork();
	
	size_of_buffer=atoi(argv[1]);
	buffer = malloc(size_of_buffer);
		
	if (pid == 0) {	
		
		for(i=0;i<size_of_buffer;i++)
			buffer[i]='a'+rand()%26;
		
		write(descriptor[1],buffer,size_of_buffer);		
		
		exit(0); 
	}
	else 
	{			
		
		int bytes_read;
		wait(NULL);
		{			
			bytes_read=read(descriptor[0],buffer, size_of_buffer);
			buffer[bytes_read]='\0';
			if(bytes_read>0)
			{     
				fprintf(stdout,"Parent reading message:\"%s\"\n", buffer);			
			}
		}
	}
	
	close(descriptor[0]);
	close(descriptor[1]);
	return 0;
}
