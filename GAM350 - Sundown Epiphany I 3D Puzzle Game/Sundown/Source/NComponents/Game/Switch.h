/*****************************************************************
*\file         Switch.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
DefineDataComponent( Switch );
Entity linked_entity;
bool triggered;
EndDefine();
}

namespace Xivi::Components::Switch::Reflect
{
RTTR_REGISTRATION
{
rttr::registration::class_<Xivi::Switch>( std::string( Xivi::Switch::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Switch>.m_id ) )
.property( "Linked Entity", &Xivi::Switch::linked_entity )( rttr::policy::prop::as_reference_wrapper )
;
}
}
