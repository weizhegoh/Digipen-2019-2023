/* Start Header
*****************************************************************/
/*!
\file new-coro-lib.cpp
\author Tiong Jun Ming Jerome, j.tiong, 390005919
\par j.tiong\@digipen.edu
\date Aug 2 2020
\brief
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/
#include "new-coro-lib.h"
#include <map>
#include <stack>
#include <queue>
#define SIZE 1048576

namespace CORO
{
	// Thread Counter
	int count = 0;
	// Current running thread
	ThreadID curr_id = 0;
	
	// Thread states
	enum State : int
	{
		readyState = 0,
		runningState,
		waitingState,
		terminatedState
	};
	
	// TCB data struct
	struct TCB
	{
		TCB(): _id(count), _function(nullptr), _param(nullptr), 
			   _stackPtr(nullptr), _stackBasePtr(nullptr), _value(nullptr), 
			   _state(readyState)
		{
		}
		
		~TCB()
		{
			delete[] (char*)_stackBasePtr;
		}
		
		ThreadID _id;
		void* (*_function)(void*);
		void* _param;
		void* _stackPtr;
		void* _stackBasePtr;
		void* _value;
		State _state;
	};
	
	// Container for all the Threads
	std::map<ThreadID, TCB> ThreadList;
	// New threads using stack for LIFO
	std::stack<ThreadID> NewList;
	// Ready threads using queue for FIFO
	std::queue<ThreadID> ReadyList;
	// Second ThreadID waiting for first ThreadID
	std::map<ThreadID, ThreadID> WaitingList;
	// Store Pushed values
	std::queue<void*> PushedValues;
	
	// Initialize the thread library
	void thd_init()
	{
		curr_id = new_thd(nullptr, nullptr);
		
		ThreadList[curr_id]._state = runningState;
		
		NewList.pop();
	}
	
	// Creates a new thread
	ThreadID new_thd( void*(*thd_function_t)(void*), void* param)
	{
		TCB temp;
		temp._function = thd_function_t;
		temp._param = param;
		ThreadList[temp._id] = temp;
		NewList.push(temp._id);
		count++;
		return temp._id;
	}
	
	// Exits the thread with ret_value
	void thread_exit(void *ret_value)
	{
		if (WaitingList.find(curr_id) != WaitingList.end())
		{
			ReadyList.push(WaitingList[curr_id]);
		}
		ThreadList[curr_id]._state = terminatedState;
		ThreadList[curr_id]._value = ret_value;
		
		thd_yield();
	}
	
	// Wait for Thread to be completed
	int wait_thread(ThreadID id, void **value)
	{
		// Check if thread exists
		if(ThreadList.find(id) != ThreadList.end())
		{
			WaitingList[id] = curr_id;
			// If thread is not already terminated
			if (ThreadList[id]._state != terminatedState)
			{
				ThreadList[curr_id]._state = waitingState;
				thd_yield();
			}
			WaitingList.erase(id);
			// Get the return value
			if(value)
				*value = ThreadList[id]._value;
			
			// Remove the Thread
			ThreadList.erase(id);
			
			return WAIT_SUCCESSFUL;
		}
		else
			return NO_THREAD_FOUND;
	}
	
	// Yield the CPU for another Thread
	void thd_yield()
	{
		// Context Saving
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
		
		asm volatile("movq %%rsp, %0\n\t"
                     : "+m"
                     (ThreadList[curr_id]._stackPtr));
					 
		if(ThreadList[curr_id]._state == runningState ||
		   ThreadList[curr_id]._state == readyState)
		{
			ThreadList[curr_id]._state = readyState;
			ReadyList.push(curr_id);
		}
		
		if (!NewList.empty())
		{
			curr_id = NewList.top();
			NewList.pop();
			ThreadList[curr_id]._state = runningState;
			
			ThreadList[curr_id]._stackBasePtr = new char[SIZE];
			ThreadList[curr_id]._stackPtr = (char*)ThreadList[curr_id]._stackBasePtr + SIZE;
			
			asm volatile("movq %0, %%rsp \n\t"
						 :: "m"
						 (ThreadList[curr_id]._stackPtr));
						
			ThreadList[curr_id]._value = ThreadList[curr_id]._function(ThreadList[curr_id]._param);
			thread_exit(ThreadList[curr_id]._value);
		}
		else
		{
			curr_id = ReadyList.front();
			ReadyList.pop();
			
			ThreadList[curr_id]._state = runningState;
			
			asm volatile("movq %0, %%rsp \n\t"
						 :: "m"
						 (ThreadList[curr_id]._stackPtr));
		}
		
		// pop stack pointer
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
	
	void push_value(void* value)
	{
		PushedValues.push(value);
        thd_yield();
	}

	void pull_value(void** value)
	{
		// Waits until value is pushed into the deque
        while(true)
        {
            if(!PushedValues.empty())
            {
                *value = PushedValues.front();
                PushedValues.pop();
                break;
            }
            else
                thd_yield();
		}
	}
}