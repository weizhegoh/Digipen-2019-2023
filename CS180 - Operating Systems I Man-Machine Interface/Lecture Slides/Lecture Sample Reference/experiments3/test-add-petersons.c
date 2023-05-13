#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "peterson.h"

int count = 0;

struct num_and_thread_id
{
    int num;
    int id;
};

void *add_many(void *arg)
{
    struct num_and_thread_id *s_pointer = (struct num_and_thread_id *) arg;
    int i;
    for(i=0;i<s_pointer->num; i++)
    {
        get_mutex(s_pointer->id);
        count++;
        release_mutex(s_pointer->id);
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
    
    if(seq_or_thd)
    {
        //Threads
        struct num_and_thread_id param, param2;
        
        flags = PTHREAD_SCOPE_SYSTEM;
        pthread_attr_init(&a);
        pthread_attr_setscope(&a, flags);

        param.num = num;
        param.id = 0;
        
        pthread_create(&tid[0], &a, add_many, (void*)&param);	        
        
        param2.num = num;
        param2.id = 1;
        
        pthread_create(&tid[1], &a, add_many, (void*)&param2);	
        pthread_join(tid[0], NULL);
        pthread_join(tid[1], NULL);       
    }
    else
    {
        struct num_and_thread_id param;
        param.num = num;
        param.id = 0;
        //Sequential example
        add_many((void*)&param);
        add_many((void*)&param);
    }
    
    printf("count is %d\n", count);
}
