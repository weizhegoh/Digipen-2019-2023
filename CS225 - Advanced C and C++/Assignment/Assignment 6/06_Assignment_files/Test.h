#ifndef _TEST_H_
#define _TEST_H_

#include <exception>

using Test = void (*)();

void run(std::size_t& testIndex, Test test)
{
	++testIndex;
	try
	{
		test();
		std::cout
			<< "Test "
			<< testIndex
			<< " completed successfully."
			<< std::endl;
	}
	catch (const std::string& exception)
	{
		std::cout
			<< "Test "
			<< testIndex
			<< " failed. "
			<< exception
			<< std::endl;
	}
	catch (const std::exception& exception)
	{
		std::cout
			<< "Test "
			<< testIndex
			<< " failed. "
			<< exception.what()
			<< std::endl;
	}
	catch (...)
	{
		std::cout
			<< "Test "
			<< testIndex
			<< " failed due to an unexpected exception!"
			<< std::endl;
	}
}

#endif
