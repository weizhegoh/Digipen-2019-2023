/*****************************************************************
*\file		 TextManager.cpp
*\brief		 TextManager functions definitions.

*\author(s)   Lee Liang Ping	175 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <TextManager.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;

void TextManager::AddText(FreeTypeList type,
	mathEngine::Vector3D position,
	float size,
	glm::vec3 color,
	Duration time,
	bool activate,
	FreeRenderType rendertype,
	FreeType_Font fontType,
	std::string text)
{
	auto it = textList.find(type);
	if (it != textList.end())
	{
		return;
	}
	textList.insert(std::make_pair(type, TextClass{ position, size, color, activate, time,rendertype, fontType, text, false }));
}

void TextManager::UpdateTextList()
{
	for (auto it = textList.begin(); it != textList.end();)
	{
		switch (it->second.lifetime.first)
		{
		case DurationType::LIFETIME_EXISTS:
			it->second.lifetime.second -= static_cast<float>(g_dt);
			if (it->second.lifetime.second <= 0.f)
				it->second.activte = false;
			break;
		case DurationType::NO_DURTION:
			break;
		}

		if (it->second.onremove)
			it = textList.erase(it);
		else
			++it;
	}
}

void TextManager::UpdateTextAndPosition(FreeTypeList type, std::string text, mathEngine::Vector3D position)
{
	UpdateText(type, text);
	UpdatePosition(type, position);
}

void TextManager::UpdatePosition(FreeTypeList type, mathEngine::Vector3D position)
{
	auto it = textList.find(type);
	if (it != textList.end())
		if (it->second.activte)
			it->second.position = position;
}

void TextManager::UpdateText(FreeTypeList type, std::string text)
{
	auto it = textList.find(type);
	if (it != textList.end())
		if (it->second.activte)
			it->second.text = text;
}

void TextManager::RemoveText(FreeTypeList type, RemoveOption option)
{
	auto it = textList.find(type);
	if (it != textList.end())
	{
		if (option == RemoveOption::DEACTIVATE)
		{
			it->second.lifetime.second = 0;
			it->second.activte = false;
		}
		else if (option == RemoveOption::REMOVE)
			it->second.onremove = true;
	}
}

void TextManager::ActivateText(FreeTypeList type, bool activation, DurationType durationtype, float time)
{
	auto it = textList.find(type);
	if (it != textList.end())
	{
		it->second.activte = activation;
		if(durationtype != DurationType::NO_SET)
			it->second.lifetime = std::make_pair(durationtype, time);
	}
}

void TextManager::RenderTextList()
{
	for (const auto& x : textList)
	{
		Graphics::FreeType& freetype = gFakeEngine->GetFreeTypeFont(x.second.fontType);
		if (x.second.activte)
			freetype.RenderText(
				x.second.text,
				x.second.position.x, x.second.position.y,
				x.second.scale,
				x.second.color,
				x.second.type);
	}
}

void TextManager::Reset()
{
	textList.clear();
}

void TextManager::HelperFunctionAddText()
{
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	AddText(
		FreeTypeList::GAMETIMER,
		mathEngine::Vector3D(-100.f, (float)window.GetHalfHeight(), 0.f),
		0.9f,
		glm::vec3(1.f, 1.f, 1.f),
		std::make_pair(DurationType::NO_DURTION, 0.f),
		true,
		FreeRenderType::SCREEN,
		FreeType_Font::OCR_A_EXTENDED);

	AddText(
		FreeTypeList::WIN_STATEMENT,
		mathEngine::Vector3D(-100.f, -50.f, 0.f),
		0.9f,
		glm::vec3(1.f, 0.f, 0.f),
		std::make_pair(DurationType::NO_DURTION, 0.f),
		false,
		FreeRenderType::MODEL_WORLD,
		FreeType_Font::OCR_A_EXTENDED,
		"You WIN");

	AddText(
		FreeTypeList::GOD_MODE_ACTIVATION,
		mathEngine::Vector3D(0, 0, 0),
		0.4f,
		glm::vec3(0.85f, 0.2f, 0.2f),
		std::make_pair(DurationType::NO_DURTION, 0.f),
		false,
		FreeRenderType::SCREEN,
		FreeType_Font::OCR_A_EXTENDED,
		"GOD MODE ACTIVATED");


	AddText(
		FreeTypeList::SPEED_MODE_ACTIVATION,
		mathEngine::Vector3D(0, 0, 0),
		0.4f,
		glm::vec3(0.85f, 0.2f, 0.2f),
		std::make_pair(DurationType::NO_DURTION, 0.f),
		false,
		FreeRenderType::SCREEN,
		FreeType_Font::OCR_A_EXTENDED,
		"SPEED MODE ACTIVATED");

	AddText(
		FreeTypeList::HACK_ACTIVATION,
		mathEngine::Vector3D(0, 0, 0),
		0.5f,
		glm::vec3(1.f, 0.f, 0.f),
		std::make_pair(DurationType::NO_DURTION, 0.f),
		false,
		FreeRenderType::SCREEN,
		FreeType_Font::OCR_A_EXTENDED,
		"HACK ACTIVATED");

	AddText(
		FreeTypeList::InventoryCollect,
		mathEngine::Vector3D(0, 0, 0),
		0.6f,
		glm::vec3(1.f, 1.f, 1.f),
		std::make_pair(DurationType::NO_DURTION, 0.f),
		true,
		FreeRenderType::SCREEN,
		FreeType_Font::OCR_A_EXTENDED,
		"Rock Saamples: ");

	AddText(
		FreeTypeList::Objectives,
		mathEngine::Vector3D(0, 0, 0),
		0.6f,
		glm::vec3(1.f, 1.f, 1.f),
		std::make_pair(DurationType::NO_DURTION, 0.f),
		true,
		FreeRenderType::SCREEN,
		FreeType_Font::OCR_A_EXTENDED,
		"Rock Saamples: ");

}