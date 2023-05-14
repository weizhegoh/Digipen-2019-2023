/*****************************************************************
*\file         BaseSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

// Global
#include "CloneEntitySystem.h"

#ifdef XV_ENGINE_IMPL
#include "CloneEntitySystem.inl"
#endif

namespace Xivi::List
{
using Global_Base_SList = SList<
  CloneEntitySystem>;
using Notifier_Base_SList = SList<>;
using Update_Base_SList = SList<>;
}