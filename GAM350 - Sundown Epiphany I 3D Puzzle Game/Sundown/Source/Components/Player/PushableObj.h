/*****************************************************************
*\file         PushableObj.h
*\author(s)    Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct PushableObject
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Pushable Object"
  };

  bool isLinked = false;
  float Grabdistance = 1.0f;
  int Playernum = 0; //Which player is grabing
  Vec3f distance;
};
}

namespace Xivi::Components::PushableObject::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::PushableObject>( std::string( Xivi::PushableObject::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::PushableObject>.m_id ) )
  .property( "Is it Linked To Player", &Xivi::PushableObject::isLinked )( rttr::policy::prop::as_reference_wrapper )
  .property( "Distance Grabbed", &Xivi::PushableObject::Grabdistance )( rttr::policy::prop::as_reference_wrapper );
}
}
