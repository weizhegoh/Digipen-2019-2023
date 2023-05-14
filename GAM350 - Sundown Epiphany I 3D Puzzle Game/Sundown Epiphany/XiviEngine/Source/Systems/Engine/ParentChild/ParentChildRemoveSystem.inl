/*****************************************************************
*\file         ParentChildRemoveSystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void ParentChildRemoveSystem::OnEvent( const Entity &parent )
{
  if ( IsEntityZombie( parent ) )
    return;

  if ( HaveComponent<Child>( parent ) )
  {
    XV_CORE_TRACE( "[ParentChildRemoveSystem] Removing Child and Root Component from Parent - {}", parent.Index() );
    AddOrRemoveComponents<CList<>, CList<Child, Root>>( parent );
  }
}
}