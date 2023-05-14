/*****************************************************************
*\file		 TextureManager.cpp
*\brief		 TextureManager functions definitions.

*\author(s)   Lee Liang Ping	16 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <TextureManager.h>

TextureManager::TextureManager()
{
	//codes
}

TextureManager::~TextureManager()
{
	//codes
}



void TextureManager::PushTextures(const TextureType objType, const std::string pFileName)
{
	textureList[objType] = std::make_unique<Textures>(pFileName);
}

Textures* TextureManager::GetTextures(const TextureType objType)
{
	return textureList[objType].get();
}