/*****************************************************************
*\file         PatricleSettings.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
namespace Xivi
{
  //Settings setups
using InBETWN_Vec3 = std::pair<Math::Vector3, Math::Vector3>;
using InBETWN_Vec2 = std::pair<Math::Vector2, Math::Vector2>;
using InBETWN_Float = std::pair<float, float>;
using InBETWN_Int = std::pair<int, int>;
using FollowEntity = std::pair<bool, ECS::Entity>;
template<typename TYPE, typename InBETWN>
struct ParticleSetting
{
  TYPE type;
  InBETWN values;
};

//Component
struct ParticleSettings
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Particle Settings"
  };
  int count = 0;
  int rateOverDistance = 0;
  int rateOfSpawn = 3;
  float spawnInterval = 1.f;
  int spawnTimes = 0;
  int currentSpawnTime = 0;
  InBETWN_Vec3 direction;
  InBETWN_Float speed;
  InBETWN_Float lifetime;
  InBETWN_Float starttime;
  InBETWN_Float transparency;
  InBETWN_Float scale;
  InBETWN_Vec3 axis;
  Math::Vector3 colour;
  FollowEntity followEntity;
  Math::Vector3 positionOffSet;
  Math::Vector3 positionOffSet2;
  //... tbc
};
}

namespace Xivi::Components::ParticleSettings::Reflect
{
RTTR_REGISTRATION
{
  // InBETWN_Vec3
  rttr::registration::class_<InBETWN_Vec3>( "InBETWN_Vec3" )
  .property( "First", &InBETWN_Vec3::first )
  .property( "Second", &InBETWN_Vec3::second );

  // InBETWN_Vec2
  rttr::registration::class_<InBETWN_Vec2>( "InBETWN_Vec2" )
  .property( "First", &InBETWN_Vec2::first )
  .property( "Second", &InBETWN_Vec2::second );

  // InBETWN_Float
  rttr::registration::class_<InBETWN_Float>( "InBETWN_Float" )
  .property( "First", &InBETWN_Float::first )
  .property( "Second", &InBETWN_Float::second );

  // InBETWN_Int
  rttr::registration::class_<InBETWN_Int>( "InBETWN_Int" )
  .property( "First", &InBETWN_Int::first )
  .property( "Second", &InBETWN_Int::second );

  // FollowEntity
  rttr::registration::class_<FollowEntity>( "FollowEntity" )
  .property( "First", &FollowEntity::first )
  .property( "Second", &FollowEntity::second );

  // ParticleSettings
  rttr::registration::class_<Xivi::ParticleSettings>( std::string( Xivi::ParticleSettings::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::ParticleSettings>.m_id ) )
  .property( "Count", &Xivi::ParticleSettings::count )( rttr::policy::prop::as_reference_wrapper )
  .property( "Rate Over Distance", &Xivi::ParticleSettings::rateOverDistance )( rttr::policy::prop::as_reference_wrapper )
  .property( "Rate of Spawn", &Xivi::ParticleSettings::rateOfSpawn )( rttr::policy::prop::as_reference_wrapper )
  .property( "Spawn Interval", &Xivi::ParticleSettings::spawnInterval )( rttr::policy::prop::as_reference_wrapper )
  .property( "Direction", &Xivi::ParticleSettings::direction )( rttr::policy::prop::as_reference_wrapper )
  .property( "Speed", &Xivi::ParticleSettings::speed )( rttr::policy::prop::as_reference_wrapper )
  .property( "Life Time", &Xivi::ParticleSettings::lifetime )( rttr::policy::prop::as_reference_wrapper )
  .property( "Start Time", &Xivi::ParticleSettings::starttime )( rttr::policy::prop::as_reference_wrapper )
  .property( "Transparency", &Xivi::ParticleSettings::transparency )( rttr::policy::prop::as_reference_wrapper )
  .property( "Scale", &Xivi::ParticleSettings::scale )( rttr::policy::prop::as_reference_wrapper )
  .property( "Colour", &Xivi::ParticleSettings::colour )( rttr::policy::prop::as_reference_wrapper )
  .property( "Position Spawn OffSet", &Xivi::ParticleSettings::positionOffSet )( rttr::policy::prop::as_reference_wrapper )
  .property( "Position Spawn OffSet2", &Xivi::ParticleSettings::positionOffSet2 )( rttr::policy::prop::as_reference_wrapper )
  .property( "Axis", &Xivi::ParticleSettings::axis )( rttr::policy::prop::as_reference_wrapper )
    ;
}
}