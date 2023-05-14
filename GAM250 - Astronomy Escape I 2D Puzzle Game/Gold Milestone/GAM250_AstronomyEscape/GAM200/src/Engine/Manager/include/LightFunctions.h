/*****************************************************************
*\file		 LightFunctions.h
*\brief		 Header file declarations for LightFunction.

*\author(s)   Lee Liang Ping	16 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <ComponentsHeader.h>

class LightEffects
{
public:

	void Init(const size_t& amount = 10);

	void RandomGenerate_NormalLight(Entity entity);
	void RandomGenerate_MCTrailLight(Entity entity);


	//void RenderLight(LightFlag& lightFlag, PointLight& pointLight, AABB& aabb, GLSLShader& shdrpgm);

private:
	void Create_NormalLight(size_t amount);
	void Create_MainCharLight();
	void Create_KeyObjectiveLight();


	Position RandomPosition(float x, float y);
	PointLight RandomPointLight(std::pair<float, float> maxAmbientBetween);
};