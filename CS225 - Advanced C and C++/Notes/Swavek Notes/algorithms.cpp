#include <iostream>
#include <map>
#include <vector>

namespace cs225
{
	/***************************************************************************
	 *
	 * for_each()
	 *
	 * Invoke a callable object per item in the given range from begin until end
	 * (excluding).
	 **************************************************************************/
	template <
		typename TIterator,
		typename F
	>
	F for_each(
		TIterator begin, const TIterator end,
		F action
	)
	{
		while (begin != end)
		{
			action(*begin++);
		}
		return action;
	}

	/***************************************************************************
	 *
	 * copy()
	 *
	 * Copy all elements in the given range from ibegin until iend into
	 * a preallocated container starting at obegin.
	 **************************************************************************/
	template <
		typename TInIterator,
		typename TOutIterator
	>
	TOutIterator copy(
		TInIterator ibegin, const TInIterator iend,
		TOutIterator obegin
	)
	{
		while (ibegin != iend)
		{
			*obegin++ = *ibegin++;
		}
		return obegin;
	}

	/***************************************************************************
	 *
	 * copy_if()
	 *
	 * Copy all elements in the given range from ibegin until iend that match
	 * the predicate into a preallocated container starting at obegin.
	 **************************************************************************/
	template <
		typename TInIterator,
		typename TOutIterator,
		typename F
	>
	TOutIterator copy_if(
		TInIterator ibegin, const TInIterator iend,
		TOutIterator obegin,
		F predicate
	)
	{
		while (ibegin != iend)
		{
			if (predicate(*ibegin))
			{
				*obegin++ = *ibegin;
			}
			ibegin++;
		}
		return obegin;
	}

	/***************************************************************************
	 *
	 * count()
	 *
	 * In the given range from begin until end count all occurrences of
	 * the given value.
	 **************************************************************************/
	template <
		typename TInIterator,
		typename TValue
	>
	size_t count(
		TInIterator begin, const TInIterator end,
		TValue value
	)
	{
		size_t result = 0;
		while (begin++ != end)
		{
			if (*begin == value)
			{
				++result;
			}
		}
		return result;
	}

	/***************************************************************************
	 *
	 * count_if()
	 *
	 * In the given range from begin until end count all elements that match
	 * the given predicate.
	 **************************************************************************/
	template <
		typename TInIterator,
		typename F
	>
	size_t count_if(TInIterator begin, const TInIterator end, F predicate)
	{
		size_t result = 0;
		while (begin != end)
		{
			if (predicate(*begin++))
			{
				++result;
			}
		}
		return result;
	}
	
	/***************************************************************************
	 *
	 * begin()
	 *
	 * Return a beginning iterator; for a static array return a pointer to the
	 * first element; for a container call its begin() member function.
	 **************************************************************************/
	template <
		typename T,
		std::size_t N
	>
	auto begin(T (&array)[N])
	{
		return array;
	}
	
	template <typename T>
	auto begin(T& container)
    {
		return container.begin();
	}
		
	/***************************************************************************
	 *
	 * end()
	 *
	 * Return an end iterator; for a static array return a pointer to the
	 * element after the last one; for a container call its end() member
	 * function.
	 **************************************************************************/
	template <
		typename T,
		std::size_t N
	>
	auto end(T (&array)[N])
	{
		return array + N;
	}
	
	template <typename T>
	auto end(T& container)
    {
		return container.end();
	}
}

#if 1

// Use our implementations
namespace ns = cs225;

#else

// Use STL implementations
#include <algorithm>
namespace ns = std;

#endif

int main()
{
	////////////////////////////////////////////////////////////////////////////
	// A static array.
	////////////////////////////////////////////////////////////////////////////
	
	const int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	const size_t count = sizeof(array)/sizeof(*array);
	
	// Printing with a loop via a subscript operator.
	{
		for (size_t i = 0; i < count; ++i)
		{
			std::cout << array[i] << ", ";
		}
		std::cout << std::endl;
	}
	
	// Printing with a loop with a pointer iterator.
	{
		const int* begin = array;
		const int* end = array + count;
		for (const int* it = begin; it != end; ++it)
		{
			const int& i = *it;
			std::cout << i << ", ";
		}
		std::cout << std::endl;
	}
	
	// Printing with a loop with an abstracted iterator.
	{
		for (auto it = ns::begin(array); it != ns::end(array); ++it)
		{
			const int& i = *it;
			std::cout << i << ", ";
		}
		std::cout << std::endl;
	}
	
	// Printing with an abstracted loop.
	{
		for (const int& i : array)
		{
			std::cout << i << ", ";
		}
		std::cout << std::endl;
	}
	
	// Printing with for_each and a lambda expression without a capture.
	{
		using Func = void (*)(const int&);
		Func print = [](const int& i)
		{
			std::cout << i << ", ";
		};
		ns::for_each(ns::begin(array), ns::end(array), print);
		std::cout << std::endl;
	}

	// Counting elements with a given value.
	size_t n = ns::count(ns::begin(array), ns::end(array), 5);
	std::cout << "Count of a value `5`: " << n << std::endl;
	
	// Counting elements with a predicate.
	using FuncBool = bool (*)(const int&);
	FuncBool isEven = [](const int& i)
	{
		return (i % 2 == 0);
	};
	size_t m = ns::count_if(ns::begin(array), ns::end(array), isEven);
	std::cout << "Count of even values: " << m << std::endl;
	
	// Copying elements that match a predicate into a preallocated vector.
	std::vector<int> v1;
	v1.resize(m);
	ns::copy_if(ns::begin(array), ns::end(array), ns::begin(v1), isEven);
	for (const auto& i : v1)
	{
		std::cout << i << ", ";
	}
	std::cout << std::endl;
	
	// Copying all elements into a preallocated vector.
	std::vector<int> v2;
	v2.resize(m);
	ns::copy(ns::begin(v1), ns::end(v1), ns::begin(v2));
	for (const auto& i : v2)
	{
		std::cout << i << ", ";
	}
	std::cout << std::endl;
	
	////////////////////////////////////////////////////////////////////////////
	// A map container.
	////////////////////////////////////////////////////////////////////////////
	
	std::map<std::string, int> numbers =
	{
		{"one", 1},
		{"two", 2}
	};
	numbers["three"] = 3;
	
	// Printing with a loop with an abstracted iterator.
	{
		for (const std::pair<std::string, int>& number : numbers)
		{
			std::cout << number.first << ", " << number.second << std::endl;
		}
		std::cout << std::endl;
	}
	
	// Printing with an abstracted loop.
	{
		for (const auto& number : numbers)
		{
			std::cout << number.first << ", " << number.second << std::endl;
		}
		std::cout << std::endl;
	}
	
	// Printing names with an iterator loop and std::tie()
	{
		std::string name;
		int value;
		for (auto it = ns::begin(numbers); it != ns::end(numbers); ++it)
		{
			std::tie(name, value) = *it;
			std::cout << name << ", ";
		}
	}
	
	// Printing with an abstracted loop and a structured binding.
	{
		for (const auto& [name, value] : numbers)
		{
			std::cout << name << ", " << value << std::endl;
		}
		std::cout << std::endl;
	}
}
