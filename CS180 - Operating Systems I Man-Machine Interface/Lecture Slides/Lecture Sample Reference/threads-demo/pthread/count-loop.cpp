// pthread.cpp
// -- simple thread creation using POSIX
// cs180 1/08

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
using namespace std;

char const *table[]
   = { "A man, a plan, a canal: Panama!",
       "No lemons, no melon.",
       "Step on no pets.",
       "Was it a rat I saw?",
       "Dog as a devil deified lived as a god.",
       "Able was I ere I saw Elba.",
       "Yawn! Madonna fan? No damn way!",
       "Go hang a salami. I'm a lasagna hog!"
     };

const int table_size = 8;
int acc=0;

void *palindrome(void *arg) {
	int count = (long)arg;
	std::cout << getpid() << std::endl;
	for(int i=0; i<count; ++i)
	{
		acc++;
	}	
	return 0;
}


int main(void) {
  pthread_t thread_id1, thread_id2;  
  void *ret_value;
  pthread_create(&thread_id1, 
				 //Obtaining the thread ID by reference
			     0,  // Null Pointer default attributes
				 palindrome, //the starting function of the thread
				 ((void*)10000000)); //Argument into the starting function);				 
  
  pthread_create(&thread_id2, 
				 //Obtaining the thread ID by reference
			     0,  // Null Pointer default attributes
				 palindrome, //the starting function of the thread
				 ((void*)10000000)); //Argument into the starting function);				 
  
  
				//pthread_join is the thread equivalent 
				// of wait. pthread_join waits for a 
				//particular thread to ``join'' it. 
				// i.e., complete execution.
  pthread_join(thread_id1, &ret_value);	 
  pthread_join(thread_id2, &ret_value);
				//pthread_join is the thread equivalent 
				// of wait. pthread_join waits for a 
				//particular thread to ``join'' it. 
				// i.e., complete execution.
  std::cout << acc << std::endl;
  
  return 0;
}
