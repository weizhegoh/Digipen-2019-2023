/*****************************************************************
*\file		 TextureManager.h
*\brief		 Header file declarations for TextureManager.

*\author(s)   Lee Liang Ping	10 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>
#include <Textures.h>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	void PushTextures(const TextureType objType, const std::string pFileName);
	Textures* GetTextures(const TextureType objType);
	void FreeTextures();
private:
	std::unordered_map<TextureType, std::unique_ptr<Textures>> textureList{};
};