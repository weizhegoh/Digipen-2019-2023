/* Start Header
*****************************************************************/
/*!
\file   allocator.cpp
\author Goh Wei Zhe, weizhe.goh, 440000119
\par    email: weizhe.goh\@digipen.edu
\date   November 26, 2020
\brief  Contains definition of class allocator

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#include "allocator.h"
#include <iostream>

unsigned char* cs225::allocator::allocate(size_t count)
{
	size_t size = (count + CHAR_BIT - 1) / CHAR_BIT;
	unsigned char* bitset = new unsigned char[size] {0};

	std::cout << "Allocate " << size << " elements." << std::endl;
	return bitset;
}

void cs225::allocator::deallocate(unsigned char* ptr, size_t count)
{
	size_t size = (count + CHAR_BIT - 1) / CHAR_BIT;
	delete[] ptr;

	std::cout << "Deallocate " << size << " elements." << std::endl;
}