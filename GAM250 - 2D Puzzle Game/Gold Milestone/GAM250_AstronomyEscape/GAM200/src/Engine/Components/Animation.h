/*****************************************************************
*\file		 Animation.h
*\brief		 Animation Component 

*\author(s)   Lee Jun Jie	14 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include "Animations.h"
#include <GameTypes.h>

struct Animation
{
  Animations* animationsList {};
  Index currentFrameIndex {0};
  float currentFrameTimer {0};
  ObjectState currentObjectState { ObjectState::IDLE };
  bool endAni{ false };
};