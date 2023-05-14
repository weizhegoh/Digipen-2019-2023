/*****************************************************************
*\file         AnimationStateSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "../Sundown/Source/NComponents/Entity/Player.h"
namespace Xivi
{
struct AnimationState;
struct AnimatorComponent;
struct AnimationMeshComponent;
}
namespace Xivi::Graphics
{
class Manager;
struct AnimationStateSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = Xivi::ECS::System::Type::Update
  {
    .m_id = 402,
    .m_name = "Animation State System"
  };

  Ptr<Graphics::Manager> m_graphic_manager;

  void OnCreate() noexcept;
  void operator()(
    Player &player,
    AnimationState &anim_state,
    AnimatorComponent &animator,
    AnimationMeshComponent &mesh ) noexcept;
};
}