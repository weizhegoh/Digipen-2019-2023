/*****************************************************************
*\file         PlayerControls.h
*\author(s)    Tan Tong Wee
         Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi
{
struct PlayerControls
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Player Controls"
  };

  enum Movement_ActionMap
  {
    Up,
    Down,
    Left,
    Right,
    None
  };

  bool holding_box;
  Entity box_entity;
  Movement_ActionMap moveAction;

  //remove in the future...

  float speed = 1.f;
  float jump = 5.f;
  float sprintMultiplier = 2.f;

  float toShrinkScale = 3.0f;
  float toGrowScale = 2.0f;

  float jumpGrow = 1200.f;
  float originalJump = 500.f;

  Vec3f initialScale;
  Vec3f initPos;
  bool usedAbility = false;
  bool player_1 = false;

  bool duringScaling = false;
  bool duringScalingBack = false;
  Vec3f newScale;

  float abilityTime = 3.0f;
  float testTime = 0.0f;
  int bitCollision = 0; //for kenric

  enum growAbility
  {
    SHRINK,
    NORMAL,
    BIG
  };

  growAbility currentState;
  int player1Ability = 1;
  int player1AbilityLast = 0;
  int player1AbilityPrev = 1;

  bool duringGrowing = false;
  bool duringShrinking = false;

  float newPos;
  bool duringGrowingPos = false;
  bool duringShrinkingPos = false;

  bool shadowCreated = false;

  Entity m_clone;

  bool shadowDestroy = false;
  Vec3f clone_pos;
  Vec3f clone_aabb;
  Vec3f clone_transform;

  bool isLinked;
  bool isJump;
  float currentScalingFactor;
};
}

namespace Xivi::Components::PlayerControls::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::PlayerControls>( std::string( Xivi::PlayerControls::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::PlayerControls>.m_id ) )
  .property( "Player 1", &Xivi::PlayerControls::player_1 )( rttr::policy::prop::as_reference_wrapper )
  .property( "Speed", &Xivi::PlayerControls::speed )( rttr::policy::prop::as_reference_wrapper )
  .property( "Sprint Multiplier", &Xivi::PlayerControls::sprintMultiplier )( rttr::policy::prop::as_reference_wrapper )
  .property( "Jump", &Xivi::PlayerControls::jump )( rttr::policy::prop::as_reference_wrapper )
  .property( "Growth Jump", &Xivi::PlayerControls::jumpGrow )( rttr::policy::prop::as_reference_wrapper )
  .property( "Original Jump", &Xivi::PlayerControls::originalJump )( rttr::policy::prop::as_reference_wrapper )
  .property( "Shrink scale", &Xivi::PlayerControls::toShrinkScale )( rttr::policy::prop::as_reference_wrapper )
  .property( "Grow scale", &Xivi::PlayerControls::toGrowScale )( rttr::policy::prop::as_reference_wrapper );
}
}
