/*****************************************************************
*\file		 Shapes.cpp
*\brief		 Shapes functions definitions.

*\author(s)   Lee Liang Ping	30 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <Shapes.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;

Shapes::Shapes(ObjectShapes objShapes) :pMesh{NULL}, pHotspot{NULL}
{
	(void)objShapes;
	switch (objShapes)
	{
	case ObjectShapes::SQUARE:
		pMesh = Graphics::Gfx::GfxLoadFixedModel2();
		break;
	case ObjectShapes::SQUARELESS:
		pMesh = Graphics::Gfx::GfxLoadFixedModel("Assets/meshes/square.msh");
		break;
	case ObjectShapes::CIRCLE:
		pMesh = Graphics::Gfx::GfxLoadFixedModel("Assets/meshes/circle.msh");	
		break;
	case ObjectShapes::TRIANGLE:
		pMesh = Graphics::Gfx::GfxLoadFixedModel("Assets/meshes/triangle.msh");
		break;

	}
	pHotspot = Graphics::Gfx::GfxBuildHotspotModel();
}

Shapes::~Shapes()
{
	Graphics::Gfx::RenderFunctions& gRender = gFakeEngine->GetGraphicManager<Graphics::Gfx::RenderFunctions>();
	gRender.DeleteVAO(pMesh.vaoid);
	gRender.DeleteVAO(pHotspot.vaoid);
}
