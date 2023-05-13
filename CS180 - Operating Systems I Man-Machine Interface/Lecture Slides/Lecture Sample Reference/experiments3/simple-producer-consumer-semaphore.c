#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#define BUFFER_SIZE 256

/*
    Global Variables:
    1. Of course not supposed to have global variables in any "real" code.
    2. For purposes of demo only.
    3. produced_buffer is the "shared buffer" between all producers and consumers.
       Producers write into it, Consumers read from it.
    4. Three semaphores:
        a.  empty - 0 when buffer is empty, BUFFER_SIZE when buffer is full.
                    Initialized to 0. Because buffer is empty at the beginning.
        b.  buffer_mutex - ensure mutual exclusion in access to shared buffer.
        c.  full - 0 when buffer is full, BUFFER_SIZE when buffer is empty.
                   Initialized to BUFFER_SIZE. Because buffer is full at the beginning
    5. consumed_buffer - a shared buffer among the consumers for storing whatever 
                         they obtained from the buffer.
*/
int produced_buffer[BUFFER_SIZE];
sem_t empty, buffer_mutex, full;
int *consumed_buffer;


/*
    Consumer thread function.
    1. out - the index in the produced_buffer that can be consumed.
    2. consumed_in - the index in the consumed_buffer that can be written to.
    3. out and consumed_in are "shared" by all consumer threads. 
       Note the static declaration.	
    4. consumed, num and i are local variables. num is the number of times
       the loop will iterate.
       
    Refer to slides for explanation of the consumer semaphore solution.
*/

void *consumer(void *arg)
{
    static unsigned char out=0;
    static unsigned int consumed_in = 0;
    int consumed;
    int num = (int) arg;
    int i;
    
    for(i=0; i< num ; i++)
    {
        sem_wait(&empty);
        
        sem_wait(&buffer_mutex);
        consumed = produced_buffer[out++];			
        consumed_buffer[consumed_in++] = consumed;	
        sem_post(&buffer_mutex);		
        sem_post(&full);
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
    Refer to slides for explanation of the producer semaphore solution.
*/

void *producer(void *arg)
{
    static unsigned char in = 0;
    static int produced_num = 0;
    int num = (int) arg;
    int i =0;
    
    for(i=0; i< num ; i++)
    {
        sem_wait(&full);
        sem_wait(&buffer_mutex);
        produced_buffer[in++] = ++produced_num;			
        sem_post(&buffer_mutex);
        sem_post(&empty);
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
        fprintf(stderr, "Usage: %s <num_of_times> <num of consumer-producer-threads>\n", argv[0]);
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
        Initializing the sempahores. 
        prototype of sem_init:
            sem_init(address of semaphore variable, flag, initial_value);
        The flag when 1 indicates that this semaphores can be
        shared with other processes. For this demo, we only need it 
        to 0.
    */		
    
    sem_init(&full, 0, BUFFER_SIZE);
    sem_init(&empty, 0, 0);
    sem_init(&buffer_mutex, 0, 1);	
    
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
