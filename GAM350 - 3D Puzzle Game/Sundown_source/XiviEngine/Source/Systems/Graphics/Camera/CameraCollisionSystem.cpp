/*****************************************************************
*\file         CameraCollisionSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "CameraCollisionSystem.h"
#include "Components/GraphicsComponents.h"
namespace Xivi::Graphics
{
void CameraCollisionSystem::OnCreate() noexcept
{
  m_query.NoneOf<DebugDrawComponent, UI>();
}
void CameraCollisionSystem::OnUpdate() noexcept
{}
}