/*****************************************************************
*\file         GraphicTypes.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include <rttr/registration.h>
#include <rttr/type.h>

namespace Xivi::Graphics
{
namespace Model
{
enum class StaticType
{
  CUBE = 0,
  SPHERE,
  PLANE,
  SKYBOX,
  SQUARE
};
}

enum class TextureType
{
  EMPTY = 0,
  TEST,
  TEST_GAMMA,
  //SKYBOX_TEXTURE
  SKYBOX_LT,
  SKYBOX_RT,
  SKYBOX_DN,
  SKYBOX_UP,
  SKYBOX_BK,
  SKYBOX_FT
};

enum class ShaderOption
{
  SHAPE = 0,
  TEXTURE,
  QUAD,
  NORMAL,
  AnimationWireFrame,
  ANIMATION,
  SKYBOX_SHADER,
  DEPTH_RTT,
  SHADOW_MAPPING,
  UI,
  LINE
};

enum class DrawOption
{
  POINTS = 0,
  LINE_STRIP,
  LINES,
  DEFAULT
};
}

namespace Xivi::Graphics::Type::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::enumeration<Graphics::Model::StaticType>( "Static Type" )
  (
  rttr::value( "CUBE", Graphics::Model::StaticType::CUBE ),
  rttr::value( "Sphere", Graphics::Model::StaticType::SPHERE ),
  rttr::value( "Plane", Graphics::Model::StaticType::PLANE ),
  rttr::value( "SkyBox", Graphics::Model::StaticType::SKYBOX )

  );

// Texture Type
rttr::registration::enumeration<Graphics::TextureType>( "Texture Type" )
(
  rttr::value( "Empty", Graphics::TextureType::EMPTY ),
  rttr::value( "TEST", Graphics::TextureType::TEST ),
  rttr::value( "TEST_GAMMA", Graphics::TextureType::TEST_GAMMA )
  );

// Shader Option
rttr::registration::enumeration<Graphics::ShaderOption>( "Shader Option" )
(
  rttr::value( "Shape", Graphics::ShaderOption::SHAPE ),
  rttr::value( "Texture", Graphics::ShaderOption::TEXTURE ),
  rttr::value( "Framebuffer", Graphics::ShaderOption::QUAD ),
  rttr::value( "Normal", Graphics::ShaderOption::NORMAL )
  );

// Draw Option
rttr::registration::enumeration<Graphics::DrawOption>( "Draw Option" )
(
  rttr::value( "Points", Graphics::DrawOption::POINTS ),
  rttr::value( "Line Strip", Graphics::DrawOption::LINE_STRIP ),
  rttr::value( "Lines", Graphics::DrawOption::LINES ),
  rttr::value( "Default", Graphics::DrawOption::DEFAULT )
  );
}
}