/*****************************************************************
*\file         Grid.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "GameGrid.h"
#include "GridPosition.h"

namespace Xivi
{
using Grid_CList = CList<GameGrid, GridPosition>;
}