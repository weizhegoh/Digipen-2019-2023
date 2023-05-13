/*****************************************************************
*\file         UpdateSummarySystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Components/GUI/Tags.h"
#include "NEvents/StepEvents.h"

namespace Xivi
{
DefineGlobalEventSystem( UpdateSummarySystem, UpdateGUISummaryEvent );

void OnEvent() noexcept;

EndDefine();
}