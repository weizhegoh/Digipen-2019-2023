#include <iostream>
#include "new-coro-lib.h"

void *fib(void *a)
{
    int num = *reinterpret_cast<int *>(a);
    long long first=0;
    long long second=1;
    for(auto i=0; i<num; ++i)
    {
        CORO::push_value(reinterpret_cast<void*>(first));
        int tmp=second;
        second+=first;
        first=tmp;
    }
}

int main()
{
    CORO::ThreadID id;
    CORO::thd_init();
    int num;
    std::cin >> num;
    id = CORO::new_thd(fib, &num);
    for(long long i=0; i<num; ++i)
    {
        long long value;
        CORO::pull_value(reinterpret_cast<void**>(&value));
        std::cout << value << std::endl;
    }
}
