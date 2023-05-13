#include <iostream>
#include "MemoryManager.h"

int main()
{
	MemoryManager m(100);

	std::cout << "The original block..." << std::endl;
	m.dump(std::cout);
	char *first=(char*)m.allocate(20);
	char *second=(char*)m.allocate(20);
	char *third=(char*)m.allocate(20);
	char *fourth=(char*)m.allocate(20);
	char *fifth=(char*)m.allocate(20);
	
	std::cout << std::endl<< "After allocating all..." << std::endl;
	m.dump(std::cout);	
	m.deallocate(second);
	std::cout << std::endl<< "After deallocating second..." << std::endl;
	m.dump(std::cout);
	m.deallocate(fourth);
	std::cout << std::endl<< "After deallocating fourth..." << std::endl;
	m.dump(std::cout);
	m.deallocate(third);
	std::cout << std::endl<< "After deallocating third..." << std::endl;
	m.dump(std::cout);
	m.deallocate(first);
	std::cout << std::endl<< "After deallocating first..." << std::endl;
	m.dump(std::cout);
	m.deallocate(fifth);
	std::cout << std::endl<< "After deallocating fifth..." << std::endl;
	m.dump(std::cout);
	
	/**** 
	    There will be no difference between first, best and worst fit 
		for this driver.
	****/
}