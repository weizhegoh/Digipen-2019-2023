#ifndef _IFORMATTERFACTORY_H_
#define _IFORMATTERFACTORY_H_

#include <string>
#include "IFormatterElement.h"

class IFormatterFactory
{
public:
	virtual IFormatterElement* create_begin() = 0;
	virtual IFormatterElement* create_header1(std::string s) = 0;
	virtual IFormatterElement* create_paragraph(std::string s) = 0;
	virtual IFormatterElement* create_header2(std::string s) = 0;
	virtual IFormatterElement* create_blockquote(std::string s) = 0;
	virtual IFormatterElement* create_end() = 0;
	virtual ~IFormatterFactory() = default;
};

#endif // _IFORMATTERFACTORY_H_