/*****************************************************************
*\file         ParentChildTransformSystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void ParentChildTransformSystem::OnEvent( const Entity &child, const Entity &parent )
{
  GetEntity( child, [&]( Parent &child_parent, Position &child_pos, LocalToParent &local, Transform &transform )
  {
    GetEntity( child_parent.m_entity, [&]( Position &parent_pos, Transform &parent_transform )
    {
      local.m_position = child_pos.m_position - parent_pos.m_position;
      local.scale = transform.scale.getZ() == 0.0f ? 0.0f : parent_transform.scale / transform.scale;
    } );
  } );
}
}