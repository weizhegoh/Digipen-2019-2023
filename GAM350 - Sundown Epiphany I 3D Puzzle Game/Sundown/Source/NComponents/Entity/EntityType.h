/*****************************************************************
*\file         EntityType.h
*\author(s)    Tan tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "Xivi/ECS/XVECS.h"
namespace Xivi
{
enum class GameType
{
  Empty = 0,
  Blocked,
  Hole,
  Floor,
  FallingFloor,
  MovingObstacle,
  MovingPlatform,
  PressurePlate,
  Switch,
  Door,
  UnlockedDoor,
  Box,
  Cheese,
  Goal,
  Player
};

DefineDataComponent( EntityType );
GameType value { GameType::Empty };
EndDefine();
}

namespace Xivi::Components::Reflect::EntityType
{
RTTR_REGISTRATION
{
rttr::registration::enumeration<GameType>( "Game Type" )
(
rttr::value( "Empty", GameType::Empty ),
rttr::value( "Blocked", GameType::Blocked ),
rttr::value( "Hole", GameType::Hole ),
rttr::value( "Floor", GameType::Floor ),
rttr::value( "FallingFloor", GameType::FallingFloor ),
rttr::value( "MovingObstacle", GameType::MovingObstacle ),
rttr::value( "MovingPlatform", GameType::MovingPlatform ),
rttr::value( "PressurePlate", GameType::PressurePlate ),
rttr::value( "Switch", GameType::Switch ),
rttr::value( "Door", GameType::Door ),
rttr::value( "UnlockedDoor", GameType::UnlockedDoor ),
rttr::value( "Box", GameType::Box ),
rttr::value( "Cheese", GameType::Cheese ),
rttr::value( "Goal", GameType::Goal ),
rttr::value( "Player", GameType::Player )
);

rttr::registration::class_<Xivi::EntityType>( "Entity Type" )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::EntityType>.m_id ) )
.property( "Type", &Xivi::EntityType::value );
}
}