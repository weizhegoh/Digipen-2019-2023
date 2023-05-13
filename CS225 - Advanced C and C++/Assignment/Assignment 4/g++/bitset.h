#ifndef _BITSET_H_
#define _BITSET_H_

#include <bitset>

namespace cs225
{
	template<std::size_t N>
	class bitset
	{
	public:

		std::bitset<N> bitset;
		
		size_t count();
		std::string to_string();
		std::string to_string(char s1);
		std::string to_string(char s1, char s2);
		
		size_t size();
		bool test(size_t index);

		void set(size_t pos, bool value = true);
		void reset(size_t pos);
		void flip(size_t pos);

		bool operator[](std::size_t pos) const;


	};
}

#include "bitset.hpp"

#endif

