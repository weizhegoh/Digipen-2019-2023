/*****************************************************************
*\file         InterpolateSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Components/GraphicsComponents.h"
#include "Components/EngineComponents.h"
namespace Xivi::Graphics
{
struct InterpolateSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = Xivi::ECS::System::Type::Update
  {
    .m_id = 214,
    .m_name = "Interpolate System"
  };

  void OnStart() noexcept;
  void operator()(
    ECS::Entity &ent,
    PlayerControls &pc,
    Direction &dir,
    Transform &transform,
    Position &position ) noexcept;
};
}