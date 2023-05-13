/*****************************************************************
*\file		 RenderingSystem.h
*\brief		 Header file declarations for RenderingSystem.

*\author(s)   Lee Liang Ping	14 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <glslshader.h>
class RenderingSystem : public System
{
public:
	void Init(EntitySystemOption systemoption);

	void Update(EntitySystemOption systemoption);

	static void RenderObjects(const std::set<Entity>& renderList, EntitySystemOption systemoption);
	static void RenderObjects(const std::vector<Entity>& renderList, EntitySystemOption systemoption);

private:
	static void RenderDebug(Entity entity, EntitySystemOption& systemoption);
	static void RenderLight(Entity entity, EntitySystemOption& systemoption, GLSLShader& shdrpgm);
};