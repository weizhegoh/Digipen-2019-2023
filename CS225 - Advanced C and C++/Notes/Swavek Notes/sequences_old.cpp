// g++ -Wall -Werror -Wextra -Wconversion -pedantic -std=c++11 -o sequences_old sequences_old.cpp

#include <iostream>

// See https://en.cppreference.com/w/cpp/utility/integer_sequence

/******************************************************************************
 * A type for storing a sequence.                                             *
 ******************************************************************************/

template <size_t... Ns>
struct index_sequence
{
	// The following member function does not exist in the STL type.
	// We will use it to test the sequence.
	static void print()
	{
		size_t numbers[] = {Ns...};
		for (const auto& number : numbers)
		{
			std::cout << number << " ";
		}
		std::cout << std::endl;
	}
};

/******************************************************************************
 * A meta-function for generating a sequence using a pre-C++17 approach       *
 ******************************************************************************/
 
template <size_t Counter, size_t... Rest>
struct make_sequence_impl
{
	using type = typename make_sequence_impl<
		Counter - 1,
		Counter - 1, Rest...
	>::type;
};

template <size_t... Rest>
struct make_sequence_impl<0, Rest...>
{
	using type = index_sequence<Rest...>;
};

template <size_t N>
using make_sequence =
	typename make_sequence_impl<N>::type;

/******************************************************************************
 * A sample use of a sequence                                                 *
 ******************************************************************************/
 
int main()
{
	/*
		The following statement is equivalent to this:
		index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>::print();
	*/
	make_sequence<10>::print();
}
