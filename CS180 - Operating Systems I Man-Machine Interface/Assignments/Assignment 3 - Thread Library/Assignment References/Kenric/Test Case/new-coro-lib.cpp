/* Start Header
*****************************************************************/
/*!
\file new-coro-lib.cpp
\author Kenric Tan, weiliangkenric.tan, 390001119
\par weiliangkenric.tan\@digipen.edu
\date 04/08/2020
\brief This file contains the implementation of a user thread

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#include <vector>
#include <queue>
#include <stack>
#include <map>
#include "new-coro-lib.h"
#include <iostream>

namespace CORO
{
	constexpr auto MB = 1048576;

	enum ThreadState : int
	{
		NEW_STATE = 0,
		WAIT_STATE,
		READY_STATE,
		RUNNING_STATE,
		TERMINATION_STATE,
		TEST_STATE
	};

	typedef void* (*thd_function_t)(void*);

	struct TCB //Thread Control block
	{
		ThreadID _tid;
		ThreadID _currID;
		ThreadState _state;
		void* _sp;
		char* _stack;
		thd_function_t _func;
		void* _parameters;
		void* _retVal;
		~TCB()
		{
			delete[] _stack;
		}
	};

	static ThreadID currID;
	std::map<ThreadID, TCB> TCB_List;
	std::stack<ThreadID> New_thds;
	std::queue<ThreadID> Ready_thds;
	std::map<ThreadID, ThreadID> Waiting_thds;

/*============================================================================*/
//variables for push and pull functions

	//queue to store value to be pushed and pulled
	std::queue<void*> puller_queue;
	//variable to store push
	void** push_storage;
	//variable for puller id
	ThreadID pullerID;
/*============================================================================*/

/******************************************************************************/
/*!
	\fn    void thd_init()

	\brief Assigning a new thread into running state

*/
/******************************************************************************/
	void thd_init()
	{
		//setting current thread a new thread
		currID = new_thd(nullptr, nullptr);
		TCB_List[currID]._currID = currID;
		TCB_List[currID]._state = ThreadState::RUNNING_STATE;
		//removing thread from new thread as it is set to the running state
		New_thds.pop();
	}

/******************************************************************************/
/*!
	\fn    ThreadID new_thd(thread_func_t _func, void* parameters)

	\brief Creating a new thread

*/
/******************************************************************************/
	ThreadID new_thd(thd_function_t _func, void* parameters)
	{
		static ThreadID id = 0;
		//creating a new thread
		TCB thread{
			id,
			currID,
			NEW_STATE,
			nullptr,
			nullptr,
			_func,
			parameters,
			nullptr
		};
		TCB_List[id] = thread;
		//pushing the id into the new thread list
		New_thds.push(id);
		//increment id every new thread
		return id++;
	}

/******************************************************************************/
/*!
	\fn    void thread_exit(void* retVal)

	\brief Terminate a thread after it finish execution

*/
/******************************************************************************/
	void thread_exit(void* retVal)
	{
		//checking if there are any thread on the waiting list
		if (Waiting_thds.find(currID) != Waiting_thds.end())
		{
			//set waiting threads to ready threads
			TCB_List[Waiting_thds[currID]]._state = ThreadState::READY_STATE;
			Ready_thds.push(Waiting_thds[currID]);
		}

		//when thread exit , set thread to termination state
		TCB_List[currID]._state = ThreadState::TERMINATION_STATE;
		//assigning "current" return value
		TCB_List[currID]._retVal = retVal;
		// Yield CPU for another thread
		thd_yield();
	}

/******************************************************************************/
/*!
	\fn    	int wait_thread(ThreadID id, void** value)

	\brief  Waits for a thread to be completed and obtain the return value of the thread

*/
/******************************************************************************/
	int wait_thread(ThreadID id, void** value)
	{

		//assigning return_value to value 
		if (value)
			*value = TCB_List[id]._retVal;

		// find whether thread exist
		auto start = TCB_List.find(id);
		if (start != TCB_List.end() && start->second._state != ThreadState::TERMINATION_STATE)
		{
			// create a new waiting thread in the waiting thread list with the current thread
			Waiting_thds[id] = currID;
			// set the current thread to wait
			TCB_List[currID]._state = ThreadState::WAIT_STATE;
			// Yield CPU for another thread
			thd_yield();

			//assigning return_value to value 
			if (value)
				*value = TCB_List[id]._retVal;

			//assigning thread to termination state
			TCB_List[id]._state = ThreadState::TERMINATION_STATE;
			//removing the thread from our threadlist
			TCB_List.erase(id);

			return WAIT_SUCCESSFUL;
		}
		else
			return NO_THREAD_FOUND;
	}

/******************************************************************************/
/*!
	\fn    	void thd_yield()

	\brief	Causes the current thread to yield the CPU for another thread to be

*/
/******************************************************************************/
	void thd_yield()
	{
/*============================================================================*/
//Saving context, storing stack pointer into current running TCB's stack pointer

		asm volatile(
			"push %%rax \n\t"
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
			:::);

		asm volatile("movq %%rsp, %0\n\t"
			: "+m"
			(TCB_List[currID]._sp));
/*============================================================================*/

		//check if ready list is not empty
		if (!Ready_thds.empty())
		{

			if (TCB_List[currID]._state != ThreadState::TERMINATION_STATE &&
				TCB_List[currID]._state != ThreadState::WAIT_STATE)
			{
				// since it is not terminated or wait , it shows that the thread is ready to run 
				Ready_thds.push(currID);
				TCB_List[currID]._state = ThreadState::READY_STATE;
			}

			//First in the ready thread queue would be assign to the running thread
			currID = Ready_thds.front();
			//remove the ready thread since it is assigned to the running thread
			Ready_thds.pop();
			//assign thread state to running state
			TCB_List[currID]._state = ThreadState::RUNNING_STATE;

		}
		//check if the new thread list is not empty
		else if (!New_thds.empty())
		{
			if (TCB_List[currID]._state != ThreadState::TERMINATION_STATE &&
				TCB_List[currID]._state != ThreadState::WAIT_STATE)
			{
				// since it is not terminated or wait , it shows that the thread is ready to run 
				Ready_thds.push(currID);
				TCB_List[currID]._state = ThreadState::READY_STATE;
			}

			//First in the new thread queue would be assign to the running thread
			currID = New_thds.top();
			//remove the new thread from new thread list since it is executed(running state)
			New_thds.pop();
			//assign thread state to running state
			TCB_List[currID]._state = ThreadState::RUNNING_STATE;

			// Set stack and stack pointer of the running thread
			TCB_List[currID]._stack = new char[MB];
			TCB_List[currID]._sp = (char*)TCB_List[currID]._stack + MB;

			//saving the stack pointer (sp)
			asm volatile("movq %0, %%rsp\n\t"
				:: "m"
				(TCB_List[currID]._sp));

			//Set return value of the running thread
			TCB_List[currID]._retVal = TCB_List[currID]._func(TCB_List[currID]._parameters);
			thread_exit(TCB_List[currID]._retVal);
		}

/*============================================================================*/
// Restoring context, Restoring stack pointer into current running TCB's stack pointer
		asm volatile("movq %0, %%rsp \n\t"
			:: "m"
			(TCB_List[currID]._sp));

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
			:::);
/*============================================================================*/
	}

/******************************************************************************/
/*!
	\fn    	void push_value(void* push_value)

	\brief	Push value inputted into push queue

*/
/******************************************************************************/
	void push_value(void* push_value)
	{
		//check whether puller queue is empty
		if (!puller_queue.empty())
		{
			//if it is not empty, save the push_value into our push storage
			*push_storage = push_value;
			//assign the current thread into ready state
			TCB_List[currID]._state = ThreadState::READY_STATE;
			// Yield CPU for another thread
			thd_yield();
		}
		else
		{
			//if puller queue is empty
			std::cerr << "Queue is empty" << std::endl;
		}
	}

/******************************************************************************/
/*!
	\fn    	void pull_value(void** pull_value)

	\brief	Retrives value from pull queue

*/
/******************************************************************************/
	void pull_value(void** pull_value)
	{
		//storing address of storing variable into our variable push_storage
		push_storage = pull_value;
		//moving puller into puller queue
		puller_queue.push(push_storage);
		// Yield CPU for another thread
		thd_yield();
	}
}