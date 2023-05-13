#include "ObjectAllocator.h"

ObjectAllocator::ObjectAllocator(size_t ObjectSize, const OAConfig& config) : Config_(config)
{
	ObjectSize_ = ObjectSize;
}

void *ObjectAllocator::Allocate() throw (OAException)
{
	return new char[ObjectSize_];
}

void ObjectAllocator::Free(void *anObject) throw(OAException)
{
	// Defer to C++ heap manager
	delete [] reinterpret_cast<char *>(anObject);
}
