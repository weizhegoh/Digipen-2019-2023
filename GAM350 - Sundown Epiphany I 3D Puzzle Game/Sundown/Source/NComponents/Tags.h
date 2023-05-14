/*****************************************************************
*\file         Tags.h
*\author(s)    Tan Tong Wee
               Kenric Tan Wei Liang
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

DefineTagComponent( Move );
DefineTagComponent( Rotate );
DefineTagComponent( Scale );
DefineTagComponent( Animating );
DefineTagComponent( BezierTag );
DefineTagComponent( Falling );
DefineTagComponent( CheckFall );
DefineTagComponent( Activated );
DefineTagComponent( Triggered );
DefineTagComponent( PressurePlateObj );
DefineTagComponent( DoorObj );
DefineTagComponent( SwitchObj );
DefineTagComponent( GoalObj );
DefineTagComponent( Environment );
DefineTagComponent( PressurePlateCount );
DefineTagComponent( DirectionInitialisationTag );
DefineTagComponent( InteractableSwitch );

DefineTagComponent( LeveledTile );

namespace Xivi
{
using Tag_CList = CList<
  Move,
  Rotate,
  Scale,
  Animating,
  BezierTag,
  CheckFall,
  Falling,
  Activated,
  Triggered,
  PressurePlateObj,
  DoorObj,
  SwitchObj,
  GoalObj,
  Environment,
  PressurePlateCount,
  DirectionInitialisationTag,
  InteractableSwitch,
  LeveledTile>;
}