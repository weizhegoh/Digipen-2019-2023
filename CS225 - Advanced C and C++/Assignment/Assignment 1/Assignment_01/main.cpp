/******************************************************************************
 * @Author:     Swavek Wlodkowski
 * @Course:     CS 225
 * @Trimester:  Fall 2020
 * @Assignment: 1
 *
 * This program accepts an index of a test case to execute (1-10; 0 for all)
 * and runs that test. Study this program carefully and write the missing code.
 *****************************************************************************/
#include "MemoryAlloc.h"

#include <iostream>
#include <stdexcept>

#include "Background.h"

// Including headers twice should always work.
#include "Color.h"
#include "Background.h"

namespace
{
#include "Test.h"

	// Default constructor of an automatic variable.
	void test1()
	{
		const cs225::Background background;
		if (background.count() != 0)
		{
			throw std::runtime_error(
				"A default constructor must create Background without any "
				"colors."
			);
		}
	}

	// Default constructors of dynamic variables.
	void test2()
	{
		const cs225::Background* backgrounds = new const cs225::Background[2];

		if (backgrounds[0].count() != 0)
		{
			throw std::runtime_error(
				"A default constructor must create Background without any "
				"colors."
			);
		}

		if (backgrounds[1].count() != 0)
		{
			throw std::runtime_error(
				"A default constructor must create Background without any "
				"colors."
			);
		}

		delete[] backgrounds;
	}

	// Copy constructor of a dynamic variable.
	void test3()
	{
		const cs225::Color colors[] = {
			cs225::Color{0xFF, 0, 0},
			cs225::Color{11, 22, 33},
			cs225::Color{0, 0, 0xFF},
			cs225::Color{0xFF, 0xFF, 0xFF}
		};
		const std::size_t colorCount = sizeof(colors) / sizeof(*colors);
		const cs225::Background* a = new const cs225::Background{
		    colorCount,
		    colors
		};
		const cs225::Background* const b = new cs225::Background{*a};
		delete a;

		if (b->count() != 4)
		{
			throw std::runtime_error(
				"A copy constructor must create Background with 4 colors."
			);
		}

		const cs225::Color color = (*b)[1];
		if ((color.red() != 11) ||
			(color.green() != 22) ||
			(color.blue() != 33)
		)
		{
			throw std::runtime_error(
				"A copy constructor must properly copy original pixels."
			);
		}

		delete b;
	}

	// Copy assignment of a dynamic variable.
	void test4()
	{
		const cs225::Color colors[] = {
			cs225::Color{0xFF, 0, 0},
			cs225::Color{11, 22, 33},
			cs225::Color{0, 0, 0xFF},
			cs225::Color{0xFF, 0xFF, 0xFF}
		};
		const std::size_t colorCount = sizeof(colors) / sizeof(*colors);
		const cs225::Background* a = new const cs225::Background{
		    colorCount,
		    colors
		};
		cs225::Background* b = new cs225::Background;

		*b = *a;

		delete a;

		if (b->count() != 4)
		{
			throw std::runtime_error(
				"A copy constructor must create Background with 4 colors."
			);
		}

		const cs225::Color color = (*b)[1];
		if ((color.red() != 11) ||
			(color.green() != 22) ||
			(color.blue() != 33)
		)
		{
			throw std::runtime_error(
				"A copy constructor must properly copy original pixels."
			);
		}

		delete b;
	}

	// User defined constructor of an automatic variable.
	void test5()
	{
		const cs225::Background* a = nullptr;
		{
			const cs225::Color colors[] = {
				cs225::Color{0xFF, 0, 0},
				cs225::Color{11, 22, 33},
				cs225::Color{0, 0, 0xFF},
				cs225::Color{0xFF, 0xFF, 0xFF}
			};
			const std::size_t colorCount = sizeof(colors) / sizeof(*colors);
			a = new const cs225::Background{colorCount, colors};
		}

		const cs225::Color color = (*a)[1];
		if ((color.red() != 11) ||
			(color.green() != 22) ||
			(color.blue() != 33)
		)
		{
			throw std::runtime_error(
				"A copy constructor must properly copy original pixels."
			);
		}

		delete a;
	}

	// Copy assignment of a dynamic variable.
	void test6()
	{
		if (sizeof(cs225::Background) != 16)
		{
			throw std::runtime_error(
				"The size of Background data type is excessive; "
				"in 64-bit system must be exactly 16 bytes."
			);
		}
	}

	// Avoid default constructor
	void test7()
	{
		if (std::is_trivially_default_constructible_v<cs225::Background>)
		{
			throw std::runtime_error(
				"Background must manage resources; it must have "
				"a default constructor with non-default implementation."
			);
		}
	}

	// Avoid shallow constructor
	void test8()
	{
		if ((!std::is_copy_constructible_v<cs225::Background>) ||
			(std::is_trivially_copy_constructible_v<cs225::Background>)
		)
		{
			throw std::runtime_error(
				"Background must use a deep copy constructor."
			);
		}
	}

	// Avoid shallow assignment
	void test9()
	{
		if ((!std::is_copy_assignable_v<cs225::Background>) ||
			(std::is_trivially_copy_assignable_v<cs225::Background>)
		)
		{
			throw std::runtime_error(
				"Background must use a deep copy assignment."
			);
		}
	}

	// Avoid default destructor
	void test10()
	{
		if (std::is_trivially_destructible_v<cs225::Background>)
		{
			throw std::runtime_error(
				"Background must manage resources; it must have "
				"a destructor with non-default implementation."
			);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

int main()
{
	std::size_t testIndex;
	if (std::cin >> testIndex)
	{
		const Test tests[] = {
			test1,
			test2,
			test3,
			test4,
			test5,
			test6,
			test7,
			test8,
			test9,
			test10
		};
		const std::size_t testCount = sizeof(tests) / sizeof(*tests);

		if (testIndex == 0)
		{
			for (std::size_t i = 0; i < testCount; ++i)
			{
				run(tests[i]);
			}
		}
		else if (testIndex <= testCount)
		{
			run(tests[testIndex - 1]);
		}
		else
		{
			std::cout << "Invalid test index!" << std::endl;
		}
	}
}
