/*****************************************************************
*\file		 TextManager.h
*\brief		 Header file declarations for TextManager.

*\author(s)   Lee Liang Ping	31 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <Gametypes.h>
#include <FreeType.h>
#include <TextClass.h>

extern float g_dt;
class TextManager
{
	//True = life timeexist
	//False = lifetime does not exist
	using Duration = std::pair<DurationType, float>;
	std::map<FreeTypeList, TextClass> textList;
public:
	void AddText(FreeTypeList type,
		mathEngine::Vector3D position,
		float size,
		glm::vec3 color,
		Duration time,
		bool activate,
		FreeRenderType rendertype,
		FreeType_Font fontType,
		std::string text = "No Update Text"
	);

	void UpdateTextAndPosition(FreeTypeList type, std::string text, mathEngine::Vector3D position);
	void UpdateText(FreeTypeList type, std::string text);
	void UpdatePosition(FreeTypeList type, mathEngine::Vector3D position);
	void RemoveText(FreeTypeList type, RemoveOption option);
	void ActivateText(FreeTypeList type, bool activation, DurationType durationtype = DurationType::NO_SET, float time = 0.f);
	void UpdateTextList();
	void RenderTextList();
	void Reset();

	void HelperFunctionAddText();
};

