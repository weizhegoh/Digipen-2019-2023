/*****************************************************************
*\file         GridPosition.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "Xivi/ECS/XVECS.h"
#include "Xivi/Core/Utils/Math.h"

namespace Xivi
{
DefineDataComponent( GridPosition );
Vec3i value { -1, -1, -1 };
Vec3f offset { 0.f, 0.f, 0.f };
EndDefine();
}

namespace Xivi::Components::Reflect::GridPosition
{
RTTR_REGISTRATION
{
rttr::registration::class_<Xivi::GridPosition>( "Grid Position" )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GridPosition>.m_id ) )
  .property( "Value", &Xivi::GridPosition::value )( rttr::policy::prop::as_reference_wrapper )
  .property( "Offset", &Xivi::GridPosition::offset )( rttr::policy::prop::as_reference_wrapper );
}
}