#ifndef BITSET_HPP_
#define BITSET_HPP_

#include "bitset.h"

template<size_t N> size_t cs225::bitset<N>::count()
{
	return 0;
}

template<size_t N> std::string cs225::bitset<N>::to_string()
{
	return "";
}

template<size_t N> std::string cs225::bitset<N>::to_string(char s1)
{
	(void)s1;

	return "";
}

template<size_t N> std::string cs225::bitset<N>::to_string(char s1, char s2)
{
	(void)s1;
	(void)s2;

	return "";
}

template<size_t N> size_t cs225::bitset<N>::size()
{
	return 0;
}

template<size_t N> bool cs225::bitset<N>::test(size_t index)
{
	(void)index;
	return 0;
}

template<size_t N> void cs225::bitset<N>::set(size_t pos, bool value)
{
	(void)pos;
	(void)value;

	return;
}

template<size_t N> void cs225::bitset<N>::reset(size_t pos)
{
	(void)pos;

	return;
}

template<size_t N> void cs225::bitset<N>::flip(size_t pos)
{
	(void)pos;
	return;
}

template<size_t N> bool cs225::bitset<N>::operator[](size_t pos) const
{
	(void)pos;
	return 0;
}

#endif // !BITSET_HPP_