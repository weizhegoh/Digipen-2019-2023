// Provide the interface of the Background class.
#include "Color.h"

#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

namespace cs225
{
	class Background
	{
	private:
		size_t size;
		Color* c;

	public:
		Background();
		Background(size_t _colourCount, const Color* _colors);

		Background(const Background &rhs);
		Background &operator= (const Background &rhs);		
		
		const Color operator[](unsigned int i) const;

		size_t count() const;

		~Background();
	};
}

#endif