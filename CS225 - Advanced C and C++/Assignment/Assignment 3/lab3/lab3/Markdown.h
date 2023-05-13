#ifndef _MARKDOWN_H_
#define _MARKDOWN_H_

#include "IFormatterFactory.h"
#include "ISavable.h"

class Markdown : public IFormatterFactory
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

	~Markdown() {}
};

#endif // _MARKDOWN_H_
