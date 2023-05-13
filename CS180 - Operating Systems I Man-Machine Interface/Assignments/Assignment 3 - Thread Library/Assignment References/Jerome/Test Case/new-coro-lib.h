#ifndef NEW_CORO_LIB_H
#define NEW_CORO_LIB_H
namespace CORO
{
using ThreadID = unsigned;
void thd_init();
ThreadID new_thd( void*(*)(void*), void *);
void thread_exit(void *);
int wait_thread(ThreadID id, void **value);
void thd_yield();
void push_value(void*);
void pull_value(void**);
const int WAIT_SUCCESSFUL = 0;
const int NO_THREAD_FOUND = -1;
enum ThreadState : int;
}
#endif