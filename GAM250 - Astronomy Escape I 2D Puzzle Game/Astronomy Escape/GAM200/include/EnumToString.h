/*****************************************************************
*\file		 EnumToString.h
*\brief		 Overload function for enum to string

*\author(s)   Lee Jun Jie	16 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>

namespace EnumToString
{
	std::string Get(const ObjectState objState);
	std::string Get(const ObjectDirection objDir);
	std::string Get(const ZoomOption zoomOption);
	std::string Get(const CollisionType shaderOption);
	std::string Get(const TimerMap shaderOption);
	std::string Get(const EntitySystemOption shaderOption);
	std::string Get(const FreeRenderType shaderOption);
	std::string Get(const StagesEnum state);
	std::string Get(const ParticleAction action);
	std::string Get(const ParticleTypesEnum type);
}