#ifndef _HTML_H_
#define _HTML_H_

#include "IFormatterFactory.h"
#include "ISavable.h"

class Html : public IFormatterFactory, public IFormatterElement, public ISavable
{
public:
    std::string str;
    
    Html() = default; 
    Html(std::string s): str{s}{};
    
	IFormatterElement* create_begin() override
	{
        std::string newString; 
        
        newString += "<!doctype html>\n";
        newString += "<html lang=en>\n";
        newString += "<style>blockquote { border-left: 4px solid #d0d0d0;";
        newString += " padding: 4px; }</style>\n";
        newString += "<head>\n";
        newString += "\t<meta charset=utf-8>\n";
        newString += "\t<title>Result</title>\n";
        newString += "</head>\n";
        newString += "<body>\n";
        
		return new Html(newString);
	}

	IFormatterElement* create_header1(std::string s) override
	{
        std::string newString; 
        
        newString += "\t<h1>1. ";
		newString += s;
        newString += "</h1>\n";
     
		return new Html{newString};
	}

	IFormatterElement* create_paragraph(std::string s) override
	{
        std::string newString; 
        
        newString += "\t<p>";
		newString += s;
        newString += "</p>\n";
     
		return new Html{newString};
	}

	IFormatterElement* create_header2(std::string s) override
	{
        std::string newString; 
        
        static int count = 0;
        ++count;

        newString += "\t<h2>";
        
        if (count == 1)
        {
            newString += "1.1. ";
        }
        else
        {
            newString += "1.2. ";
        }

		newString += s;
   		newString += "</h2>\n";
     
		return new Html{newString};
	}

	IFormatterElement* create_blockquote(std::string s) override
	{
        std::string newString; 
        
        newString +="\t<blockquote>";
		newString += s;
   		newString += "</blockquote>\n";
     
		return new Html{newString};
	}

	IFormatterElement* create_end()  override
	{
        std::string newString; 
        
        newString += "</body>\n";
        newString += "</html>\n";
        
		return new Html{newString};
	}
    
    void save(std::ostream& stream)
    {
        stream << str; 
    }

	~Html(){}
};

#endif // _HTML_H_
