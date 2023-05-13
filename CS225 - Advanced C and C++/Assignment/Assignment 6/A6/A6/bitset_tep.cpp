/* Start Header
*****************************************************************/
/*!
\file   bitset_tep.cpp
\author Goh Wei Zhe, weizhe.goh, 440000119
\par    email: weizhe.goh\@digipen.edu
\date   November 26, 2020
\brief  Contains definition of non-templated alias of class bitset_tep

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#include "bitset_tep.h"

cs225::bitset_tep::bitset_tep(std::unique_ptr<cs225::bitset_tep::IConcept> concept)
	: _concept{std::move(concept) } 
{

}

size_t cs225::bitset_tep::count() const
{
	return _concept->count();
}

size_t cs225::bitset_tep::size() const
{
	return _concept->size();
}

void cs225::bitset_tep::set(size_t pos, bool value)
{
	_concept->set(pos, value);
}

void cs225::bitset_tep::flip(size_t pos) const
{
	_concept->flip(pos);
}

void cs225::bitset_tep::reset(size_t pos) const
{
	_concept->reset(pos);
}

bool cs225::bitset_tep::test(size_t pos) const
{
	return _concept->test(pos);
}

bool cs225::bitset_tep::operator[](size_t pos) const
{
	return _concept->operator[](pos);
}

std::string cs225::bitset_tep::to_string(char c1, char c2) const
{
	return _concept->to_string(c1, c2);
}