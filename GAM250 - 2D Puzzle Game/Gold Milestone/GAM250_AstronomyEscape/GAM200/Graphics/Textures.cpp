/*****************************************************************
*\file		 Textures.cpp
*\brief		 Textures functions definitions.

*\author(s)   Lee Liang Ping	14 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <Textures.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;

Textures::Textures(const std::string pFileName)
{
	pTex = Graphics::Gfx::GfxLoadTexture(pFileName.c_str());
	assert(pTex);
}

Textures::~Textures()
{
	Graphics::Gfx::RenderFunctions& gRender = gFakeEngine->GetGraphicManager<Graphics::Gfx::RenderFunctions>();
	gRender.DeleteVAO(pTex);
}