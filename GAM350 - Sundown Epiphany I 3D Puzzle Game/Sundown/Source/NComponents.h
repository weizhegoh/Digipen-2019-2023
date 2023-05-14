/*****************************************************************
*\file         NComponents.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "NComponents/Entity/Entity.h"
#include "NComponents/Grid/Grid.h"
#include "NComponents/Game/Game.h"
#include "NComponents/Tags.h"
namespace Xivi
{
using Sundown_NCList = Entity_CList::Extend<Grid_CList>::Extend<Game_CList>::Extend<Tag_CList>;
}