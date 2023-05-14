/*****************************************************************
*\file		 ParticleSystem.h
*\brief		 Header file declarations for ParticleSystem.

*\author(s)   Lee Liang Ping	11 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>

class ParticleSystem : public System
{
public:
	void Init();

	void Update();

	void CreateParticles(CreateParticleEvent* s);
	void CreatePulseParticles(CreatePulseParticleEvent* s);

	void GenerationUpdate();

};