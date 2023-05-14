/*****************************************************************
*\file         AudioComponents.h
*\author(s)    Kevin Neo Jian Sheng

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

// Include accordingly
#include "Components/Engine/Audio/Audio2DCom.h"
#include "Components/Engine/Audio/Audio3DCom.h"

namespace Xivi
{
using Audio_CList = CList<Audio2DComponent, Audio3DComponent>;
}