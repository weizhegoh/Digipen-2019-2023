/*****************************************************************
*\file         DebugDraw_c.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Xivi/Graphics/Model/Model.h"
#include <Resource/Manager.h>
namespace Xivi
{
struct DebugDrawComponent
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Debug Draw"
  };
  //not sure what var to have, on hold first
  Resource::Handler<Graphics::StaticModel> model;
  bool temp;
};
}

namespace Xivi::Components::DebugDrawComponent::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::DebugDrawComponent>( std::string( Xivi::DebugDrawComponent::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::DebugDrawComponent>.m_id ) )
  .property( "temp", &Xivi::DebugDrawComponent::temp )( rttr::policy::prop::as_reference_wrapper );
}
}