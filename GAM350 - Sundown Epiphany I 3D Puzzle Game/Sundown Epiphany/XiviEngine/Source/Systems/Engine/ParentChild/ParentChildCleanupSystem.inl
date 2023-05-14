/*****************************************************************
*\file         ParentChildCleanupSystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void ParentChildCleanupSystem::operator()( Entity &entity, Parent *parent, Child *child )noexcept
{
  if ( parent )
  {
    GetEntity( parent->m_entity, [&]( Child &parent_child )
    {
      for ( auto it = parent_child.m_entities.begin(), end = parent_child.m_entities.end(); it != end; ++it )
        if ( *it == entity )
        {
          XV_CORE_TRACE( "[ParentChildCleanUpSystem] Unlinking Child - {} from Parent - {}", entity.Index(), parent->m_entity.Index() );
          parent_child.m_entities.erase( it ); // Unlink Children from Parent
          if ( parent_child.m_entities.empty() ) // Delete Children Component if empty
            SendGlobalEvent<ParentChildRemove>( parent->m_entity );
          return;
        }
    } );
  }

  if ( child )
    for ( auto c : child->m_entities )
      if ( !IsEntityZombie( c ) )
      {
        GetComponent<Parent>( c ).m_entity = {};
        DestroyEntity( c ); // Delete Children
      }
}
}