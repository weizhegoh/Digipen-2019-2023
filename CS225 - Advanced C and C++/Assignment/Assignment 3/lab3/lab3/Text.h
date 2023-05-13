#ifndef _TEXT_H_
#define _TEXT_H_

#include "IFormatterFactory.h"
#include "ISavable.h"

class Text : public IFormatterFactory
{
public:
	IFormatterElement* create_begin()
	{
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

	~Text() {}
};

#endif // _TEXT_H_