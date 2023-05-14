/*****************************************************************
*\file         LightProperties.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::Graphics
{
  //varabile contian Light properties
struct LightProperties
{
  Vec4f ambient;
  Vec4f diffuse;
  Vec4f specular;
};
struct QuadFunction
{
  float constant;
  float linear;
  float quadratic;
};

struct DirLight : virtual LightProperties
{
  Vec3f direction;
  Vec3f position;
};

struct PointLight :
  virtual LightProperties,
  virtual QuadFunction
{
  Vec3f position;
};

struct SpotLight :
  virtual LightProperties,
  virtual QuadFunction
{
  Vec3f position;
  Vec3f direction;
  float cutOff = 12.5f;
  float outerCutOff = 15.0f;
};

using PointLights = std::vector<PointLight>;
using DirLights = std::vector<DirLight>;
}

namespace Xivi::Graphics::Lights::Reflect
{
RTTR_REGISTRATION
{
// Light Properties
rttr::registration::class_<Graphics::LightProperties>( "Light Properties" )
.constructor<>()( rttr::policy::ctor::as_object )
.property( "Ambient", &Graphics::LightProperties::ambient )( rttr::policy::prop::as_reference_wrapper )
.property( "Diffuse", &Graphics::LightProperties::diffuse )( rttr::policy::prop::as_reference_wrapper )
.property( "Specular", &Graphics::LightProperties::specular )( rttr::policy::prop::as_reference_wrapper );

// Quadratic Function
rttr::registration::class_<Graphics::QuadFunction>( "Quad Function" )
.constructor<>()( rttr::policy::ctor::as_object )
.property( "Constant", &Graphics::QuadFunction::constant )( rttr::policy::prop::as_reference_wrapper )
.property( "Linear", &Graphics::QuadFunction::linear )( rttr::policy::prop::as_reference_wrapper )
.property( "Quadratic", &Graphics::QuadFunction::quadratic )( rttr::policy::prop::as_reference_wrapper );
}
}