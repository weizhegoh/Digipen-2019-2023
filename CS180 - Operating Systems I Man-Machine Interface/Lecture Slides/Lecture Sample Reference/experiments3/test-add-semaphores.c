#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>


int count = 0;

sem_t common_sem;

void *add_many(void *arg)
{
    int num = (int)arg;
    int i;
    for(i=0;i<num; i++)
    {		
        sem_wait(&common_sem);
        count++;		
        sem_post(&common_sem);
    }
}

int main(int argc, char**argv)
{
    int num, flags;
    int seq_or_thd;
    pthread_attr_t a;
    pthread_t tid[2]; //[0] - producer [1] - reader    
    
    if(argc!=3)
    {
        fprintf(stderr, "Usage: %s <num_of_times> <0: sequential OR 1: threads>\n", argv[0]);
        exit(-1);
    }
    
    num = atoi(argv[1]);
    seq_or_thd = atoi(argv[2]);
    
    sem_init(&common_sem, 0, 1);
    
    if(seq_or_thd)
    {          
        flags = PTHREAD_SCOPE_SYSTEM;
        pthread_attr_init(&a);
        pthread_attr_setscope(&a, flags);

        
        pthread_create(&tid[0], &a, add_many, (void*)num);	
        pthread_create(&tid[1], &a, add_many, (void*)num);	
            
        pthread_join(tid[0], NULL);
        pthread_join(tid[1], NULL);       
    }
    else
    {
        //Sequential example
        add_many((void*)num);
        add_many((void*)num);
    }
    
    printf("count is %d\n", count);
}
