/******************************************************************************
 * @Author:     Swavek Wlodkowski
 * @Course:     CS 225
 * @Trimester:  Fall 2020
 * @Assignment: 1
 * 
 * This file contains the interface for the Color class. Do not change it, but
 * remember to provide the implementation of these member functions in *.cpp.
 *****************************************************************************/

#ifndef _COLOR_H_
#define _COLOR_H_

namespace cs225
{
	class Color
	{
	public:
		using type = unsigned char;

		Color(
			type red = 0,
			type green = 0,
			type blue = 0
		);
	
		type red() const;
		type green() const;
		type blue() const;
		
	private:
		type _red;
		type _green;
		type _blue;
	};
}

#endif
