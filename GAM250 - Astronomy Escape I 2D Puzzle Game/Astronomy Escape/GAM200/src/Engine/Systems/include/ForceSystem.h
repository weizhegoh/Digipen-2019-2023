#pragma once
#include <System.h>

class ForceSystem : public System
{
public:
	void Init();

	void Update(EntitySystemOption systemoption);
};
