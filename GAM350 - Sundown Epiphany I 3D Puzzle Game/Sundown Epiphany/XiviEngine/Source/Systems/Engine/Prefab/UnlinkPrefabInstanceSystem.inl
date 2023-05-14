/*****************************************************************
*\file         UnlinkPrefabInstanceSystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void UnlinkPrefabInstanceSystem::OnEvent( Entity &entity, ReferencedPrefab &ref_prefab )
{
  GetEntity( ref_prefab.m_entity, [&]( Prefab &prefab )
  {
    std::erase( prefab.referenced_entities, entity );
  } );
}
}