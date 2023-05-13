#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "new-coro-lib.h"

/* prototype for thread routine */
void *print_message_function ( void *ptr );

/* struct to hold data to be passed to a thread
   this shows how multiple data items can be passed to a thread */
typedef struct str_thdata
{
    int thread_no;  char message[100];
} thdata;

int main()
{
    CORO::ThreadID thread1, thread2;  /* thread variables */
    thdata data1, data2;         /* structs to be passed to threads */

    /* initialize data to pass to thread 1 */
    data1.thread_no = 1;  strcpy(data1.message, "Hello!");

    /* initialize data to pass to thread 2 */
    data2.thread_no = 2;  strcpy(data2.message, "Hi!");

    /* create threads 1 and 2 */
    CORO::thd_init();
    thread1 = CORO::new_thd(print_message_function, (void *) &data1);
    thread2 = CORO::new_thd(print_message_function, (void *) &data2);

    /* Main block now waits for both threads to terminate, before it exits
       If main block exits, both threads exit, even if the threads have not
       finished their work */
    CORO::wait_thread(thread1, nullptr);
    CORO::wait_thread(thread2, nullptr);

    /* exit */
    exit(0);
} /* main() */

/**
 * print_message_function is used as the start routine for the threads used
 * it accepts a void pointer
**/
void *print_message_function ( void *ptr )
{
    thdata *data;
    data = (thdata *) ptr;  /* type cast to a pointer to thdata */

    /* do the work */
    printf("Thread %d says %s \n", data->thread_no, data->message);

    CORO::thread_exit(0); /* exit */
} /* print_message_function ( void *ptr ) */