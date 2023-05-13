/*****************************************************************
*\file         ParentChildLinkSystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void ParentChildLinkSystem::OnEvent( Entity &child, Entity &parent )
{
  XV_CORE_INFO( "[ParentChildLinkSystem] Parenting..." );

  // Parenting itself
  if ( child == parent )
  {
    XV_CORE_WARN( "[ParentChildLinkSystem] Cannot parent itself" );
    return;
  }

  // Check not setting child's child as parent
  std::function<bool( Entity & )> recurse = [&]( Entity &entity ) -> bool
  {
    if ( HaveComponent<Child>( entity ) )
    {
      auto &childs = GetComponent<Child>( entity );

      for ( auto &child : childs.m_entities )
        if ( child == parent || recurse( child ) )
          return true;
    }
    return false;
  };

  if ( recurse( child ) )
  {
    XV_CORE_WARN( "[ParentChildLinkSystem] Cannot set child as parent" );
    return;
  }

  bool end_return { false };

  // Link Parent to Child
  if ( HaveComponent<Parent>( child ) )
  {
    GetEntity( child, [&]( Parent &child_parent )
    {
// Check not setting parent as parent
      if ( child_parent.m_entity == parent )
      {
        XV_CORE_WARN( "[ParentChildLinkSystem] Parenting existing child" );
        end_return = true;
        return;
      }

      GetEntity( child_parent.m_entity, [&]( Child &parent_child )
      {
        for ( auto it = parent_child.m_entities.begin(), end = parent_child.m_entities.end(); it != end; ++it )
          if ( *it == child )
          {
            XV_CORE_TRACE( "[ParentChildLinkSystem] Unlinking Child - {} from Parent - {}", child.Index(), child_parent.m_entity.Index() );
            parent_child.m_entities.erase( it );
            if ( parent_child.m_entities.empty() )
              SendGlobalEvent<ParentChildRemove>( child_parent.m_entity );
            return;
          }
      } );
      child_parent.m_entity = parent;
      XV_CORE_TRACE( "[ParentChildLinkSystem] Linking Parent - {} to Child - {}", child_parent.m_entity.Index(), child.Index() );
    } );
  }
  else /*if( !HaveComponent<UI>( child ) )*/
    child = AddOrRemoveComponents<CList<Parent, LocalToParent>, CList<Root>>( child, [&]( Parent &child_parent )
  {
    child_parent.m_entity = parent;
    XV_CORE_TRACE( "[ParentChildLinkSystem] Linking Parent - {} to Child - {}", child_parent.m_entity.Index(), child.Index() );
  } );

  if ( end_return )
    return;

  // Link Child to Parent
  if ( HaveComponent<Child>( parent ) )
    GetEntity( parent, [&]( Child &parent_child )
  {
    parent_child.m_entities.push_back( child );
    XV_CORE_TRACE( "[ParentChildLinkSystem] Linking Child - {} to Parent - {}", child.Index(), parent.Index() );
  } );
  else
  {
    if ( !HaveComponent<Parent>( parent ) )
      parent = AddOrRemoveComponents<CList<Child, Root>>( parent, [&]( Child &parent_child )
    {
      parent_child.m_entities.push_back( child );
      XV_CORE_TRACE( "[ParentChildLinkSystem] Linking Child - {} to Parent - {}", child.Index(), parent.Index() );
    } );
    else
      parent = AddOrRemoveComponents<CList<Child>>( parent, [&]( Child &parent_child )
    {
      parent_child.m_entities.push_back( child );
      XV_CORE_TRACE( "[ParentChildLinkSystem] Linking Child - {} to Parent - {}", child.Index(), parent.Index() );
    } );
  }

  SendGlobalEvent<ParentChildLinkCallbacks>( child, parent );
}
}