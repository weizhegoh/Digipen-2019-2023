// pthread.cpp
// -- simple thread creation using POSIX
// cs180 1/08
#include <stdio.h>
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


void *palindrome(void *arg) {
  printf("Palindrome's address is %p\n", palindrome);
  std::cout << "Argument is " << arg << std::endl;
  pthread_t my_id = pthread_self();
  unsigned haha = (unsigned long) my_id;
  //pthread_self returns the calling thread's ID.
  
  srand(time(0));
  int max = 1 + rand()%10;
  std::cout << "Iterating " << max << " times" << std::endl;
  for (int i=0; i < max; ++i) {
    cout << haha << " printing " 
		 << table[rand()%table_size] << endl;
    sleep(1);
  }
  return ((void*)8888);
  //pthread_exit(0);
}


int main(void) {
  pthread_t thread_id1, thread_id2;  
  void *ret_value;
  pthread_create(&thread_id1, 
				 //Obtaining the thread ID by reference
			     0,  // Null Pointer default attributes
				 palindrome, //the starting function of the thread
				 ((void*)palindrome)); //Argument into the starting function);				 
  pthread_create(&thread_id2, 
				 //Obtaining the thread ID by reference
			     0,  // Null Pointer default attributes
				 palindrome, //the starting function of the thread
				 ((void*)palindrome)); //Argument into the starting function);				 
  
  pthread_join(thread_id1, &ret_value);
				//pthread_join is the thread equivalent 
				// of wait. pthread_join waits for a 
				//particular thread to ``join'' it. 
				// i.e., complete execution.
  std::cout << (long)(ret_value) << std::endl;				
  pthread_join(thread_id2, &ret_value);
				//pthread_join is the thread equivalent 
				// of wait. pthread_join waits for a 
				//particular thread to ``join'' it. 
				// i.e., complete execution.
  std::cout << (long)(ret_value) << std::endl;
  
  return 0;
}
