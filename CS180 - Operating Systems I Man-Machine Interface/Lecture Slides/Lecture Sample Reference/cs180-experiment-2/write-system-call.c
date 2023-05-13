#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void) 
{

	int fd = open("./temp.txt", O_CREAT| O_RDWR);
	write(fd, "Hello World", 5);
	close(fd);
	return 0;
}
