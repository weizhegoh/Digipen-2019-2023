/*****************************************************************
*\file         EditorSystems.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
// Base
#include "Editor/Systems/Base/EditorBaseSystems.h"

namespace Xivi
{
using Notifier_Editor_SList =
List::Notifier_EditorBase_SList;

using Global_Editor_SList =
List::Global_EditorBase_SList;

using Update_Editor_SList =
List::Update_EditorBase_SList;
}
#endif