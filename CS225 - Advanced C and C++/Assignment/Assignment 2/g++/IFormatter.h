#pragma once
#include <string>

class IFormatter
{
public:
	virtual IFormatter& insertHeader1(std::string s) = 0;
	virtual IFormatter&  paragraph(std::string s) = 0;
	virtual IFormatter& insertHeader2(std::string s) = 0;
	virtual IFormatter& blockquote(std::string s)= 0;
	virtual ~IFormatter() = default;
};