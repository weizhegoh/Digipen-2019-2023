#include <stdio.h>

int main(int argc, char **argv)
{
	int i;
	for(i=0; i<argc; i++)	
		printf("Arg %i: %s\n", i, argv[i]);	
}