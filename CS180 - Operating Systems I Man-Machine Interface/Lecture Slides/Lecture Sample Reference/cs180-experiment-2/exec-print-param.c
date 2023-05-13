#include "unistd.h"
#include <stdio.h>
int main()
{
	execl("./print-param.exe", "1", "2", "3", "4", "5",	0);	
}