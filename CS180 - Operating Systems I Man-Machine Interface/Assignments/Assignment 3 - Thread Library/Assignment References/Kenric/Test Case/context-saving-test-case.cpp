#include <stdio.h>
#include <cstdlib>
#include "new-coro-lib.h"
void *t(void *a)
{
	long v_rdi, v_rsi, v_rbx, v_r12, v_r13, v_r14, v_r15;
	v_rdi = rand()%100;
	v_rsi = rand()%100;	
	v_rbx = rand()%100;
	v_r12 = rand()%100;
	v_r13 = rand()%100;	
	v_r14 = rand()%100;
	v_r15 = rand()%100;
	asm volatile
	( 	
      "movq %0, %%rdi\n\t"
      "movq %1, %%rsi\n\t"	  
      "movq %2, %%rbx\n\t"
      "movq %3, %%r12\n\t"	  
      "movq %4, %%r13\n\t"	  
      "movq %5, %%r14\n\t"	  
      "movq %6, %%r15\n\t"	  
	  : 
	  : "r" (v_rdi), "r" (v_rsi),"r" (v_rbx),"r" (v_r12), \
			"r" (v_r13),"r" (v_r14),"r" (v_r15)
	  : "rdi", "rsi", "rbx", "r11","r12", "r13", "r14"
	);
	
	CORO::thd_yield();
	
	long rdi, rsi, rbx, r12, r13, r14, r15;
	
	asm volatile
	( 
      "movq %%rdi, %0\n\t"
      "movq %%rsi, %1\n\t"
      "movq %%rbx, %2\n\t"
      "movq %%r12, %3\n\t"	  
      "movq %%r13, %4\n\t"	  
      "movq %%r14, %5\n\t"	  
      "movq %%r15, %6\n\t"	  
      : "+m" (rdi), "+m" (rsi), "+m" (rbx), "+m" (r12),\
			"+m" (r13), "+m" (r14), "+m" (r15)
	  :
	  : 
	);
	
	if (rdi!=v_rdi||rsi!=v_rsi||rbx!=v_rbx|| \
		r12!=v_r12||r13!=v_r13||r14!=v_r14||r15!=v_r15)
		printf ("%s: ERROR \n", a);
	else
		printf("%s: %d %d %d %d %d %d %d\n", a, \
				rdi, rsi, rbx, r12, r13, r14, r15);
		
	CORO::thd_yield();
	
	return NULL;
}

int main()
{
    CORO::ThreadID id;
    CORO::thd_init();
	char p1[] = "t1";
	char p2[] = "t2";
	
    id = CORO::new_thd(t, (void*)p2);
    t( (void*)p1);
}
