#pragma once
#include "IFormatter.h"
#include <iostream>

class Html : public IFormatter
{
public:
	std::ostream& os;
	Html(std::ostream& stream);
	IFormatter& insertHeader1(std::string s);
	IFormatter& paragraph(std::string s);
	IFormatter& insertHeader2(std::string s);
	IFormatter& blockquote(std::string s);
	~Html();
};

