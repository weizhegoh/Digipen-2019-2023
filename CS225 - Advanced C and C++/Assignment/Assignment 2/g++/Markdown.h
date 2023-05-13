#pragma once
#include "IFormatter.h"

class Markdown : public IFormatter
{
public:
	std::ostream& os;
	Markdown(std::ostream& stream);
	IFormatter& insertHeader1(std::string s);
	IFormatter& paragraph(std::string s);
	IFormatter& insertHeader2(std::string s);
	IFormatter& blockquote(std::string s);
	~Markdown();
};

