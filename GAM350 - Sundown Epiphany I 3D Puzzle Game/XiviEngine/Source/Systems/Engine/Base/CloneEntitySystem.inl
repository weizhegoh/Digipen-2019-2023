/*****************************************************************
*\file         CloneEntitySystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void CloneEntitySystem::OnEvent( Entity &entity )
{
  auto &c_mgr = ECS::Component::DescriptorManager::Instance();
  auto &archetype = GetArchetype( entity );
  auto signature = archetype.GetSignature();

  // Remove Parent component to create archetype without parent
  // Parent linking is done at the child level
  signature.set( ECS::Component::info<Parent>.m_bit_index, false );
  auto descriptor = c_mgr.DescriptorVector( signature );
  auto &target_archetype = GetOrCreateArchetype( descriptor );
  auto clone = target_archetype.CreateEntity();

  // Generate a descriptor without child and entit
  // The child and entity value should not be duplicated
  signature.set( ECS::Component::info<Child>.m_bit_index, false ); // Remove Child
  signature.set( ECS::Component::info<Entity>.m_bit_index, false ); // Remove Entity
  auto setter_descriptor = c_mgr.DescriptorVector( ( signature & ~c_mgr.m_tag_bits ) & ~c_mgr.m_exclusive_tag_bits );

  m_world.m_archetype_manager.UpdateStructuralChanges();

  #ifdef XV_EDITOR
  auto filter_name = m_world.m_entity_manager.Get( entity ).m_name;
  while ( std::isdigit( filter_name.back() ) )
    filter_name.pop_back();
  auto count = 1;
  for ( auto global_entity : m_world.m_entity_manager.m_entities )
  {
    if ( global_entity.Zombie() )
      continue;

    if ( auto pos = global_entity.m_name.find( filter_name ); pos != std::string::npos )
    {
      auto substr = global_entity.m_name.substr( pos + filter_name.size() );
      if ( !substr.empty() && std::ranges::all_of( substr.begin(), substr.end(), []( char c )
      {
        return std::isdigit( c );
      } ) )
        if ( auto i = std::stoi( substr ); i >= count )
          count = i + 1;
    }
  }

  m_world.m_entity_manager.Get( clone ).m_name = filter_name + std::to_string( count );
  #else
  m_world.m_entity_manager.Get( clone ).m_name = {};
  #endif

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
        SendGlobalEvent<CloneEntity>( c );
        SendGlobalEvent<ParentChildLink>( c, clone );
      }
    } );
  }

  // Link to prefab, if copying a prefab instance
  if ( HaveComponent<ReferencedPrefab>( clone ) )
    GetEntity( clone, [&]( ReferencedPrefab &ref_prefab )
  {
    GetEntity( ref_prefab.m_entity, [&]( Prefab &prefab )
    {
      prefab.referenced_entities.push_back( clone );
    } );
  } );

  entity = clone;
}
}