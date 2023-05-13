#include "Constructor.h"
#include "Text.h"
#include "Html.h"
#include "Markdown.h"

std::vector<std::string> get_options()
{
	std::vector < std::string > v; 

	v.push_back("Text");
	v.push_back("Markdown");
	v.push_back("HTML");

	return v;
}

IFormatterFactory* construct(int selected_option)
{
	IFormatterFactory* result = NULL; 

	switch (selected_option)
	{
		case 0 : result = new Text(); break;
		case 1 : result = new Markdown(); break;
		case 2: result = new Html(); break;
		default: break; 
	}

	return result; 
}

void save(ISavable& savable, std::ostream& stream)
{

}
