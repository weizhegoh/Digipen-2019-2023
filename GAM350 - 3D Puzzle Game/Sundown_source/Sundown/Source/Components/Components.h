/*****************************************************************
*\file         PlayerState.h
*\author(s)    Tan Tong Wee
         Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Player/PlayerControls.h"
#include "Player/PlayerState.h"
#include "Player/PushableObj.h"
#include "Components/ParticleComponents.h"

namespace Xivi
{
using Sundown_CList = CList<PlayerControls,
  PlayerState, PushableObject>;
}