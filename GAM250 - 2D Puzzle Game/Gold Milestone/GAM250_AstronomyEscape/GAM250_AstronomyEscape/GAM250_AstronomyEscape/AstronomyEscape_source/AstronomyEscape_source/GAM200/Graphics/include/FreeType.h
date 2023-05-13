/*****************************************************************
*\file		 FreeType.h
*\brief		 Header file declarations for FreeType.

*\author(s)   Lee Liang Ping	10 lines x 100% Code contribution

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <Character.h>
#include <GameTypes.h>
namespace Graphics
{
	class FreeType
	{
		std::map<char, Character> Characters;
		unsigned int  VAO;
		unsigned int  VBO;
	public:
		void Init(std::string file);
		void RenderText(std::string text, float x, float y, float scale, glm::vec3 color, FreeRenderType type);
	};
	//class FreeType
	//{
	//	std::map<char, Character> Characters;
	//	unsigned int  VAO;
	//	unsigned int  VBO;
	//	unsigned int  textureId;

	//	int atlas_width;
	//	int atlas_height;
	//	std::vector<float> vertices;
	//	std::vector<float> normVerts;
	//public:
	//	void Init(std::string file);
	//	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color, FreeRenderType type);
	//	float normaliseFloat(float num, float min, float max);
	//};
}

