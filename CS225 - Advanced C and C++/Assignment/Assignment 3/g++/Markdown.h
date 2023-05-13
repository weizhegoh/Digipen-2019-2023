#ifndef _MARKDOWN_H_
#define _MARKDOWN_H_

#include "IFormatterFactory.h"
#include "ISavable.h"

class Markdown : public IFormatterFactory, public IFormatterElement
, public ISavable
{
public:
     std::string str;
    
    Markdown() = default; 
    Markdown(std::string s): str{s}{};

	IFormatterElement* create_begin()
	{
        std::string newString; 
        
        newString += "\n";
		return new Markdown(newString);
	}

	IFormatterElement* create_header1(std::string s) override
	{
        std::string newString; 
        
        newString += "# 1. ";
   		newString += s;
   		newString += "\n";
     
		return new Markdown{newString};
	}

	IFormatterElement* create_paragraph(std::string s) override
	{
        std::string newString; 

   		newString += s;
   		newString += "\n";
        
        static int count = 0;
        ++count;
        
        if (count <= 2)
        {
            newString+= "\n";
        }
		
        return new Markdown{newString};
	}

	IFormatterElement* create_header2(std::string s) override
	{
        std::string newString; 
        
        static int counter = 0;
        ++counter;

        if (counter == 1)
        {
            newString += "## 1.1. ";
        }
        else
        {
            newString += "## 1.2. ";
        }

   		newString += s;
   		newString += "\n";
     
		return new Markdown{newString};
	}

	IFormatterElement* create_blockquote(std::string s) override
	{
        std::string newString; 

        newString += "> ";
   		newString += s;
   		newString += "\n\n";
     
		return new Markdown{newString};
	}

	IFormatterElement* create_end() override
	{
        std::string newString; 
		return new Markdown{newString};
	}
    
    void save(std::ostream& stream)
    {
        stream << str; 
    }

	~Markdown() {}
};

#endif // _MARKDOWN_H_
