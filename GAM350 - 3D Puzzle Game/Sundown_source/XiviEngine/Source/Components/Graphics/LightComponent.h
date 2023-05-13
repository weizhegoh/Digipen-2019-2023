/*****************************************************************
*\file         LightComponent.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Xivi/Core/Utils/Math.h"

#include "Xivi/Graphics/Light/LightProperties.h"

namespace Xivi
{
  //DefineSingletonComponent(BrightnessContrast);
  //float bright = 1.f;
  //float contrast = 1.f;
  //Math::SIMD::Vec4 AverageLuminance = Math::SIMD::Vec4(0.5, 0.5, 0.5, 1.0);
  //EndDefine();

DefineSingletonComponent( GlobalAmbient );
Math::SIMD::Vec4 ambient = Math::SIMD::Vec4( 1.f, 1.f, 1.f, 1.f );;
EndDefine();

struct LightProperty
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "LightProperties"
  };
  //allow change
  Vec4f ambient = Vec4f { .5f, .5f, .5f, 1.f };
  Vec4f diffuse = Vec4f { .5f, .5f, .5f, 1.f };;
  Vec4f specular = Vec4f { .5f, .5f, .5f, 1.f };;
  f32 radius;
  f32 near_plane { 0.01f };
  f32 far_plane { 200.f };
  f32 bound { 200.f };
  Vec3f view_at { 0.f,0.f,0.f };
  Vec3f view_up { 0.f,1.f,0.f };
  bool toRenderShadow = false;
  unsigned int frameBufferID;
  unsigned int shadowMapID;
  bool toggleOff = true;
};

struct QuadraticFunction
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "QuadraticFunction"
  };
  //allow change
  float constant;
  float linear;
  float quadratic;
};

struct SpotLight_cutOff
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "SpotLight_cutOff"
  };
  //allow change
  float cutOff = 12.5f;
  float outerCutOff = 15.0f;
};

struct LightData
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "LightData"
  };

  //allow change
  std::vector<Entity> lightEntitiesList {};
  std::unordered_map<Entity, std::pair<Vec3f, Mat4x4>> lightData_map {};
  std::unordered_map<Entity, unsigned int> shadowMapID_map {};
  bool isValid = false;
};
}

namespace Xivi::Components::Light::Reflect
{
RTTR_REGISTRATION
{
// Light Property
rttr::registration::class_<Xivi::LightProperty>( std::string( Xivi::LightProperty::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::LightProperty>.m_id ) )
.property( "Ambient", &LightProperty::ambient )( rttr::policy::prop::as_reference_wrapper )
.property( "Diffuse", &LightProperty::diffuse )( rttr::policy::prop::as_reference_wrapper )
.property( "Specular", &LightProperty::specular )( rttr::policy::prop::as_reference_wrapper )
.property( "Radius", &LightProperty::radius )( rttr::policy::prop::as_reference_wrapper )
.property( "Near Plane", &LightProperty::near_plane )( rttr::policy::prop::as_reference_wrapper )
.property( "Far Plane", &LightProperty::far_plane )( rttr::policy::prop::as_reference_wrapper )
.property( "Bound", &LightProperty::bound )( rttr::policy::prop::as_reference_wrapper )
.property( "View Up", &LightProperty::view_at )( rttr::policy::prop::as_reference_wrapper )
.property( "View At", &LightProperty::view_up )( rttr::policy::prop::as_reference_wrapper )
.property( "Light Render Shadow", &LightProperty::toRenderShadow )( rttr::policy::prop::as_reference_wrapper );

    // Quadratic Function
    rttr::registration::class_<Xivi::QuadraticFunction>( std::string( Xivi::QuadraticFunction::info.m_name ) )
    ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::QuadraticFunction>.m_id ) )
    .property( "Constant", &QuadraticFunction::constant )( rttr::policy::prop::as_reference_wrapper )
    .property( "Linear", &QuadraticFunction::linear )( rttr::policy::prop::as_reference_wrapper )
    .property( "Quadratic", &QuadraticFunction::quadratic )( rttr::policy::prop::as_reference_wrapper );

    // Spot Light Cut Off
    rttr::registration::class_<Xivi::SpotLight_cutOff>( std::string( Xivi::SpotLight_cutOff::info.m_name ) )
    ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::SpotLight_cutOff>.m_id ) )
    .property( "Cut Off", &SpotLight_cutOff::cutOff )( rttr::policy::prop::as_reference_wrapper )
    .property( "Outer Cut Off", &SpotLight_cutOff::outerCutOff )( rttr::policy::prop::as_reference_wrapper );

    // Light Property
    rttr::registration::class_<Xivi::GlobalAmbient>( std::string( Xivi::GlobalAmbient::info.m_name ) )
    ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GlobalAmbient>.m_id ) )
    .property( "Ambient", &GlobalAmbient::ambient )( rttr::policy::prop::as_reference_wrapper )
  ;
}
}
