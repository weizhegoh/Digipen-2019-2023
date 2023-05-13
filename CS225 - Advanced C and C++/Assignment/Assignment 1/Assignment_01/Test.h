/******************************************************************************
 * @Author:     Swavek Wlodkowski
 * @Course:     CS 225
 * 
 * This file contains helper code for running tests cases. Do not change it.
 *****************************************************************************/
 
#ifndef _TEST_H_
#define _TEST_H_

#include <exception>
#include <cstdlib>

using Test = void (*)();

void run(Test test)
{
	try
	{
		test();
		std::cout
			<< "Test completed successfully."
			<< std::endl;
	}
	catch (const std::string& exception)
	{
		std::cout
			<< "Test failed. "
			<< exception
			<< std::endl;
	}
	catch (const std::exception& exception)
	{
		std::cout
			<< "Test failed. "
			<< exception.what()
			<< std::endl;
	}
	catch (...)
	{
		std::cout
			<< "Test failed due to an unexpected exception!"
			<< std::endl;
	}
}

#endif
