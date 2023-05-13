/* Start Header
*****************************************************************/
/*!
\file <new-coro-lib.cpp>
\author <Lim Yong Kiang, Darren , lim.y, 390006119>
\par <lim.y\@digipen.edu>
\date <Aug 11, 2020>
\brief
      This file contains the code for a new coro libary.
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/
#include "new-coro-lib.h"
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define SIZE1M 1048576

namespace CORO
{
    enum ThreadState : int // Enum for the list of thread states
    {
        NEW = 0,
        READY,
        RUNNING,
        WAITING,
        TERMINATED
    };

    ThreadID _counter = 0; // Counter of threads
    ThreadID _curr = 0;    // Current thread number

    struct TCB
    {
        ThreadID thd_ID;          // The ID of the thread.
        ThreadID wait_ID;         // The ID of the thread that it is waiting.
        void *sp;                 // stack pointer
        void *bsp;                // base stack ptr
        void *params;             // params for the function pointer
        void *ret_val;            // Return Value
        void *(*funcPtr)(void *); // Function Pointer
        ThreadState curr_state;   // Current state

        // Default contructor that initializes the ID of the Thread
        TCB()
            : thd_ID(_counter++), wait_ID(0), sp(nullptr), bsp(nullptr),
            params(nullptr), ret_val(nullptr),
            funcPtr(nullptr), curr_state(NEW) {}
        // Conversion constructor used to make new threads.
        TCB(void *(*_functionPtr)(void *), void *_param)
            : thd_ID(_counter++), wait_ID(0), sp(nullptr), bsp(nullptr),
            params(_param), ret_val(nullptr),
            funcPtr(_functionPtr), curr_state(NEW) {}
    };
    std::queue<ThreadID> readyQueue;        // A queue with all the threads that are in Ready which are FCFS manner.
    std::stack<ThreadID> newStack;          // A stack of Threads which are LIFO manner.
    std::map<ThreadID, TCB *> threadsMap;   // A map with all the threads created
    std::vector<ThreadID> terminatedVector; // Vector with all the ID of the terminated threads
    std::deque<void*> valuesDeque;                 // For values to push and pop

    // Initialize the base thread
    void thd_init()
    {
        // Create base thread
        TCB *_temp = new TCB();

        // Set state to running
        _temp->curr_state = RUNNING;
        threadsMap[_temp->thd_ID] = _temp;
        _curr = _temp->thd_ID;
    }

    // Function to make a new thread
    ThreadID new_thd(void *(*_functionPtr)(void *), void *_param)
    {
        TCB *_temp = new TCB(_functionPtr, _param);

        // Allocating space for the stack
        _temp->bsp = new char[SIZE1M];
        _temp->sp = (char *)_temp->bsp + SIZE1M;

        // Add to new threads
        newStack.push(_temp->thd_ID);
        // Add to map of threads
        threadsMap[_temp->thd_ID] = _temp;

        return _temp->thd_ID;
    }

    // Stop running current thread and run the next thread.
    // If the ready queue is not empty, always run the threads in the ready queue.
    // If the ready queue is empty, we will run a newly created thread.
    void thd_yield()
    {
        ThreadID _next = 0;
        // Context saving

        asm volatile("push %%rbx\n\t"
            "push %%rbp\n\t"
            "push %%r8\n\t"
            "push %%r9\n\t"
            "push %%r10\n\t"
            "push %%r11\n\t"
            "push %%r12\n\t"
            "push %%r13\n\t"
            "push %%r14\n\t"
            "push %%r15\n\t"
            "push %%rdi\n\t"
            "push %%rsi\n\t"
            "push %%rdx\n\t"
            "push %%rax\n\t"
            :::);
            
        asm volatile("movq %%rsp, %0"
            : "+m"(threadsMap[_curr]->sp));

        // If the ready queue is not empty, always run the threads in the ready queue.
        if (!readyQueue.empty())
        {
            // Set next as thread in the front of the queue (FCFS)
            _next = readyQueue.front();
            readyQueue.pop();

            // Current thread changed to READY state if RUNNING
            if (threadsMap[_curr]->curr_state == RUNNING)
            {
                threadsMap[_curr]->curr_state = READY;
                readyQueue.push(_curr);
            }

            _curr = _next;
            threadsMap[_curr]->curr_state = RUNNING;

            // Context Loading of new current thread
            asm volatile("movq %0, %%rsp\n\t"
                :
            : "m"(threadsMap[_curr]->sp));

            asm volatile("pop %%rax\n\t"
                "pop %%rdx\n\t"
                "pop %%rsi\n\t"
                "pop %%rdi\n\t"
                "pop %%r15\n\t"
                "pop %%r14\n\t"
                "pop %%r13\n\t"
                "pop %%r12\n\t"
                "pop %%r11\n\t"
                "pop %%r10\n\t"
                "pop %%r9\n\t"
                "pop %%r8\n\t"
                "pop %%rbp\n\t"
                "pop %%rbx\n\t"
                :::);
        }
        // If the ready queue is empty, we will run a newly created thread.
        else if (!newStack.empty())
        {
            // Set next as thread in the top of the stack (LIFO)
            _next = newStack.top();
            newStack.pop();

            // Current thread changed to READY state if RUNNING
            if (threadsMap[_curr]->curr_state == RUNNING)
            {
                threadsMap[_curr]->curr_state = READY;
                readyQueue.push(_curr);
            }

            _curr = _next;
            threadsMap[_curr]->curr_state = RUNNING;

            // Move stack pointer
            asm volatile("movq %0, %%rsp\n\t"
                : "+m"(threadsMap[_curr]->sp)
                :
                :);

            // run then exit the thread
            threadsMap[_curr]->ret_val = threadsMap[_curr]->funcPtr(threadsMap[_curr]->params);
            thread_exit(threadsMap[_curr]->ret_val);
        }
    }


    // Function to make the thread wait for a given ID
    int wait_thread(ThreadID id, void **value)
    {
        ThreadID _waitThd = 0; // ID to wait
        bool _waitChange = false; // Boolean check

        // Look in all threads
        for (auto &it : threadsMap)
        {
            // Finds for correct id
            if (it.second->thd_ID == id)
            {
                // Check if current state was TERMINIATED
                if (it.second->curr_state != TERMINATED)
                {
                    threadsMap[_curr]->curr_state = WAITING;
                    threadsMap[_curr]->wait_ID = id;

                    // Wait until the thread that is waiting, terminates
                    do
                    {
                        bool _checker = false;
                        thd_yield();
                        for (auto &termIT : terminatedVector)
                        {
                            if (threadsMap[termIT]->thd_ID == id)
                            {
                                _checker = !_checker;
                                break;
                            }
                        }
                        if (_checker)
                            break;
                    } while (true);

                    threadsMap[_curr]->curr_state = READY;
                }
                if (value)
                {
                    // See if any thread waiting for the current thread
                    for (auto &termIT : terminatedVector)
                    {
                        if (threadsMap[termIT]->thd_ID == id)
                        {
                            // Assign the return value 
                            *value = it.second->ret_val;
                            break;
                        }
                    }
                }
                _waitThd = it.second->thd_ID;
                _waitChange = true;
                break;
            }
        }

        if (_waitChange)
            if (threadsMap[_waitThd]->curr_state == TERMINATED && threadsMap[_waitThd]->bsp)
            {
                // Delete the saved stack
                delete[](char *)(threadsMap[_waitThd]->bsp);
                threadsMap[_waitThd]->bsp = nullptr;

                // Returning WAIT_SUCCESSFUL
                return WAIT_SUCCESSFUL;
            }
        // Returning NO_THREAD_FOUND
        return NO_THREAD_FOUND;
    }

    // Makes a thread TERMINIATED and yield it and run the next thread
    void thread_exit(void *ret_value)
    {
        // Change to Terminated
        threadsMap[_curr]->curr_state = TERMINATED;
        // Assign Return value
        threadsMap[_curr]->ret_val = ret_value;

        // Store Terminated into a vector
        terminatedVector.push_back(_curr);

        // Check if any thread waiting for current thread
        for (auto &it : threadsMap)
            if (it.second->wait_ID == threadsMap[_curr]->thd_ID)
            {
                if (it.second->curr_state != TERMINATED)
                {
                    it.second->curr_state = READY;
                    readyQueue.push(it.first);
                }
                break;
            }
        thd_yield();
    }

    // Helps clear the data of the thread
    struct clearThreads
    {
        ~clearThreads()
        {
            for (auto &it : threadsMap)
                delete it.second;
        }
    } clearThreads;
    
    // Function to push a value into the deque
    void push_value(void*  pushed_value)
    {
        valuesDeque.push_back(pushed_value);
        thd_yield();
    }

    // Function to pull a value fron the deque
    void pull_value(void** pulled_value)
    {
        // Waits until value is pushed into the deque
        while(1)
        {
            if(!valuesDeque.empty())
            {
                *pulled_value = valuesDeque.front();
                valuesDeque.pop_front();
                break;
            }
            else
                thd_yield();
            
        }
    }
} // namespace CORO
