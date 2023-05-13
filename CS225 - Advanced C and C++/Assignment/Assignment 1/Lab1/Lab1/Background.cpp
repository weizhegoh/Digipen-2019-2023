// Provide the implementation of the Background class.

#include <iostream>
#include "Background.h"
#include "Color.h"

namespace cs225
{
	Background::Background() :size(0), c(new Color[size]) {}

	Background::Background(size_t _colourCount, const Color* _colors):size(_colourCount),c(new Color[size])
	{
		for (unsigned int i = 0; i < _colourCount; ++i)
		{
			this->c[i] = { _colors[i].red(), _colors[i].green(), _colors[i].blue() };
		}
	}

	Background::Background(const Background& rhs) :size(rhs.size), c(new Color[size])
	{
		for (unsigned int i = 0; i < rhs.size; ++i)
		{
			this->c[i] = { rhs.c[i].red(), rhs.c[i].green(), rhs.c[i].blue() };
		}
	}

	Background &Background::operator=(const Background &rhs)
	{
		if (this == &rhs)
		{
			return*this;
		}

			this->size = rhs.size;
			Color* clr = new Color[rhs.size];
			 
			for (unsigned int i = 0; i < rhs.size; ++i)
			{
				clr[i] = { rhs.c[i].red(), rhs.c[i].green(), rhs.c[i].blue() };
			}

			delete[] this->c;
			this->c = clr;

			return *this;
	}

	const Color Background::operator[](unsigned int i)const
	{
		Color clr = { c[i].red(), c[i].green(), c[i].blue() };	
		return clr;
	}

	size_t Background::count() const
	{
		return this->size;
	}

	 Background:: ~ Background()
	{
		 delete[] c;
	}
}