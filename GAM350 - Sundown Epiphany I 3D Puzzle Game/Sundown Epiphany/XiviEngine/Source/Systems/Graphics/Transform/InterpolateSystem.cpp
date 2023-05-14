/*****************************************************************
*\file         InterpolateSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "InterpolateSystem.h"
#include "Components/GraphicsComponents.h"
#include "Components/EngineComponents.h"
#include "Xivi/Graphics/Helper/GraphicHelper.h"

namespace Xivi::Graphics
{
void InterpolateSystem::OnStart() noexcept
{}

void InterpolateSystem::operator()(
  ECS::Entity &ent,
  PlayerControls &pc,
  Direction &dir,
  Transform &transform,
  Position &position ) noexcept
{
  if ( m_paused )
    return;
  //auto angle = cosf(dir.m_dir / dir.offset);
  //Vec3f b = dir.offset - dir.m_dir;
  //auto res = dir.m_dir.dot(dir.offset);
  //auto lenA = std::abs(Math::length(dir.m_dir));
  //auto lenb = std::abs(Math::length(dir.offset));

  //if (lenA == lenb)
  //	return;
  //auto angle = std::abs(Math::ArcCos(res / (lenA * lenb)));
  auto speed = dir.speed;
  //if (angle >= Math::s_PI * 0.5f)
    //speed *= 0.5f;
  dir.m_dir = Math::SIMD::lerp( dir.m_dir, dir.offset, speed );
  //if (HaveComponent<Camera_c>(ent))
  //{
  //	Camera_c& camera_c = GetComponent<Camera_c>(ent);
  //	camera_c.offsetY = Math::RadToDeg(-std::atan2(dir.m_dir.getX(), dir.m_dir.getZ()));
  //	float setY = Math::Helper::MathFunction::LinearInterpolate(transform.axis.getY(), camera_c.offsetY, speed);
  //	transform.axis.setY(setY);
  //}
}
}