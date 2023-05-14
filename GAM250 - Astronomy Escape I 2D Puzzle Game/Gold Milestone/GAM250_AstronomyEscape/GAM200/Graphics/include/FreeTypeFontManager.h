/*****************************************************************
*\file		 FreeTypeFontManager.h
*\brief		 Header file declarations for FreeTypeFontManager.

*\author(s)   Lee Liang Ping	7 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <Gametypes.h>
#include <FreeType.h>
namespace Graphics
{
	class FreeTypeFontManager
	{
		std::map<FreeType_Font, FreeType> freetypeFonts;
	public:
		void AddFont(FreeType_Font option, std::string file);
		FreeType& GetFont(const FreeType_Font index = FreeType_Font::OpenSansRegular);
	};
}
