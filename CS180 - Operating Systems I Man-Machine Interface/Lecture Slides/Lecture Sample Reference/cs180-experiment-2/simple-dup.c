#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void) 
{
	int fd = open("./temp1.txt", O_CREAT| O_RDWR);
	
	/*
			close(1) means making file descriptor 1 unused. 
			dup(fd) means we are duplicating fd on lowest unused 
			file descriptor, which is 1. 
			So that both fd and 1 will point to the same file. 
	*/
	
	close(1);
	dup(fd);
	
	/*
			You should find that all of the statements do
			the same thing. Why?
	*/
	write(1, "Hello World", 11);
	//printf("Hello World");
	//write(fd, "Hello World", 11);
	close(fd);
	return 0;
}
