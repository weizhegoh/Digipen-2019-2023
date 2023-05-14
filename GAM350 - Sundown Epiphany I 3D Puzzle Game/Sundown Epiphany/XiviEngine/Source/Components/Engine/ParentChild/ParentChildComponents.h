/*****************************************************************
*\file         ParentChildComponents.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

// Data Components
#include "Parent.h"
#include "Child.h"

// Tag Components
#include "Root.h"

namespace Xivi
{
using ParentChild_CList = CList<Parent, Child, Root>;
}