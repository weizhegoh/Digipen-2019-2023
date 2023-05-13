/*****************************************************************
*\file         AnimationState.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
namespace Xivi
{
struct AnimationState
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Animation State"
  };

  enum class AnimState
  {
    anim_idle,
    anim_walking,
    anim_jump_start,
    anim_jump_loop,
    anim_jump_end,
    anim_running,
    anim_dab,
    anim_push,
    anim_clone,
    anim_open_switch,
    anim_close_switch
  };
  AnimState _animCurrentState = AnimState::anim_idle;
  AnimState _animPrevState;
  bool run = false;
  float walkingStep1, walkingStep2;
  bool walkonce = true;
  float runStep1, runStep2;
  bool runonce = true;
  float stepcounter;
  void UpdateState( AnimState newState )
  {
    _animPrevState = _animCurrentState;
    _animCurrentState = newState;
  }

  bool checkNotJump()
  {
    return
      (
      _animCurrentState != AnimationState::AnimState::anim_jump_start &&
      _animCurrentState != AnimationState::AnimState::anim_jump_loop &&
      _animCurrentState != AnimationState::AnimState::anim_jump_end
      );
  }

private:
  //float lastFrame;
};
}

namespace Xivi::Components::AnimationState::Reflect
{
RTTR_REGISTRATION
{
  // Colour
  rttr::registration::class_<Xivi::AnimationState>( std::string( Xivi::AnimationState::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::AnimationState>.m_id ) )
  .property( "walkingStep1", &Xivi::AnimationState::walkingStep1 )( rttr::policy::prop::as_reference_wrapper )
  .property( "walkingStep2", &Xivi::AnimationState::walkingStep2 )( rttr::policy::prop::as_reference_wrapper )
  .property( "runStep1", &Xivi::AnimationState::runStep1 )( rttr::policy::prop::as_reference_wrapper )
  .property( "runStep2", &Xivi::AnimationState::runStep2 )( rttr::policy::prop::as_reference_wrapper )
  ;
  //.property("Speed", &Xivi::AnimatorComponent::speed)(rttr::policy::prop::as_reference_wrapper);
}
}