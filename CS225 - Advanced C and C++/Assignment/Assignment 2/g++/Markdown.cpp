#include "Markdown.h"
#include <iostream>

static int count = 0;
int counter2 = 0;

//Markdown::Markdown(std::ostream& stream) :os{ stream } {}

IFormatter& Markdown::insertHeader1(std::string s)
{
	os << "# 1. " << s << std::endl;
	//std::cout << std::endl;

	return *this;
}

IFormatter& Markdown::paragraph(std::string s)
{
	++count;
	os << s << std::endl;

	if (count <= 2)
	{
		std::cout << std::endl;
	}
	//os << s << std::endl;

	return *this;
}

IFormatter& Markdown::insertHeader2(std::string s)
{

	++counter2;

	std::string paragraph;

	if (counter2 == 1)
	{
		paragraph = "## 1.1. ";
	}
	else
	{
		paragraph = "## 1.2. ";
	}

	os << paragraph << s << std::endl;

	//os << s << std::endl;
	return *this;
}

IFormatter& Markdown::blockquote(std::string s)
{
	os << "> " << s << std::endl;
	std::cout << std::endl;
	return *this;
}


//IFormatter* construct(std::ostream& stream)
//{
//	IFormatter* result = new Markdown(stream);
//	return result;
//}

Markdown::~Markdown() {}
