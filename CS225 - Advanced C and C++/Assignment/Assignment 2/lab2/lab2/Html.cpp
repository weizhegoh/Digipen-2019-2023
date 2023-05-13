#include "Html.h"

static int count = 0;
Html::Html(std::ostream& stream) :os{ stream } {}

IFormatter& Html::insertHeader1(std::string s)
{
	std::cout <<"1. " << s << std::endl;
	return *this;
}

IFormatter& Html::paragraph(std::string s)
{
	std::cout << s << std::endl;
	return *this;
}

IFormatter& Html::insertHeader2(std::string s)
{
	++count;

	std::string paragraph;

	if (count == 1)
	{
		paragraph = "1.1. ";
	}
	else
	{
		paragraph = "1.2. ";
	}

	os << paragraph << s << std::endl;
	//std::cout << s << std::endl;
	return *this;
}

IFormatter& Html::blockquote(std::string s)
{
	std::cout << s << std::endl;
	return *this;
}

IFormatter* construct(std::ostream& stream)
{
	IFormatter* result = new Html(stream);
	return result;
}

Html::~Html(){}
