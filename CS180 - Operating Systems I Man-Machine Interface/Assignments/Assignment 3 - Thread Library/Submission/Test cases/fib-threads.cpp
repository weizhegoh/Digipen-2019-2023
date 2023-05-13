#include <cstdio>
#include <cstdlib>
#include "new-coro-lib.h"

/*
	This program computes the fibonacci series, where F(0)=0 and F(1)=1.

	For n>1, F(n) = F(n-1) + F(n-2)

	Taken and modified from midterm. Instead of forking, we create new threads
	to compute F(n-1) and F(n-2) that we need.

	This test case tests the case when threads create more threads.

	Sample outputs:

	C:\...>fib-threads.exe 0
	fib(0) is 0

	C:\...>fib-threads.exe 1
	fib(1) is 1

	C:\...>fib-threads.exe 2
	fib(2) is 1

	C:\...>fib-threads.exe 5
	fib(5) is 5

	C:\...>fib-threads.exe 6
	fib(6) is 8

	C:\...>fib-threads.exe 7
	fib(7) is 13

	C:\...>fib-threads.exe 10
	fib(10) is 55

	C:\...>fib-threads.exe 12
	fib(12) is 144

	C:\...>fib-threads.exe 13
	fib(13) is 233

	C:\...>fib-threads.exe 14
	fib(14) is 377

	You should be able to run up to fib-threads
*/

void *fib_thd(void *param)
{
    long num;
    num = reinterpret_cast<long>(param);
    if (num > 1)
    {
        CORO::ThreadID t1, t2;
        long t1_res, t2_res;

        t1 = CORO::new_thd(fib_thd, (void*)(num-1));
        t2 = CORO::new_thd(fib_thd, (void*)(num-2));
        CORO::wait_thread(t1, (void**) &t1_res);
        CORO::wait_thread(t2, (void**) &t2_res);
        CORO::thread_exit((void*)(t1_res+t2_res));
    }
    else
        return (void*)num;
}

int main(int argc, char **argv)
{
    long num;
    CORO::ThreadID my_thd;
    int result;

    if(argc != 2)
    {
        printf("%s <number>", argv[0]);
        return 1;
    }

    num = atoi(argv[1]);

    if(num < 0)
    {
        printf("%s <number>", argv[0]);
        printf("number must be positive!");
        return 1;
    }

    CORO::thd_init();
    my_thd = CORO::new_thd(fib_thd, (void*)num);
    CORO::wait_thread(my_thd,(void**) &result);
    printf("fib(%d) is %d\n", num,   result);
}










































































































































































