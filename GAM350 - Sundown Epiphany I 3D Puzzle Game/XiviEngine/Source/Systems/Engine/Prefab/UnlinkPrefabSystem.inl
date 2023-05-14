/*****************************************************************
*\file         UnlinkPrefabSystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void UnlinkPrefabSystem::OnEvent( Prefab &prefab )
{
  std::for_each( prefab.referenced_entities.begin(),
                 prefab.referenced_entities.end(),
                 [&]( Entity &e )
  {
    AddOrRemoveComponents<CList<>, CList<ReferencedPrefab>>( e );
  } );
}
}