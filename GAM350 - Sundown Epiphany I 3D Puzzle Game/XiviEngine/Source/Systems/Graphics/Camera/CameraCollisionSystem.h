/*****************************************************************
*\file         CameraCollisionSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "CameraSystem.h"
namespace Xivi::Graphics
{
struct CameraCollisionSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::ChildUpdate<CameraSystem, CameraSystem::ChildEvent>
  {
    .m_id = 201,
    .m_name = "Camera Collision System"
  };
  //using None = ECS::Query::None<DebugDrawComponent, UI>;
  ECS::Query::Instance m_query;

  void OnCreate() noexcept;
  void OnUpdate() noexcept;
};
}