#ifndef DATA_H
#define DATA_H

#include <sstream>
#include <string>
#include <memory> // shared_ptr

namespace AI
{
	using Log = std::ostringstream;

	// States of the behavior tree nodes 
	enum State { Undefined = 0, Failure, Success, Running};

	// To convert states to text
	static const char* STATES[] = { "Undefined", "Failure", "Success", "Running" };

	// Base class for a condition or an action tasks
	class Task
	{
	protected:
		std::string id;
		State state;
		
	public:
		Task(std::string id = "", State state = State::Undefined)
			: id{ id }, state{ state }
		{
		}

		virtual ~Task()
		{
		}

		virtual Task& operator()(Log* log = nullptr, std::string level = "")
		{
			if (log) *log << level << "Task" << '(' << id << ')' << std::endl;

			if (log) *log << level << "L " << STATES[state] << std::endl;
			return *this;
		}

		std::string getId() const
		{
			return id;
		}

		State getState() const
		{
			return state;
		}
	};

	using SMART = std::shared_ptr<Task>; // Smart pointer

} // end namespace

#endif