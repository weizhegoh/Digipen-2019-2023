/*****************************************************************
*\file         ParticleSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include "Xivi/Core/Declaration.h"
#include "Components/ParticleComponents.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
namespace Xivi::Graphics
{
class Manager;
class M_FrameBuffer;
}
namespace Xivi
{
struct TimerEngine;
}
namespace Xivi::Particle
{
DefineUpdateSystem_ID( ParticleSystem, 262 );
struct ClampParticlesValues : ECS::Event::Instance<ParticleSettings &>
{};
struct DestroyParticles : ECS::Event::Instance<>
{};
using Events = tuplet::tuple<ClampParticlesValues>;

Ptr<RM> m_resource_manager { nullptr };
Ptr< Graphics::M_FrameBuffer> m_framebuffer { nullptr };
Ptr< Graphics::Manager> m_graphic_manager { nullptr };
Ptr< TimerEngine> m_timer { nullptr };

using ParticleType = CList<ParticleEmitter, ParticleSettings, Position, Transform, AABB>;
void OnCreate() noexcept;
void operator()(
  ParticleEmitter &emitter,
  ParticleSettings &settings,
  Position &emitterPosition ) noexcept;

EndDefine();
}