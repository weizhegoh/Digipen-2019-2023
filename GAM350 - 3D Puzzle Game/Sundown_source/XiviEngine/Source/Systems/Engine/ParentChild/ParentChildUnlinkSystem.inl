/*****************************************************************
*\file         ParentChildUnlinkSystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void ParentChildUnlinkSystem::OnEvent( Entity &child )
{
  if ( HaveComponent<Parent>( child ) )
  {
    GetEntity( child, [&]( Parent &child_parent )
    {
      SendGlobalEvent<ParentChildUnlinkCallbacks>( child, child_parent.m_entity );
      GetEntity( child_parent.m_entity, [&]( Child &parent_child )
      {
        for ( auto it = parent_child.m_entities.begin(), end = parent_child.m_entities.end(); it != end; ++it )
          if ( *it == child )
          {
            XV_CORE_TRACE( "[ParentChildUnlinkSystem] Unlinking Child - {} from Parent - {}", child.Index(), child_parent.m_entity.Index() );
            parent_child.m_entities.erase( it );
            break;
          }
        if ( parent_child.m_entities.empty() )
          SendGlobalEvent<ParentChildRemove>( child_parent.m_entity );
      } );
    } );
    child = AddOrRemoveComponents<CList<>, CList<Parent, LocalToParent>>( child );
  }
}
}