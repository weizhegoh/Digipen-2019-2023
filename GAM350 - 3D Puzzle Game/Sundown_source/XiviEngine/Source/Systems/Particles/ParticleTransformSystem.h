/*****************************************************************
*\file         ParticleTransformSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
namespace Xivi
{
struct ParticleEmitter;
struct ParticleSettings;
}

namespace Xivi::Particle
{
struct ParticleTransformSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = Xivi::ECS::System::Type::Update
  {
    .m_id = 265,
    .m_name = "Particle Transform System"
  };
  //using All = ECS::Query::All<ParticleEmitter>;

  void operator()( ParticleEmitter &emitter, ParticleSettings &settings ) noexcept;
};
}