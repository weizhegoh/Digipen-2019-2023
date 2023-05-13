/*****************************************************************
*\file		 FreeTyoeFontManager.cpp
*\brief		 FreeTyoeFontManager functions definitions.

*\author(s)   Lee Liang Ping	15 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <FreeTypeFontManager.h>

void Graphics::FreeTypeFontManager::AddFont(FreeType_Font option, std::string file)
{
	auto temp = freetypeFonts.find(option);
	ARTLESS_ASSERT(temp == freetypeFonts.end(), "Font type is Registered");
	Graphics::FreeType font;
	font.Init(file);
	FreeTypeFontManager::freetypeFonts.insert({ option, font });
}
Graphics::FreeType& Graphics::FreeTypeFontManager::GetFont(const FreeType_Font index)
{
	ARTLESS_ASSERT(freetypeFonts.find(index) != freetypeFonts.end(), "pass in index > shader size");
	return freetypeFonts.find(index)->second;
}