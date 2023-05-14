/*****************************************************************
*\file         EntityDirection.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
enum class RotationDirection
{
  Up,
  Down,
  Left,
  Right
};

DefineDataComponent( EntityDirection );
RotationDirection value;
EndDefine();
}

namespace Xivi::Components::Reflect::EntityDirection
{
RTTR_REGISTRATION
{
  rttr::registration::enumeration<RotationDirection>( "Game Direction" )(
      rttr::value( "Up", RotationDirection::Up ),
      rttr::value( "Down", RotationDirection::Down ),
      rttr::value( "Left", RotationDirection::Left ),
      rttr::value( "Right", RotationDirection::Right )
  );

  rttr::registration::class_<Xivi::EntityDirection>( "Entity Direction" )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::EntityDirection>.m_id ) )
  .property( "Direction", &Xivi::EntityDirection::value );
}
}