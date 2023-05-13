/*****************************************************************
*\file         ProcessPlayerInputSystem.h
*\author(s)    Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "NSystems/GameWorld/GameWorldSystem.h"

namespace Xivi
{
DefineSystemEventSystem( ProcessPlayerInputSystem, GameWorldSystem, PlayerInputEvent );

using Events = tuplet::tuple<ProcessPlayerEvent>;

void OnEvent() noexcept;

EndDefine();
}