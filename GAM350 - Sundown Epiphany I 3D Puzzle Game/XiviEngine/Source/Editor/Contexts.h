/*****************************************************************
*\file         Contexts.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "Contexts/EntityContext.h"
#include "Contexts/FileContext.h"
#include "Contexts/ToolContext.h"

namespace Xivi
{
struct Position;

using ArchetypeCreateFunc = Func<Entity( const Position & )>;
using AssetCreateFunc = Func<Entity( const Position &, const std::string & )>;
}
#endif