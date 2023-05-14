/*****************************************************************
*\file         LocalToParentUpdateSystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void LocalToParentUpdateSystem::OnCreate() noexcept
{
  m_updates = [&]( Position &pos, Child &child )
  {
    for ( auto c : child.m_entities )
    {
      GetEntity( c, [&]( Position &child_pos, const LocalToParent &local )
      {
        child_pos.m_position = pos.m_position + local.m_position;
        if ( HaveComponent<Child>( c ) )
          m_updates( child_pos, GetComponent<Child>( c ) );
      } );
    }
  };
}
void LocalToParentUpdateSystem::operator()( Position &pos, Child &child ) noexcept
{
  m_updates( pos, child );
}
}