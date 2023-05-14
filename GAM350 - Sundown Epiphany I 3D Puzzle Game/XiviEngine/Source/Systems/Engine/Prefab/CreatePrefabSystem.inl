/*****************************************************************
*\file         CreatePrefabInstance.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void CreatePrefabSystem::OnCreate() noexcept
{
  // Initial
  m_initial.set( ECS::Component::info<PrefabTag>.m_bit_index );
  m_initial.set( ECS::Component::info<Prefab>.m_bit_index );

  // Remove Entity, Child, Prefab when memcpying
  m_update.set( ECS::Component::info<Entity>.m_bit_index ); // Remove Entity
  m_update.set( ECS::Component::info<Child>.m_bit_index ); // Remove Child
  m_update.set( ECS::Component::info<Prefab>.m_bit_index ); // Remove Prefab
  m_update = ~m_update;

  // Recursively create prefab
  m_recurse_create = [&]( Entity &entity )
  {
    auto &c_mgr = ECS::Component::DescriptorManager::Instance();
    auto &archetype = GetArchetype( entity );
    auto signature = archetype.GetSignature() | m_initial;
    signature.set( ECS::Component::info<Parent>.m_bit_index, false ); // Remove Parent
    auto descriptor = c_mgr.DescriptorVector( signature );
    auto &target_archetype = GetOrCreateArchetype( descriptor );
    auto clone = target_archetype.CreateEntity();
    signature &= m_update;
    auto setter_descriptor = c_mgr.DescriptorVector( signature & ~c_mgr.m_tag_bits );

    m_world.m_archetype_manager.UpdateStructuralChanges();

    m_world.m_entity_manager.Get( clone ).m_name = m_world.m_entity_manager.Get( entity ).m_name + "_prefab";

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
void CreatePrefabSystem::OnEvent( Entity &entity, const std::string &filename )
{
  m_recurse_create( entity );
  SendGlobalEvent<SerialisePrefab>( entity, filename );
}
}