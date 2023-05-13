#include "Text.h"
#include <iostream>

int count = 0;

//Text::Text(std::ostream& stream) :os{ stream } {}

IFormatter& Text::insertHeader1(std::string s)
{
	os << s << std::endl;

	return *this;
}

IFormatter& Text::paragraph(std::string s)
{
	++count;

	os << s << std::endl;

	if (count <= 2 || count >= 5)
	{
		std::cout << std::endl;
	}

	return *this;
}

IFormatter& Text::insertHeader2(std::string s)
{
	os << s << std::endl;
	return *this;
}

IFormatter& Text::blockquote(std::string s)
{
	os << "\"" << s << "\"" << std::endl;
	return *this;
}

//IFormatter* construct(std::ostream& stream)
//{
//	IFormatter* result = new Text(stream);
//	return result;
//}

Text::~Text()
{
}

