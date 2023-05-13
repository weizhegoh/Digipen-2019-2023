#include <stdio.h>
#include "new-coro-lib.h"

void *spin1(void *a)
{
    int i;
    for(i=0; i< 20; i++)
    {
        printf("SPIN1\n");
        if((i+1)%4==0)
            CORO::thd_yield();
    }
    return NULL;
}

void* spin2(void *a)
{
    int i;
    for(i=0; i< 20; i++)
    {
        printf("SPIN2\n");
        if((i+1)%4==0)
            CORO::thd_yield();
    }
    return NULL;
}


int main()
{
    CORO::ThreadID id;
    CORO::thd_init();
    id = CORO::new_thd(spin2, NULL);
    spin1(NULL);
}
