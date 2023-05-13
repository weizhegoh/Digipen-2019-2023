#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <iostream>
#include <list>

class MemoryManager
{
public:

	std::list<void*> memory;

	MemoryManager(int total_bytes);
	
	void* allocate(int bytes);
	void deallocate(void* pointer);
	void dump(std::ostream& out);

	~MemoryManager(void);

};

#endif // !MEMORY_MANAGER_H
