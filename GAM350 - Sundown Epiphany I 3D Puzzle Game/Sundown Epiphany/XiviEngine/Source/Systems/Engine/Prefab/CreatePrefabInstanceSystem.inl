/*****************************************************************
*\file         CreatePrefabInstanceSystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void CreatePrefabInstanceSystem::OnCreate() noexcept

{
  m_initial.set( ECS::Component::info<ReferencedPrefab>.m_bit_index ); // Xor on
  m_initial.set( ECS::Component::info<PrefabTag>.m_bit_index ); // Xor off
  m_initial.set( ECS::Component::info<Prefab>.m_bit_index ); // Xor off

  m_update.set( ECS::Component::info<Entity>.m_bit_index ); // Remove Entity
  m_update.set( ECS::Component::info<Child>.m_bit_index ); // Remove Child
  m_update.set( ECS::Component::info<ReferencedPrefab>.m_bit_index ); // Remove ReferencedPrefab
  m_update = ~m_update;

  m_recurse_create = [&]( Entity &entity )
  {
    auto &c_mgr = ECS::Component::DescriptorManager::Instance();
    auto &archetype = GetArchetype( entity );
    auto signature = archetype.GetSignature() ^ m_initial;
    signature.set( ECS::Component::info<Parent>.m_bit_index, false ); // Remove Parent

    auto descriptor = c_mgr.DescriptorVector( signature );
    auto &target_archetype = GetOrCreateArchetype( descriptor );

    // Create instance and link prefab
    auto clone = target_archetype.CreateEntity( [&]( ReferencedPrefab &ref_prefab )
    {
      ref_prefab.m_entity = entity;
    } );

    GetEntity( entity, [&]( Prefab &prefab ) // Link instance to prefab
    {
      prefab.referenced_entities.push_back( clone );
    } );

    signature &= m_update;
    auto setter_descriptor = c_mgr.DescriptorVector( signature & ~c_mgr.m_tag_bits );

    m_world.m_archetype_manager.UpdateStructuralChanges();

    m_world.m_entity_manager.Get( clone ).m_name = m_world.m_entity_manager.Get( entity ).m_name + "_instance";

    auto src = GetComponents( entity, setter_descriptor );
    auto dest = GetComponents( clone, setter_descriptor );

    for ( const auto &[i, desc] : Enumerate( setter_descriptor ) )
      if ( desc->m_copy )
        desc->m_copy( dest[i], src[i] );
      else
        std::memcpy( dest[i], src[i], desc->m_size );

    // Create the childrens
    if ( HaveComponent<Child>( entity ) )
    {
      GetEntity( entity, [&]( Child &child )
      {
        for ( Entity c : child.m_entities )
        {
          m_recurse_create( c );
          SendGlobalEvent<ParentChildLink>( c, clone );
        }
      } );
    }
    entity = clone;
  };
}

void CreatePrefabInstanceSystem::OnEvent( Entity &entity ) noexcept
{
  m_recurse_create( entity );
}
}