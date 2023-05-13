#include <windows.h>
#include <stdio.h>
#include "new-coro-lib.h"


int main()
{
    CORO::thd_init();
    CORO::thd_yield();
    printf("This should print\n");
}