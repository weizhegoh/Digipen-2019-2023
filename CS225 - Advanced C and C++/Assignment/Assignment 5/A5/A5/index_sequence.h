#ifndef INDEX_SEQUENCE_H
#define INDEX_SEQUENCE_H

#include <iostream>

template <size_t... Ns>
struct index_sequence
{
	static void print()
	{
		#ifdef CPP11
		std::cout << "[C++11]\t";
		#else
		std::cout << "[C++17]\t";
		#endif
		
		const size_t numbers[] = {Ns...};
		for (const auto& number : numbers)
		{
			std::cout << number << ", ";
		}
		std::cout << std::endl;
	}
};

#endif // INDEX_SEQUENCE_H
