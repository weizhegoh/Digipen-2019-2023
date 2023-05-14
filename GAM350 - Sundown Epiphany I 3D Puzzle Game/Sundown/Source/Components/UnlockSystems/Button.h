/*****************************************************************
*\file         Button.h
*\author(s)    Tan Tong Wee
               Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi
{
DefineDataComponent( ButtonTimer );
f32 current_timer; // Internal processing
f32 reset_timer;   // To be reflected
EndDefine();

DefineDataComponent( ButtonControl );
i32 trigger_count; // Internal processing
bool will_reset;
EndDefine();

DefineTagComponent( ButtonTriggered );
EndDefine();

DefineTagComponent( ButtonActivated );
EndDefine();

DefineTagComponent( ButtonCompleted );
EndDefine();
}