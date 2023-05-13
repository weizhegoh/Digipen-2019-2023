/*****************************************************************
*\file         AnimatorComponent.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "string"
namespace Xivi
{
DefineTagComponent( AnimationUpdateOnce );
struct AnimatorComponent
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Animator"
  };
  float speed = 1.f;
  bool endFrame = false;
  bool isActive = true;
  float currentFrameTimer;
  float endFrameTimer;
  bool endFrameAni = false;

  int currentRun = 0;
  int numRun = -1;
  //bool once = false;
  Graphics::Animator animator;
  std::string nextAnimation;
  void UpdateAnimation( float deltatime )
  {
    //if (once && endFrame)
    //		return;
    animator.UpdateAnimation( deltatime * speed, endFrame, currentFrameTimer, endFrameTimer );
    if ( endFrame )
      endFrameAni = true;
  }
  void ActivateAnimation( int _numRun )
  {
    isActive = true;
    numRun = _numRun;
  }
  void ActivateAnimationNextRun( int _numRun, std::string next )
  {
    isActive = true;
    numRun = _numRun;
    nextAnimation = next;
  }
  int bitcollision = 0;
private:
};
}

namespace Xivi::Components::Animatoromponent::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::AnimatorComponent>( std::string( Xivi::AnimatorComponent::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::AnimatorComponent>.m_id ) )
  .property( "Speed", &Xivi::AnimatorComponent::speed )( rttr::policy::prop::as_reference_wrapper )
  .property( "isActive", &Xivi::AnimatorComponent::isActive )( rttr::policy::prop::as_reference_wrapper )
  .property( "Number of Runs", &Xivi::AnimatorComponent::numRun )( rttr::policy::prop::as_reference_wrapper );
}
}