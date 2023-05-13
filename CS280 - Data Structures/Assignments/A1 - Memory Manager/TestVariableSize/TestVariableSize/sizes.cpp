#include <stdio.h> /* printf */ 
#include <stdint.h> /* intptr_t */ 

int main(void) 
{
	int i = 123; /* sizeof(int) depends on architecture */ 
	int *p1 = &i; /* works on all architectures */ 
	intptr_t j = 456; /* intptr_t is a pointer-sized int */ 
	intptr_t *p2 = &j; /* works on all architectures */ 
	i = (int)p1; /* Probably works on 32-bit, may not on 64-bit */ 
	j = (intptr_t)p2; /* Will work on either architecture */ 
	printf("sizeof(short) : %lu\n", sizeof(short)); 
	printf("sizeof(int) : %lu\n", sizeof(int)); 
	printf("sizeof(long) : %lu\n", sizeof(long)); 
	printf("sizeof(long long) : %lu\n", sizeof(long long)); 
	printf("sizeof(void*) : %lu\n", sizeof(void*)); 
	printf("sizeof(size_t) : %lu\n", sizeof(size_t)); 
	printf("sizeof(intptr_t) : %lu\n", sizeof(intptr_t)); 
	return 0; 
}