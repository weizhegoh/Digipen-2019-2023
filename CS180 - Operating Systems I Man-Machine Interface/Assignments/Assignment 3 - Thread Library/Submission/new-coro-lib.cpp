/* Start Header
*****************************************************************/
/*!
\file   new-coro-lib.cpp
\author Goh Wei Zhe, weizhe.goh, 440000119
\par    email: weizhe.goh\@digipen.edu
\date   October 27, 2020 
\brief  To learn and understand context switching at some specific 
        level using x86/64 assembly programming. Understand anology
        between the management of threads and the management of 
        processes. 

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/
#include "new-coro-lib.h"

#include <map>
#include <queue>
#include <stack>
#include <iostream>

//max length of a queue
#define SIZE 1048576         

/******************************************************************************/
/*!
    Namespace CORO 
*/
/******************************************************************************/
namespace CORO
{
    int threadCount = 0;
    ThreadID currentID = 0;
    
    enum ThreadState : int
    {
        newState = 0,
        readyState,
        runningState,
        waitingState,
        terminatedState
    };
    
    /**************************************************************************/
    /*!
        Thread Control Block (TCB) struct
    */
    /**************************************************************************/
    struct TCB
    {
        ThreadID _id;        
        void *_stackPtr, *_stackBasePtr, *_param, *_retVal;
        void*(*_thd_function_t)(void*);
        ThreadState _state;
        
        TCB():_id(threadCount),_stackPtr(nullptr), _stackBasePtr(nullptr),
              _param(nullptr), _retVal(nullptr),_thd_function_t(nullptr),
              _state(newState){}
        
        ~TCB(){delete [] (char*)_stackBasePtr;}
    };
    
    //Store IDs of ready threads - FIFO
    std::queue<ThreadID> readyQueue;
    
    //Store push values in pushQueue
    std::queue<void*> pushQueue;
    
    //Stores all thread information
    std::map<ThreadID, TCB> threadMap;
    
    //MAP Second waiting thread ID waiting to first thread ID 
    std::map<ThreadID, ThreadID> waitingThreadMap;
    
    //stack for new threads - LIFO
    std::stack<ThreadID> newThreadStack;

    /**************************************************************************/
    /*!
    \fn     void thd_init();

    \brief  Initialize a thread to running state 
    */
    /**************************************************************************/
    void thd_init()
    {
        currentID = new_thd(nullptr, nullptr);
        threadMap[currentID]._state = runningState;
        
        newThreadStack.pop();
    }

    /**************************************************************************/
    /*!
    \fn     ThreadID new_thd(void *(*thd_function_t )(void *), void* param )

    \brief  Creates a new thread when called. 
    */
    /**************************************************************************/
    ThreadID new_thd( void*(*thd_function_t)(void*), void *param)
    {
        TCB newThread;
        newThread._thd_function_t = thd_function_t;
        newThread._param = param;
        threadMap[newThread._id] = newThread;
        newThreadStack.push(newThread._id);
        ++threadCount;

        return newThread._id;
    }

    /**************************************************************************/
    /*!
    \fn     void thread_exit(void *ret_value);

    \brief  Exits the thread with a ret_value as a return value of the thread. 
            All threads to call this function except for primary thread. 

            Another method to exit is to call return in the thread starting 
            function. i.e the function passed as a parameter into new_thd 
    */
    /**************************************************************************/
    void thread_exit(void *ret_value)
    {
        //If found, set waiting thread to ready        
        if(waitingThreadMap.find(currentID) != waitingThreadMap.end())
        {
            readyQueue.push(waitingThreadMap[currentID]);
            threadMap[currentID]._state = readyState;
        }
        
        //Terminate thread and set return value 
        threadMap[currentID]._state = terminatedState;
        threadMap[currentID]._retVal = ret_value;
        
        //thread yield
        thd_yield();
    }

   /***************************************************************************/
    /*!
    \fn     int wait_thread(ThreadID id, void **value);

    \brief  Wait for the thread to be completed and obtain the return value of
            the thread. Id identifies the thread to be waited upon and the value
            should be changed to the return value of the thread after 
            wait_thread successfully complete. 
    */
    /**************************************************************************/
    int wait_thread(ThreadID id, void **value)
    {        
        //if thread exist
        if(threadMap.find(id) != threadMap.end())
        {
            waitingThreadMap[id] = currentID;
            
            //if thread not in terminatedState
            if(threadMap[id]._state !=terminatedState)
            {
                //set thread to wait for current thread 
                threadMap[currentID]._state = waitingState;
                thd_yield();
            }
            
            waitingThreadMap.erase(id);
            
            if(value)
                *value = threadMap[id]._retVal;

            //Remove thread from threadMap 
            threadMap.erase(id);
            
            return WAIT_SUCCESSFUL;
        }
        else
            return NO_THREAD_FOUND;
    }

    /**************************************************************************/
    /*!
    \fn     void thd_yield()

    \brief  Cause current thread to yield the CPU for another thread (if any) to
            be scheduled. 
    */
    /**************************************************************************/
    void thd_yield()
    {
        //context saving
        asm volatile("push %%rax \n\t"
                     "push %%rbx \n\t"
                     "push %%rcx \n\t"
                     "push %%rdx \n\t"
                     "push %%rsi \n\t"
                     "push %%rdi \n\t"
                     "push %%rbp \n\t"
                     "push %%r8 \n\t"
                     "push %%r9 \n\t"
                     "push %%r10 \n\t"
                     "push %%r11 \n\t"
                     "push %%r12 \n\t"
                     "push %%r13 \n\t"
                     "push %%r14 \n\t"
                     "push %%r15 \n\t"
                     ::: "rsp");
        
        //save stack pointer
        asm volatile("movq %%rsp, %0\n\t"
                     : "+m"
                     (threadMap[currentID]._stackPtr));
                     
        if(threadMap[currentID]._state != terminatedState &&
           threadMap[currentID]._state != waitingState)
        {
            threadMap[currentID]._state = readyState;
            readyQueue.push(currentID);
        }
           
        if(!newThreadStack.empty())
        {
            currentID = newThreadStack.top();
            newThreadStack.pop();
            threadMap[currentID]._state = runningState;
            
            //stack allocation
            threadMap[currentID]._stackBasePtr = new char[SIZE];
            
            threadMap[currentID]._stackPtr = 
            (char*)threadMap[currentID]._stackBasePtr + SIZE;
            
            //load rsp
            asm volatile("movq %0, %%rsp\n\t"
                          :: "m"
                          (threadMap[currentID]._stackPtr));
                          
            //run and exit the thread
            threadMap[currentID]._retVal = 
            threadMap[currentID]._thd_function_t(threadMap[currentID]._param);
            
            thread_exit(threadMap[currentID]._retVal);
        }
        else 
        {
            currentID = readyQueue.front();
            readyQueue.pop();
            
            threadMap[currentID]._state = runningState;
        }
        
        //load stack pointer and pop it
        asm volatile("movq %0, %%rsp \n\t"
                     :: "m"
                     (threadMap[currentID]._stackPtr));
         
        asm volatile("pop %%r15 \n\t"
                     "pop %%r14 \n\t"
                     "pop %%r13 \n\t"
                     "pop %%r12 \n\t"
                     "pop %%r11 \n\t"
                     "pop %%r10 \n\t"
                     "pop %%r9 \n\t"
                     "pop %%r8 \n\t"
                     "pop %%rbp \n\t"
                     "pop %%rdi \n\t"
                     "pop %%rsi \n\t"
                     "pop %%rdx \n\t"
                     "pop %%rcx \n\t"
                     "pop %%rbx \n\t"
                     "pop %%rax \n\t"
                     ::: "rsp");
    }
    
    /**************************************************************************/
    /*!
    \fn     void push_value(void *pushed_value);

    \brief  Called by the Pusher to push the value to the Puller.  
    */
    /**************************************************************************/
    void push_value(void *pushed_value)
    {
        pushQueue.push(pushed_value);
        thd_yield();
    }
    
    /**************************************************************************/
    /*!
    \fn     void pull_value(void **pulled_value);

    \brief  Called by the Puller to retrieve one pushed value from the Pusher 
    */
    /**************************************************************************/
    void pull_value(void **pulled_value)
    {
        while(true)
        {
            if(!pushQueue.empty())
            {
                *pulled_value = pushQueue.front();
                pushQueue.pop();
                break;
            }
            else 
                thd_yield();
        }
    }
}