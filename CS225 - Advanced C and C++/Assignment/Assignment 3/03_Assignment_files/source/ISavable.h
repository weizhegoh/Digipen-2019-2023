#ifndef _ISAVABLE_H_
#define _ISAVABLE_H_

#include <iostream>

struct ISavable
{
	virtual void save(std::ostream& stream) = 0;
	virtual ~ISavable() = default;
};

#endif // _ISAVABLE_H_
