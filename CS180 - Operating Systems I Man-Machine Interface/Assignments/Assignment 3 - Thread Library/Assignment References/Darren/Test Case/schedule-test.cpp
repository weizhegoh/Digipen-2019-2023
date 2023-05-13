#include <cstdio>
#include <cstdlib>
#include "new-coro-lib.h"

void *thd1(void *param )
{
    int *thd2_id, thd2_res;
    thd2_id = (int *) param;
    CORO::thd_yield();
    printf("This is thread 1\n");
    CORO::wait_thread(*thd2_id, (void**)(&thd2_res));
    CORO::thd_yield();
    printf("Thd3 returned %d\n", thd2_res);
    CORO::thread_exit((void *) (thd2_res+1));
    return nullptr;
}


void *thd2(void *param )
{
    int *thd3_id, thd3_res;
    thd3_id = (int *) param;
    printf("This is thread 2\n");
    CORO::wait_thread(*thd3_id, (void**)&thd3_res);
    CORO::thd_yield();
    printf("Thd3 returned %d\n", thd3_res);
    CORO::thread_exit((void *) (thd3_res+1));
    return nullptr;
}


void *thd3(void *param )
{
    int a=1024;
    CORO::thd_yield();
    printf("This is thread 3\n");
    CORO::thread_exit((void*)a);
    return nullptr;
}


int main()
{
    CORO::ThreadID thd1_id, thd2_id, thd3_id;
    int *thd1_res;
    CORO::thd_init();
    thd3_id =CORO::new_thd(thd3, NULL);
    thd2_id =CORO::new_thd(thd2, (void *)&thd3_id);
    thd1_id =CORO::new_thd(thd1, (void *)&thd2_id);

    CORO::wait_thread(thd1_id, (void**)&thd1_res);
    printf("Thd1 returned %d\n", thd1_res);
}


















































































































































































































































