/*****************************************************************
*\file		 TransformSystem.h
*\brief		 Header file declarations for TransformSystem.

*\author(s)  Lee Liang Ping	9 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
class TransformSystem : public System
{
public:
	void Init(EntitySystemOption systemoption);

	void Update(EntitySystemOption systemoption);

	void transform(Entity entity, EntitySystemOption systemoption);
};