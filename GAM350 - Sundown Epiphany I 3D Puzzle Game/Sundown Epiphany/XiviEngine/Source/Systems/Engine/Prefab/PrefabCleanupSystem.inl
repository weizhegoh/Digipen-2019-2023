/*****************************************************************
*\file         PrefabCleanupSystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void PrefabCleanupSystem::operator()( Entity &entity, Prefab *prefab, ReferencedPrefab *ref_prefab )
{
  if ( prefab )
    SendEvent<UnlinkPrefab>( this, *prefab );
  else if ( ref_prefab )
    SendEvent<UnlinkPrefabInstance>( this, entity, *ref_prefab );
}
}