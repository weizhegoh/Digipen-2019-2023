/*****************************************************************
*\file		 ShaderManager.h
*\brief		 Header file declarations for ShaderManager.

*\author(s)   Lee Liang Ping	13 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <glslshader.h>
#include <GameTypes.h>
class ShaderManager
{
	std::map<ShaderOption, GLSLShader> shdrpgms;
public:
	//using VPSS = std::vector<std::pair<std::string, std::string>>;
	ShaderManager() = default;
	~ShaderManager();
	void AddShader(ShaderOption option, std::string vert, std::string frag);
	GLSLShader& GetShader(const ShaderOption index = ShaderOption::SHAPE);
};
