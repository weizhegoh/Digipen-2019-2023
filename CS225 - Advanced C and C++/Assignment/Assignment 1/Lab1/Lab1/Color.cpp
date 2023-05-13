// Provide the implementation of the Color class
// The interface has been already provided for you.

#include "Color.h"

namespace cs225
{
	using type = unsigned char;

	Color::Color(type red, type green, type blue):_red(red), _green(green), _blue(blue){}

	type Color::red() const
	{
		return _red;
	}

	type Color::green() const
	{
		return _green;
	}

	type Color::blue() const
	{
		return _blue;
	}
}