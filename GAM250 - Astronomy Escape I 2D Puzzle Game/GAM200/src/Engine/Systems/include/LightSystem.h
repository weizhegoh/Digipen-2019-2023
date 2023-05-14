/*****************************************************************
*\file		 LightSystem.h
*\brief		 LightSystem functions definitions.

*\author(s)   Lee Liang Ping	27 lines x % Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>

class LightSystem : public System
{
public:
	void Init();

	void Update();
};

class MainChar_LightSystem : public System
{
public:
	void Init();

	void Update();

	void ResetAmbient();
};

class KeyObjectives_LightSystem : public System
{
public:
	void Init();

	void Update();
};

class KeyObjectivesParts_LightSystem : public System
{
public:
	void Init();

	void Update();
};