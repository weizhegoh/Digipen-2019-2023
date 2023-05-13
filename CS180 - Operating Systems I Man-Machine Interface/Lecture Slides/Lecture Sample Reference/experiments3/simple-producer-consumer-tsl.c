#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include "tsl.h"
#define BUFFER_SIZE 256

/*
    Global Variables:
    1. Of course not supposed to have global variables in any "real" code.
    2. For purposes of demo only.
    3. produced_buffer is the "shared buffer" between all producers and consumers.
       Producers write into it, Consumers read from it.
    4. Two locks:
        a.  space_count_lock - ensure mutual exclusive access to space_count variable.
        b.  produced_count_lock - ensure mutual exclusive access to produced_count variable.
        c.  buffer_lock - ensure mutual exclusive access to produced_buffer.
    5. consumed_buffer - a shared buffer among the consumers for storing whatever 
                         they obtained from the buffer.
    6. spacecount is the variable keeping track of the number of produced items in the
       buffer not consumed.
*/
int produced_count=0, space_count=BUFFER_SIZE;
int produced_buffer[BUFFER_SIZE];
int *consumed_buffer;
Lock space_count_lock, produced_count_lock;
Lock buffer_lock;

/*
    Consumer thread function.
    1. out - the index in the produced_buffer that can be consumed.
    2. consumed_in - the index in the consumed_buffer that can be written to.
    3. out and consumed_in are "shared" by all consumer threads. 
       Note the static declaration.	
    4. consumed, num and i are local variables. num is the number of times
       the loop will iterate.	   
*/

void *consumer(void *arg)
{
    static unsigned char out=0;
    static unsigned consumed_in = 0;
    int consumed;
    int num = (int) arg;
    int i;
    
    for(i=0; i< num ; i++)
    {
        /*
            Read the produced_count variable.
            Spin if produced_count is 0;
            (i.e., buffer is empty).
            Otherwise, decrement produced_count and move into 
            consumption.
        */
        while(1)
        {
            int temp;
            acquire(&produced_count_lock);
            temp=produced_count;            
            if(temp>0)
                produced_count--;
            release(&produced_count_lock);
            if(temp!=0)
                break;
        }
        /*
            Get mutual exclusive access to produced_buffer
        */		
        acquire(&buffer_lock);
        consumed = produced_buffer[out++];					
        consumed_buffer[consumed_in++] = consumed;        
        release(&buffer_lock);		
        
        /*
            Get mutual exclusive access to space_count
            and increment it because there's one more 
            space available now.
        */
        acquire(&space_count_lock);
        space_count++;
        release(&space_count_lock);		
    }
}	

/*
    Producer thread function.
    1. in - the index in the produced_buffer that can be written to.
    2. produced_num - the currently produced number.
    3. in and produced_num are "shared" by all consumer threads. 
       Note the static declaration.	
    4. num and i are local variables. num is the number of times
       the loop will iterate.
    5. Note that because produced_num is put within a mutual exclusive
       section, it will increase from  1 to (number of producers threads * num).	
*/

void *producer(void *arg)
{
    static unsigned char in = 0;
    static int produced_num = 0;
    int num = (int) arg;
    int i =0;
    
    for(i=0; i< num ; i++)
    {
        /*
            Read the space_count variable.
            Spin if space_count is 0;
            (i.e., buffer is full).
            Otherwise, increment space_count and 
            enter production.
        */
        while(1)
        {
            int temp;
            acquire(&space_count_lock);
            temp=space_count;
            if(temp>0)
                space_count--;
            release(&space_count_lock);
            if(temp!=0)
            {				
                break;
            } 
        }
        /*
            Get mutual exclusive access to produced_buffer
        */		
        acquire(&buffer_lock);        
        produced_buffer[in++] = ++produced_num;	        
        release(&buffer_lock);
        
        /*
            Get mutual exclusive access to produced_count
            and increment it because there's one more 
            produced item now.
        */
        acquire(&produced_count_lock);        
        produced_count++;
        release(&produced_count_lock);		
    }
}

/*
    The main function.
*/


int main(int argc, char**argv)
{
    int num, flags;
    int num_of_consumers, num_of_producers;
    int i;
    pthread_attr_t a;
    pthread_t *producer_thds, *consumer_thds; 
    
    /*
        Error message indicating usage of the function.
    */
    
    
    if(argc!=3)
    {
        fprintf(stderr, "Usage: %s <num_of_times> <num of consumer-producer thds>\n", argv[0]);
        exit(-1);
    }
    
    /*
        Obtaining the arguments of the program.
    */	
    
    num = atoi(argv[1]);
    num_of_consumers = atoi(argv[2]);
    num_of_producers = atoi(argv[2]);
    
    /*
        POSIX thread initialization.
        setting flag to PTHREAD_SCOPE_SYSTEM ensure that the threads
        created are kernel-level threads i.e., visible to the
        kernel.
    */	
        
    flags = PTHREAD_SCOPE_SYSTEM;
    pthread_attr_init(&a);
    pthread_attr_setscope(&a, flags);
    
    /*
        Initializing the locks. 
        0 means not locked at the beginning.
        Read tsl.c for more information.
    */		
    
    space_count_lock.locked=0;
    produced_count_lock.locked=0;
    buffer_lock.locked=0;	
    
    /*
        Allocating the memory buffer for consumed_buffer.	
        Basically, the total produced buffer size would be
        number of producers*num.
    */		
    consumed_buffer = (int *) malloc(num_of_producers * num * sizeof(int));
    
    
    /* Initializing the thread id array*/
    producer_thds = (pthread_t *)malloc(num_of_producers * sizeof(pthread_t));
    consumer_thds = (pthread_t *)malloc(num_of_consumers * sizeof(pthread_t));
    
    /*Creating all the producer and consumer threads*/
    for(i=0;i<num_of_producers;i++)
        pthread_create(&producer_thds[i], &a, producer, (void*)num);		
    for(i=0; i<num_of_consumers; i++)
        pthread_create(&consumer_thds[i], &a, consumer, (void*)num);	
        
    /*wait for all producer and consumer threads to complete*/
    for(i=0;i<num_of_producers;i++)
        pthread_join(producer_thds[i], NULL);
    for(i=0;i<num_of_consumers; i++)
        pthread_join(consumer_thds[i], NULL);     

    /*check consumed_buffer */
    for(i=1; i<num_of_producers *num; i++)
    {
        if(consumed_buffer[i]!=consumed_buffer[i-1]+1)
        {
            printf("Result is wrong at index consumed_buffer[%d].\n", i);
            printf("consumed_buffer[%d] is %d.\n", i-1, consumed_buffer[i-1]);
            printf("consumed_buffer[%d] is %d.\n", i, consumed_buffer[i]);
            return -1;
        }
    }
    printf("Result is correct!\n");
    free(consumer_thds);
    free(producer_thds);
    free(consumed_buffer);
    return 0;
}
