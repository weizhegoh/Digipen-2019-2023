/* Start Header
*****************************************************************/
/*!
\file   allocator.h
\author Goh Wei Zhe, weizhe.goh, 440000119
\par    email: weizhe.goh\@digipen.edu
\date   November 26, 2020
\brief  Contains declaration of class allocator

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <memory>

namespace cs225
{	
	class allocator
	{
	public:
		static unsigned char* allocate(size_t count);
		static void deallocate(unsigned char* ptr, size_t count);
	};
}

#endif