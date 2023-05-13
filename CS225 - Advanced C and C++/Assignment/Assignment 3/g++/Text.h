#ifndef _TEXT_H_
#define _TEXT_H_

#include "IFormatterFactory.h"
#include "ISavable.h"

class Text : public IFormatterFactory, public IFormatterElement, public ISavable
{
public:
    std::string str;
    
    Text() = default; 
    Text(std::string s): str{s}{};
    
	IFormatterElement* create_begin() override
	{
        std::string newString; 
		return new Text(newString);
	}

	IFormatterElement* create_header1(std::string s) override
	{
        std::string newString; 

        newString += s;
   		newString += "\n";
        
		return new Text(newString);
	}

	IFormatterElement* create_paragraph(std::string s) override
	{
   		std::string newString; 

        newString += s;
   		newString += "\n";
        
        static int count = 0;
        ++count;
        
        if(count<=2)
            newString +="\n";
    
		return new Text(newString);
	}

	IFormatterElement* create_header2(std::string s) override
	{
        std::string newString;
        
        newString += s;
   		newString += "\n";
     
		return new Text(newString);
	}

	IFormatterElement* create_blockquote(std::string s) override
	{
        std::string newString;
        
        newString += "\"";
   		newString += s;
        newString += "\"";
   		newString += "\n";
        
		return new Text(newString);
	}

	IFormatterElement* create_end() override
	{
        std::string newString;
		return new Text(newString);
	}
    
  	void save(std::ostream& stream)
    {
        stream << str; 
    }

	~Text() {}
};

#endif // _TEXT_H_