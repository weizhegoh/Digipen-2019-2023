#ifndef _IFORMATTERELEMENT_H_
#define _IFORMATTERELEMENT_H_

#include "ISavable.h"

class IFormatterElement : public ISavable
{
public:
	virtual ~IFormatterElement() = default;
};

#endif // _IFORMATTERELEMENT_H_