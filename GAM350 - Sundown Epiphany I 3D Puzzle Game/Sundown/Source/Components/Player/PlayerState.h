/*****************************************************************
*\file         PlayerState.h
*\author(s)    Tan Tong Wee
         Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi
{
struct PlayerState
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Player State"
  };
  enum class State
  {
    Idle = 0,
    Forward = 1,
    Backward = 2,
    Leftward = 3,
    Rightward = 4,
    JUMP = 5,
  };
  State state;
};
}

namespace Xivi::Components::PlayerState::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::PlayerState>( std::string( Xivi::PlayerState::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::PlayerState>.m_id ) )
  .property( "State", &Xivi::PlayerState::state )( rttr::policy::prop::as_reference_wrapper );
}
}