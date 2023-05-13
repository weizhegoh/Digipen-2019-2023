/* Start Header
*****************************************************************/
/*!
\file   bitset_tep.hpp
\author Goh Wei Zhe, weizhe.goh, 440000119
\par    email: weizhe.goh\@digipen.edu
\date   November 26, 2020
\brief  Contains definition of templated alias of class bitset_tep

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#ifndef BITSET_TEP_HPP
#define BITSET_TEP_HPP

#include "bitset_tep.h"
#include <iostream>

template<typename T> template <typename... Args>
cs225::bitset_tep::Model<T>::Model(Args&&... args): _instance{std::forward<Args>(args)...}{}

template<typename T> 
size_t cs225::bitset_tep::Model<T>::size() const
{
   return _instance.size();
}

template<typename T>
size_t cs225::bitset_tep::Model<T>::count() const 
{
    return _instance.count();
}

template<typename T>
void cs225::bitset_tep::Model<T>::set(size_t pos, bool value)
{
    _instance.set(pos, value);
}

template<typename T>
void cs225::bitset_tep::Model<T>::flip(size_t pos)
{
    _instance.flip(pos);
}

template<typename T>
void cs225::bitset_tep::Model<T>::reset(size_t pos)
{
    _instance.reset(pos);
}

template<typename T>
bool cs225::bitset_tep::Model<T>::test(size_t pos) const
{
    return _instance.test(pos);
}

template<typename T>
std::string cs225::bitset_tep::Model<T>::to_string(char c1, char c2) const
{
    return _instance.to_string(c1, c2);
}

template<typename T>
bool cs225::bitset_tep::Model<T>::operator[](size_t pos) const
{
    return _instance.operator[](pos);
}

template<typename T, typename... Args>
static cs225::bitset_tep cs225::bitset_tep::create(Args&&... args)
{
    return bitset_tep(std::make_unique<Model<T, Args...>>(std::forward<Args>(args)...));
}

#endif // !BITSET_TEP_HPP
