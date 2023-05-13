#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void) 
{
	char buffer[100];
	
	buffer[read(0, buffer, 100)]='\0';
	//gets(buffer);
	
	write(1, buffer, strlen(buffer));
	//fprintf(stdout, "%s\n", buffer);
	
	write(2, buffer, strlen(buffer));
	//fprintf(stderr, "%s\n", buffer);
}
