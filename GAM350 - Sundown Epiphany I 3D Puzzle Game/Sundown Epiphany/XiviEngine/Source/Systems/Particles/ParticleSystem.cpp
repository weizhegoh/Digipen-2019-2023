/*****************************************************************
*\file         ParticleSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "ParticleSystem.h"
#include "Xivi/Core/Declaration.h"
#include <Resource/Manager.h>
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Xivi/Core/Timer/TimerEngine.h"
#include "Components/EngineComponents.h"
#include "Components/ParticleComponents.h"
#include "Components/GraphicsComponents.h"

namespace Xivi::Particle
{
void ParticleSystem::OnCreate() noexcept
{
  m_resource_manager = &Service::Locator::Get<RM>();
  m_framebuffer = &Service::Locator::Get<Graphics::M_FrameBuffer>();
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
  m_timer = &Service::Locator::Get<TimerEngine>();
}
void ParticleSystem::operator()(
  ParticleEmitter &emitter,
  ParticleSettings &settings,
  Position &emitterPosition ) noexcept
{
  settings.count = Math::Helper::MathFunction::clamp( settings.count, 0, 64 );
  settings.rateOfSpawn = Math::Helper::MathFunction::clamp( settings.rateOfSpawn, 0, settings.count );
  settings.transparency.first = Math::Helper::MathFunction::clamp( settings.transparency.first, 0.f, 1.f );
  settings.transparency.second = Math::Helper::MathFunction::clamp( settings.transparency.second, 0.f, 1.f );
  Math::Helper::MathFunction::clamp( settings.direction.second, -1.f, 1.f );
  Math::Helper::MathFunction::clamp( settings.direction.second, -1.f, 1.f );
  Math::Helper::MathFunction::clamp( settings.colour, 0.f, 1.f );

  if ( !emitter.isActive )
    return;

  auto &rateOverDistance = settings.rateOverDistance;
  auto &rateOfSpawn = settings.rateOfSpawn;
  bool performROD = ( rateOverDistance > 0.f );
  auto dt = m_timer->GetScaledDeltaTime();
  bool startSpawn = true;

  auto RandBetweenVec3 = Math::Helper::RandFunction::RandBetweenVec3;
  auto RandBetweenFloat = Math::Helper::RandFunction::RandBetweenFloat;

  auto &speedValues = settings.speed;
  auto &lifeTimeValues = settings.lifetime;
  auto &startTimeValues = settings.starttime;
  auto &directionValues = settings.direction;
  auto &scaleValues = settings.scale;
  auto &transparencyValues = settings.transparency;
  auto &spawnInterval = settings.spawnInterval;
  auto &colourValue = settings.colour;
  auto &positionOffSet = settings.positionOffSet;
  auto &positionOffSet2 = settings.positionOffSet2;
  auto &axisValue = settings.axis;

  if ( performROD )
  {
    auto distance = Math::distanceSq(
      emitterPosition.m_position,
      emitter.lastPositionDistance );

    startSpawn = ( distance >= rateOverDistance * rateOverDistance );
    if ( startSpawn )
      emitter.lastPositionDistance = emitterPosition.m_position;
  }
  else
  {
    emitter.spawnInterval -= dt;
    if ( emitter.spawnInterval <= 0 )
      emitter.spawnInterval = spawnInterval;
    else
      startSpawn = false;
  }

  if ( startSpawn )
  {
    int particleCount = 0;
    for ( size_t i = 0; i < settings.count; ++i )
    {
      auto &particle = emitter.particles[i];
      if ( particle.isActive ) continue;

      auto &position = particle.position.m_position;
      auto &direction = particle.direction;
      auto &velocity = particle.velocity;
      auto &speed = particle.speed;
      auto &starttime = particle.starttime;
      auto &lifetime = particle.lifetime;
      auto &isRender = particle.renderSettings.onRender;
      auto &tranparency = particle.renderSettings.transparent;
      auto &scale = particle.transform.scale;
      auto &colour = particle.colour.colour;
      auto &axis = particle.transform.axis;
      isRender = false;
      particle.isActive = true;
      starttime = RandBetweenFloat( startTimeValues );
      lifetime = RandBetweenFloat( lifeTimeValues );
      speed = RandBetweenFloat( speedValues );
      direction = RandBetweenVec3( directionValues ).normalize();
      tranparency = RandBetweenFloat( transparencyValues );
      axis = RandBetweenVec3( axisValue );
      scale = Math::Vector3( RandBetweenFloat( scaleValues ) );
      auto whichPos = ( RandBetweenFloat( Math::Helper::InBETWN_Float( { 0.f,1.f } ) ) <= 0.5f ) ? positionOffSet : positionOffSet2;
      position = emitterPosition.m_position - whichPos;
      colour = colourValue;
      ++particleCount;
      if ( particleCount >= rateOfSpawn )
        break;
    }
  }

  for ( size_t i = 0; i < settings.count; ++i )
  {
    auto &particle = emitter.particles[i];

    if ( !particle.isActive ) continue;

    auto &position = particle.position.m_position;
    auto &direction = particle.direction;
    auto &velocity = particle.velocity;
    auto &speed = particle.speed;
    auto &starttime = particle.starttime;
    auto &lifetime = particle.lifetime;
    auto &isRender = particle.renderSettings.onRender;

    lifetime -= dt; starttime -= dt;
    if ( starttime <= 0.f )
      isRender = true;
    if ( lifetime <= 0.f )
    {
      isRender = false;
      particle.isActive = false;
    }
    velocity = direction * speed * dt;
    position += velocity;
  }
}
}