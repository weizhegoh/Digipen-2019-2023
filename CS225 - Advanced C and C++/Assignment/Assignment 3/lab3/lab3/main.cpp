#include "MemoryAlloc.h"
#include "Constructor.h"

void process(IFormatterElement* const element)
{
	if (element)
	{
		try
		{
			if (ISavable* const savable = dynamic_cast<ISavable* const>(element))
			{
				save(*savable, std::cout);
			}
			delete element;
		}
		catch (...)
		{
			delete element;
			throw;
		}
	}
}

int main()
{
	std::cerr << "Choose a formatter:\n";
	
	std::vector<std::string> options = get_options();
	for (size_t i = 0; i < options.size(); ++i)
	{
		std::cerr << "\t" << (i + 1) << ". " << options[i] << "\n";
	}
	
	std::cerr
		<< "\tother to exit\n"
		<< "Your choice: ";
		
	int selected_option;
	if (std::cin >> selected_option)
	{
		if (IFormatterFactory* factory = construct(--selected_option))
		{
			std::cerr << "\nYou chose \"" << options[selected_option] << "\"." << std::endl;
			try
			{
				process(factory->create_begin());
				process(factory->create_header1("Assignment result"));
				process(factory->create_paragraph("This file represents an assignment result."));
				process(factory->create_header2("Initial paragraph"));
				process(factory->create_paragraph("This is the first paragraph of the result file."));
				process(factory->create_header2("Final paragraph"));
				process(factory->create_paragraph("This is the second paragraph of the result file."));
				process(factory->create_blockquote("This is a quote."));
				process(factory->create_paragraph("This is the third paragraph of the result file."));
				process(factory->create_paragraph("This is the last paragraph of the result file."));
				process(factory->create_end());
			}
			catch (...)
			{
				std::cerr << "An exception has been thrown!" << std::endl;
			}
			delete factory;
		}
		else
		{
			std::cerr << "Exiting..." << std::endl;
		}
	}
}
