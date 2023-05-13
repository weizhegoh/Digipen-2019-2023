#include "MemoryAlloc.h"
#include "Constructor.h"

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	IFormatter* fmt = construct(std::cout);
	if (fmt)
	{
		try
		{
			IFormatter& formatter = *fmt;
			formatter
				.insertHeader1("Assignment result")
				.paragraph("This file represents an assignment result.")
				.insertHeader2("Initial paragraph")
				.paragraph("This is the first paragraph of the result file.")
				.insertHeader2("Final paragraph")
				.paragraph("This is the second paragraph of the result file.")
				.blockquote("This is a quote.")
				.paragraph("This is the third paragraph of the result file.")
				.paragraph("This is the last paragraph of the result file.");
		}
		catch (...)
		{
			// Do nothing.
		}
		delete fmt;
	}
}
