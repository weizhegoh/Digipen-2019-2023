/*****************************************************************
*\file         Graphics.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

//For client Entry Point
//
//#include "API.h"
//#include "GraphicTypes.h"
//#include "Gfx.h"
//#include "Window.h"
//#include "ShaderManager.h"
//#include "TextureManager.h"
//#include "ShapeManager.h"
//#include "Application.h"
//#include "Camera3D.h"
//#include "../src/Collision/Geometry.h"

namespace Xivi::Graphics
{
enum class RendererAPI : u32
{
  OpenGL,
  Vulkan
};
}