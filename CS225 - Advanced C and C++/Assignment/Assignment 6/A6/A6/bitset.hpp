/* Start Header
*****************************************************************/
/*!
\file   bitset.hpp
\author Goh Wei Zhe, weizhe.goh, 440000119
\par    email: weizhe.goh\@digipen.edu
\date   November 26, 2020 
\brief  Contains definition of member functions of class bitset 

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#ifndef BITSET_HPP
#define BITSET_HPP

#include "bitset.h"

template<std::size_t N, typename Allocator>
cs225::bitset<N, Allocator>::bitset()
{
	_bitset = cs225::allocator::allocate(N);
}

template<std::size_t N, typename Allocator>
cs225::bitset<N, Allocator>& cs225::bitset<N, Allocator>::set(size_t pos, bool value)
{
	if (value)
		_bitset[pos / CHAR_BIT] |=
		static_cast<unsigned char>(1U << pos % CHAR_BIT);
	else
		_bitset[pos / CHAR_BIT] &=
		static_cast<unsigned char>(~(1U << pos % CHAR_BIT));

	return *this;
}

template<std::size_t N, typename Allocator>
cs225::bitset<N, Allocator>& cs225::bitset<N, Allocator>::reset(size_t pos)
{
	_bitset[pos / CHAR_BIT] &=
		static_cast<unsigned char>(~(1U << pos % CHAR_BIT));

	return *this;
}

template<std::size_t N, typename Allocator>
cs225::bitset<N, Allocator>& cs225::bitset<N, Allocator>::flip(size_t pos)
{
	_bitset[pos / CHAR_BIT] ^=
		static_cast<unsigned char>(1U << (pos % CHAR_BIT));

	return *this;
}

template<std::size_t N, typename Allocator>
size_t cs225::bitset<N, Allocator>::count() const
{
	size_t count = 0;

	for (size_t i = N; i > 0;)
		(bitset<N, Allocator>::test(--i) ? ++count : count);

	return count;
}

template<std::size_t N, typename Allocator>
std::string cs225::bitset<N, Allocator>::to_string(char c1, char c2) const
{
	std::string s;

	char zero = static_cast<unsigned char>(c1);
	char one = static_cast<unsigned char>(c2);

	for (size_t i = N; i > 0;)
		s.push_back(bitset<N, Allocator>::test(--i) ? one : zero);

	return s;
}

template<std::size_t N, typename Allocator>
size_t cs225::bitset<N, Allocator>::size() const
{
	return N;
}

template<std::size_t N, typename Allocator>
bool cs225::bitset<N, Allocator>::test(size_t pos) const
{
	if (N <= pos)
		throw std::out_of_range("Out of range");

	return (_bitset[pos / CHAR_BIT] & (1U << pos % CHAR_BIT)) != 0;
}

template<std::size_t N, typename Allocator>
bool cs225::bitset<N, Allocator>::operator[](size_t pos) const
{
	return (_bitset[pos / CHAR_BIT] & (1U << pos % CHAR_BIT)) != 0;
}

template<std::size_t N, typename Allocator>
cs225::bitset<N, Allocator>::~bitset()
{
	cs225::allocator::deallocate(_bitset, N);
}

#endif // !BITSET_HPP_