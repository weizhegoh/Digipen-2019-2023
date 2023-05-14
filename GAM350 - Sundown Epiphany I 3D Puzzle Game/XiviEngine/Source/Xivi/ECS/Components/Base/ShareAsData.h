/*****************************************************************
*\file         ShareAsData.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::ECS
{
struct ShareAsData
{
  static constexpr auto info = Component::Type::Exclusive { .m_name = "Share As Data" };
};
}

namespace Xivi
{
using ShareAsData = ECS::ShareAsData;
}

namespace Xivi::ECS::Components::ShareAsData::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::ShareAsData>( "Shared Filter" )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::ShareAsData>.m_id ) );
}
}