/*****************************************************************
*\file         TransformSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Components/GraphicsComponents.h"

namespace Xivi::Graphics
{
struct TransformSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = Xivi::ECS::System::Type::Update
  {
    .m_id = 216,
    .m_name = "Transform System"
  };
  ECS::Query::Instance m_query;
  struct ChildEvent : ECS::Event::Instance<>
  {};
  using Events = tuplet::tuple<ChildEvent>;

  bool test = false;
  void OnCreate() noexcept;
  void OnResume() noexcept;
  void OnUpdate() noexcept;
  void OnPostUpdate() noexcept;
};
}
