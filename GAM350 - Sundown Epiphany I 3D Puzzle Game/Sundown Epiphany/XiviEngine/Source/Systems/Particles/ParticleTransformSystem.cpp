/*****************************************************************
*\file         ParticleTransformSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "ParticleTransformSystem.h"
#include "Components/ParticleComponents.h"
#include "Xivi/Graphics/Helper/GraphicHelper.h"
namespace Xivi::Particle
{
void ParticleTransformSystem::operator()( ParticleEmitter &emitter, ParticleSettings &settings ) noexcept
{
  if ( !emitter.isActive )
    return;
  auto &unitScale = emitter.unitScales;
  for ( size_t i = 0; i < settings.count; ++i )
  {
    auto &particle = emitter.particles[i];
    if ( !particle.isActive )
      continue;
    auto &pos = particle.position;
    auto &transform = particle.transform;

    Math::Matrix4 model( 1.f ); Math::Matrix4 scale( 1.f ); Math::Matrix4 rotate( 1.f );
    Math::Vector3 posTemp = pos.m_position;
    Math::Vector3 scaleTemp = transform.scale;
    model = model.translate( posTemp.getX(), posTemp.getY(), posTemp.getZ() );
    Vec3f axisRad = Vec3f(
      Math::DegToRad( transform.axis.getX() ),
      Math::DegToRad( transform.axis.getY() ),
      Math::DegToRad( transform.axis.getZ() ) );
    rotate = Math::Helper::MathFunction::ToRotationMatrix( axisRad );
    scale = scale.scale( scaleTemp.getX(), scaleTemp.getY(), scaleTemp.getZ() );
    transform.LocalToWorld = ( model * rotate * scale );
  }
}
}