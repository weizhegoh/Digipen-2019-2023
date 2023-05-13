/* Start Header
*****************************************************************/
/*!
\file   bitset_tep.h
\author Goh Wei Zhe, weizhe.goh, 440000119
\par    email: weizhe.goh\@digipen.edu
\date   November 26, 2020
\brief  Contains declaration of class bitset_tep

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#ifndef BITSET_TEP_H
#define BITSET_TEP_H

#include <string>
#include <iostream>

namespace cs225
{
	class bitset_tep
	{
		struct IConcept
		{
			virtual size_t size() const = 0;
			virtual size_t count() const = 0;

			virtual void set(size_t, bool = true) = 0;
			virtual void flip(size_t) = 0;
			virtual void reset(size_t) = 0;
			
			virtual bool test(size_t) const = 0;
			virtual bool operator[](std::size_t pos) const = 0;

			virtual std::string to_string(char = '0', char = '1') const = 0;
			virtual ~IConcept() = default;
		};

		template<typename T>
		class Model : public IConcept
		{
			T _instance;
	
		public:
			
			template<typename... Args>
			Model(Args&&... args);

			virtual size_t size() const override; 
			virtual size_t count() const override;

			virtual void set(size_t pos, bool value = true) override;
			virtual void flip(size_t pos) override;
			virtual void reset(size_t pos) override;

			virtual bool test(size_t pos) const override;
			virtual bool operator[](std::size_t pos) const override;

			virtual std::string to_string(char = '0', char = '1') const override;
		};

		std::unique_ptr<IConcept> _concept;
		bitset_tep(std::unique_ptr<IConcept> concept);

	public:
		std::size_t count() const;
		std::size_t size() const;

		void set(size_t pos, bool value = true);
		void flip(size_t pos) const;
		void reset(size_t pos) const;

		bool test(size_t pos) const;
		bool operator[](std::size_t pos) const;

		std::string to_string(char c1 = '0', char c2 = '1') const;

		template<typename T, typename... Args>
		static bitset_tep create(Args&&... args);
	};
}

#include "bitset_tep.hpp"

#endif // !_BITSET_TEP_H

