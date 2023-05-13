/* Start Header
*****************************************************************/
/*!
\file   bitset.h
\author Goh Wei Zhe, weizhe.goh, 440000119
\par    email: weizhe.goh\@digipen.edu
\date   November 26, 2020 
\brief  Contains declaration of member functions of class bitset 

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#ifndef BITSET_H
#define BITSET_H

#include <bitset>

namespace cs225
{
	template<std::size_t N, typename Allocator>
	class bitset
	{
	public:

		unsigned char* _bitset;

		bitset();

		std::size_t count() const;
		std::size_t size() const;

		bitset<N, Allocator>& set(std::size_t pos, bool value = true);
		bitset<N, Allocator>& reset(std::size_t pos);
		bitset<N, Allocator>& flip(std::size_t pos);	
		
		bool test(std::size_t pos) const;
		bool operator[](std::size_t pos) const;

		std::string to_string(char c1 = '0' , char c2 = '1') const;
		
		~bitset();
	};
}

#include "bitset.hpp"

#endif