/*****************************************************************
*\file         Player.h
*\author(s)    Tan tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
DefineDataComponent( Player );
Vec3f prevOffsetPos;
float scale { 2.f };

i32 moves_count { 0 };
i32 moves_total;
i32 collectibles_count { 0 };
i32 collectibles_total;
EndDefine();
}

namespace Xivi::Components::Reflect::Player
{
RTTR_REGISTRATION
{
rttr::registration::class_<Xivi::Player>( "Player" )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Player>.m_id ) )
.property( "Previous Offset Position", &Xivi::Player::prevOffsetPos )( rttr::policy::prop::as_reference_wrapper )
.property( "Scale", &Xivi::Player::scale )( rttr::policy::prop::as_reference_wrapper )
.property( "Moves Total", &Xivi::Player::moves_total )( rttr::policy::prop::as_reference_wrapper )
.property( "Collectibles Total", &Xivi::Player::collectibles_total )( rttr::policy::prop::as_reference_wrapper );
}
}