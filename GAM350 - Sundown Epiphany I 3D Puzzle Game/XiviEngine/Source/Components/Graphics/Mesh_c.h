/*****************************************************************
*\file         Mesh_c.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Xivi/Graphics/Model/Model.h"
#include <Resource/Manager.h>
namespace Xivi
{
struct StaticMeshComponent
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Static Mesh"
  };
  Resource::Handler<Graphics::StaticModel> model;
  Math::Vector3 unitScales;
};

struct AnimationMeshComponent
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Animation Mesh"
  };
  Resource::Handler<Graphics::AnimationModel> model;
  Math::Vector3 unitScales;
};
}

#define ASSETREFLECT(asset) rttr::registration::class_<asset>(XV_STRINGIFY(asset)) \
.property("Resource Key", &asset::GetKey, &asset::SetKey) \

// but how to deduce the type tho
namespace Xivi::MeshAssets::Reflect
{
RTTR_REGISTRATION
{
  ASSETREFLECT( Xivi::Resource::Handler<Graphics::StaticModel> );
  ASSETREFLECT( Xivi::Resource::Handler<Graphics::AnimationModel> );
}
}

namespace Xivi::Components::Mesh::Reflect
{
RTTR_REGISTRATION
{
  // Mesh
  rttr::registration::class_<Xivi::StaticMeshComponent>( std::string( Xivi::StaticMeshComponent::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::StaticMeshComponent>.m_id ) )
  .property( "Asset", &Xivi::StaticMeshComponent::model )( rttr::policy::prop::as_reference_wrapper )
  .property( "Unit Scales", &Xivi::StaticMeshComponent::unitScales )( rttr::policy::prop::as_reference_wrapper );

rttr::registration::class_<Xivi::AnimationMeshComponent>( std::string( Xivi::AnimationMeshComponent::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::AnimationMeshComponent>.m_id ) )
  .property( "Model", &Xivi::AnimationMeshComponent::model )( rttr::policy::prop::as_reference_wrapper )
  .property( "Unit Scales", &Xivi::AnimationMeshComponent::unitScales )( rttr::policy::prop::as_reference_wrapper );
}
}