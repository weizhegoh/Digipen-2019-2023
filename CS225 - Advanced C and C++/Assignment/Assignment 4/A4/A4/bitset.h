/* Start Header
*****************************************************************/
/*!
\file   bitset.h
\author Goh Wei Zhe, weizhe.goh, 440000119
\par    email: weizhe.goh\@digipen.edu
\date   November 9, 2020 
\brief  Contains declaration of member functions of class bitset 

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#ifndef _BITSET_H_
#define _BITSET_H_

#include <bitset>

namespace cs225
{
	template<std::size_t N>
	class bitset
	{
	public:

		unsigned char* _bitset;

		bitset();

		bitset<N>& set(std::size_t pos, bool value = true);
		bitset<N>& reset(std::size_t pos);
		bitset<N>& flip(std::size_t pos);	
		
		std::size_t count() const;
		std::size_t size() const;
		bool test(std::size_t pos) const;

		std::string to_string(char c1 = '0' , char c2 = '1') const;
		bool operator[](std::size_t pos) const;
		
		~bitset();
	};
}

#include "bitset.hpp"

#endif