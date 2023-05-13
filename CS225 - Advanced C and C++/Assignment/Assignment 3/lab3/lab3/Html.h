#ifndef _HTML_H_
#define _HTML_H_

#include "IFormatterFactory.h"
#include "ISavable.h"

class Html : public IFormatterFactory
{
public:
	IFormatterElement* create_begin()
	{
		std::cout << "hi" << std::endl;
		return nullptr;
	}

	IFormatterElement* create_header1(std::string s)
	{
		return nullptr;

	}

	IFormatterElement* create_paragraph(std::string s)
	{
		return nullptr;

	}

	IFormatterElement* create_header2(std::string s)
	{
		return nullptr;

	}

	IFormatterElement* create_blockquote(std::string s)
	{
		return nullptr;

	}

	IFormatterElement* create_end()
	{
		return nullptr;

	}

	~Html(){}
};

#endif // _HTML_H_
