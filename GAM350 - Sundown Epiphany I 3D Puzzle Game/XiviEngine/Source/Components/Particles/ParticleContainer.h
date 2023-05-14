/*****************************************************************
*\file         ParticleContainer.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <Resource/Manager.h>
#include "Xivi/Core/Application.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
namespace Xivi
{
struct Particle_C
{
  Position position;
  float speed;
  Math::Vector3 direction;
  Math::Vector3 velocity;
  Transform transform;

  RenderSettings renderSettings;
  Colour colour;

  float lifetime;
  float starttime;
  bool isActive = false;
};

struct ParticleEmitter
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Particle Emitter"
  };
  ParticleEmitter()
  {
    particles.resize( 65 );
    auto &m_resource_manager = Service::Locator::Get<RM>();
    model = m_resource_manager.Get<Graphics::StaticModel>( "Assets/Models/binary/Object/Sphere.BinaryObj" );
    unitScales = model->unitScales;
    diffuse = m_resource_manager.Get<Graphics::Texture>( "Assets/Textures/default.dds" );
    specular = m_resource_manager.Get<Graphics::Texture>( "Assets/Textures/default.dds" );
  }
  std::vector<Particle_C> particles = {};
  float spawnInterval = 0.f;
  bool isActive = false;
  bool showDebugDraw = false;

  bool inBuildActive = false;
  //StaticMeshComponent mesh;
  //Material material;
  Resource::Handler<Graphics::StaticModel> model;
  Math::Vector3 unitScales;
  float shinness = 128.f;
  Resource::Handler<Graphics::Texture> diffuse;
  Resource::Handler<Graphics::Texture> specular;

  Math::Vector3 lastPositionDistance = Math::Vector3( std::numeric_limits<float>::max() );
};
}

namespace Xivi::Components::ParticleEmitter::Reflect
{
RTTR_REGISTRATION
{
  // Direction
  rttr::registration::class_<Xivi::ParticleEmitter>( std::string( Xivi::ParticleEmitter::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::ParticleEmitter>.m_id ) )
  .property( "isActive", &Xivi::ParticleEmitter::isActive )( rttr::policy::prop::as_reference_wrapper )
  .property( "Debug Draw", &Xivi::ParticleEmitter::showDebugDraw )( rttr::policy::prop::as_reference_wrapper )
  .property( "Model", &Xivi::ParticleEmitter::model )( rttr::policy::prop::as_reference_wrapper )
  .property( "Unit Scales", &Xivi::ParticleEmitter::unitScales )( rttr::policy::prop::as_reference_wrapper )
  .property( "Shinness", &Xivi::ParticleEmitter::shinness )( rttr::policy::prop::as_reference_wrapper )
  .property( "Diffuse", &Xivi::ParticleEmitter::diffuse )( rttr::policy::prop::as_reference_wrapper )
  .property( "Specular", &Xivi::ParticleEmitter::specular )( rttr::policy::prop::as_reference_wrapper );
}
}