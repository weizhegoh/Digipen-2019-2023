/*****************************************************************
*\file         GameWorldSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "Xivi/ECS/XVECS.h"
#include "NEvents/StepEvents.h"

namespace Xivi
{
DefineUpdateSystem( GameWorldSystem );

using Events = tuplet::tuple<
  PlayerInputEvent>;
void OnFrameStart() noexcept;
void OnUpdate() noexcept;
void OnPause() noexcept;
EndDefine();
}