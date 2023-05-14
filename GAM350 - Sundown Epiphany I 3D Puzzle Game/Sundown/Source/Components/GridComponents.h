/*****************************************************************
*\file         GridComponents.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi
{
struct Cell
{
  Entity entity;
  CellType type;
};

DefineDataComponent( Grid );
Vec3i grid_size { 5,5,5 };
Vec3f cell_size { 50.0f, 50.0f, 50.0f };
vector<vector<vector<Cell>>> cells;
EndDefine();

DefineDataComponent( GridPosition );
Vec3i value;
EndDefine();
}

namespace Xivi::Components::Reflect::GridPosition
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::Vec3i>( "Cell Position" )
  .property( "x", &Xivi::Vec3i::x )
  .property( "z", &Xivi::Vec3i::z )
  .property( "y", &Xivi::Vec3i::y );

  rttr::registration::class_<Xivi::GridPosition>( std::string( Xivi::GridPosition::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GridPosition>.m_id ) )
  .property( "Value", &Xivi::GridPosition::value )( rttr::policy::prop::as_reference_wrapper );
}
}