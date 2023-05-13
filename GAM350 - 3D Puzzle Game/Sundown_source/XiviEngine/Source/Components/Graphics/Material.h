/*****************************************************************
*\file         Material.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Xivi/Graphics/Texture/Texture.h"
#include <Resource/Manager.h>
namespace Xivi
{
struct Material
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Material"
  };
  //allow change
  float shinness = 128.f;
  //Assign by assetLoader
  Resource::Handler<Graphics::Texture> diffuse;
  Resource::Handler<Graphics::Texture> specular;
  Resource::Handler<Graphics::Texture> normal;
  Resource::Handler<Graphics::Texture> bump;
  bool parallaxMappingOn = false;
  float height_scale = 0.f;
};

struct SkyBoxMaterial
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "SkyBox Material"
  };
  //Assign by assetLoader
  Resource::Handler<Graphics::Texture> skybox_LT;
  Resource::Handler<Graphics::Texture> skybox_RT;
  Resource::Handler<Graphics::Texture> skybox_DN;
  Resource::Handler<Graphics::Texture> skybox_UP;
  Resource::Handler<Graphics::Texture> skybox_BK;
  Resource::Handler<Graphics::Texture> skybox_FT;
};
}

namespace Xivi::TextureResource::Reflect
{
RTTR_REGISTRATION
{
  ASSETREFLECT( Xivi::Resource::Handler<Graphics::Texture> );
}
}

namespace Xivi::Components::Material::Reflect
{
RTTR_REGISTRATION
{
  // Material
  rttr::registration::class_<Xivi::Material>( std::string( Xivi::Material::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Material>.m_id ) )
  .constructor<>()( rttr::policy::ctor::as_object )
  .property( "shinness", &Xivi::Material::shinness )( rttr::policy::prop::as_reference_wrapper )
  .property( "diffuse", &Xivi::Material::diffuse )( rttr::policy::prop::as_reference_wrapper )
  .property( "specular", &Xivi::Material::specular )( rttr::policy::prop::as_reference_wrapper )
  .property( "normal", &Xivi::Material::normal )( rttr::policy::prop::as_reference_wrapper )
  .property( "bump", &Xivi::Material::bump )( rttr::policy::prop::as_reference_wrapper )
  .property( "Parallax Map", &Xivi::Material::parallaxMappingOn )( rttr::policy::prop::as_reference_wrapper )
  .property( "Height Scale", &Xivi::Material::height_scale )( rttr::policy::prop::as_reference_wrapper )
  ;

  // Skybox
  rttr::registration::class_<Xivi::SkyBoxMaterial>( std::string( Xivi::SkyBoxMaterial::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::SkyBoxMaterial>.m_id ) )
  .constructor<>()( rttr::policy::ctor::as_object )
  .property( "skybox_LT", &Xivi::SkyBoxMaterial::skybox_LT )
  .property( "skybox_RT", &Xivi::SkyBoxMaterial::skybox_RT )
  .property( "skybox_DN", &Xivi::SkyBoxMaterial::skybox_DN )
  .property( "skybox_UP", &Xivi::SkyBoxMaterial::skybox_UP )
  .property( "skybox_BK", &Xivi::SkyBoxMaterial::skybox_BK )
  .property( "skybox_FT", &Xivi::SkyBoxMaterial::skybox_FT );
}
}