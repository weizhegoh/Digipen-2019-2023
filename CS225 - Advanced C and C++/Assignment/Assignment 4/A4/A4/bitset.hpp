/* Start Header
*****************************************************************/
/*!
\file   bitset.hpp
\author Goh Wei Zhe, weizhe.goh, 440000119
\par    email: weizhe.goh\@digipen.edu
\date   November 9, 2020 
\brief  Contains definition of member functions of class bitset 

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#ifndef BITSET_HPP_
#define BITSET_HPP_

#include "bitset.h"

template<size_t N> 
cs225::bitset<N>::bitset()
{
	size_t _size = (N + CHAR_BIT - 1) / CHAR_BIT;
	_bitset = new unsigned char[_size] {0};
}

template<size_t N> 
cs225::bitset<N>& cs225::bitset<N>::set(size_t pos, bool value)
{
	if (value)
		_bitset[pos / CHAR_BIT] |=
        static_cast<unsigned char>(1U << pos % CHAR_BIT);
	else
		_bitset[pos / CHAR_BIT] &=
        static_cast<unsigned char>(~(1U << pos % CHAR_BIT));

	return *this;
}

template<size_t N> 
cs225::bitset<N>& cs225::bitset<N>::reset(size_t pos)
{
	_bitset[pos / CHAR_BIT] &=
    static_cast<unsigned char>(~(1U << pos % CHAR_BIT));
	
    return *this;
}

template<size_t N> 
cs225::bitset<N>& cs225::bitset<N>::flip(size_t pos)
{
	_bitset[pos / CHAR_BIT] ^=
    static_cast<unsigned char>(1U << (pos % CHAR_BIT));
	
    return *this;
}

template<size_t N> size_t cs225::bitset<N>::count() const
{
	size_t count = 0;

	for (size_t i = N; i > 0;)
		(bitset<N>::test(--i) ? ++count : count);
	
	return count; 
}

template<size_t N> 
std::string cs225::bitset<N>::to_string(char c1, char c2) const
{
	std::string s;
	
	char zero = static_cast<unsigned char>(c1);
	char one = static_cast<unsigned char>(c2);

	for (size_t i = N; i > 0;)
		s.push_back(bitset<N>::test(--i) ? one : zero);

	return s;
}

template<size_t N> size_t cs225::bitset<N>::size() const
{
	return N;
}

template<size_t N> bool cs225::bitset<N>::test(size_t pos) const
{
	if (N <= pos)
		throw std::out_of_range("Out of range");

	return (_bitset[pos / CHAR_BIT] & (1U << pos % CHAR_BIT)) != 0;
}

template<size_t N> bool cs225::bitset<N>::operator[](size_t pos) const
{
	return (_bitset[pos / CHAR_BIT] & (1U << pos % CHAR_BIT)) != 0;
}

template<size_t N> cs225::bitset<N>::~bitset()
{
	delete []_bitset;
}

#endif // !BITSET_HPP_