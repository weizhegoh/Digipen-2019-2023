/*****************************************************************
*\file         ArchetypeManager.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

namespace Xivi::ECS::Archetype
{
/*
* Constructor / Destructor
*/

Manager::Manager( World::Instance &world ) noexcept
  : m_world( world )
{}

/*
* Archetypes
*/

Archetype::Instance &Manager::Get( ID id ) noexcept
{
  auto it = m_archetype_map.find( id );
  XV_CORE_ASSERT_MSG( it != m_archetype_map.end(), "[ArchetypeManager] Could not find the achetype" );
  return *it->second;
}

Archetype::Instance &Manager::Get( const std::string &str ) noexcept
{
  for ( auto arch : m_archetype_map )
    if ( arch.second->m_name == str )
      return *arch.second;
  XV_CORE_ASSERT_MSG( false, "[ArchetypeManager] Could not find the achetype" );
}

Archetype::Instance &Manager::Create( ID id, const Signature &signature, const std::string &str ) noexcept
{
  m_archetypes.push_back( MakeUnique<Archetype::Instance>( *this ) );
  auto &archetype = *m_archetypes.back().get();
  archetype.Initialise( id, signature, str );
  m_archetype_map.emplace( id, &archetype );
  m_archetypes_bits.push_back( { signature, archetype.m_exclusive_signature } );
  m_events.m_on_new_archetype.Notify( archetype );
  return archetype;
}

Archetype::Instance &Manager::GetOrCreate( const Signature &signature, const std::string &str ) noexcept
{
  XV_CORE_ASSERT_MSG( signature[Component::info<Entity>.m_bit_index], "[ArchetypeManager] Bits not set" );
  ID archetype_id { GenerateID( signature ) };
  if ( auto it = m_archetype_map.find( archetype_id ); it != m_archetype_map.end() )
    return *it->second;
  return Create( archetype_id, signature, str );
}

/*
* Structural Changes
*/

void Manager::AddToStructuralPendingList( Archetype::Instance &archetype ) noexcept
{
  if ( !archetype.m_pending_structural_changes )
  {
    archetype.m_pending_structural_changes = m_archetype_structural_pending;
    m_archetype_structural_pending = &archetype;
  }
}

void Manager::AddToStructuralPendingList( Pool::Instance &pool ) noexcept
{
  if ( !pool.m_pending_changes )
  {
    pool.m_pending_changes = m_pool_structural_pending;
    m_pool_structural_pending = &pool;
  }
}

void Manager::UpdateStructuralChanges() noexcept
{
  for ( auto ptr = m_pool_structural_pending; ptr != std::bit_cast<Ptr<Pool::Instance>>( end_structural_changes );)
  {
    auto next = ptr->m_pending_changes;
    ptr->UpdateStructuralChanges( m_world.m_entity_manager );
    ptr->m_pending_changes = nullptr;
    ptr = next;
  }
  m_pool_structural_pending = std::bit_cast<Ptr<Pool::Instance>>( end_structural_changes );

  for ( auto ptr = m_archetype_structural_pending; ptr != std::bit_cast<Ptr<Instance>>( end_structural_changes );)
  {
    auto next = ptr->m_pending_structural_changes;
    ptr->UpdateStructuralChanges();
    ptr->m_pending_structural_changes = nullptr;
    ptr = next;
  }
  m_archetype_structural_pending = std::bit_cast<Ptr<Instance>>( end_structural_changes );
}

/*
* Entities API
*/

template <Xivi::ECS::Concepts::SetterCallback Func>
Entity Manager::AddOrRemoveComponents( Entity entity, const Signature &add, const Signature &remove, Func &&func ) noexcept
{
  XV_CORE_ASSERT_MSG( !remove[Component::info<Entity>.m_bit_index], "[ArchetypeManager] Bit is not 0" );

  XV_CORE_ASSERT_MSG( !entity.Zombie(), "[ArchetypeManager] Entity is zombie" );
  auto &entry = m_world.m_entity_manager.Get( entity );
  XV_CORE_ASSERT_MSG( !entry.m_validation.m_zombie, "[ArchetypeManager] Entity is zombie" );

  auto &from_archetype = *entry.m_archetype;
  auto signature = entry.m_archetype->m_signature;
  signature = ( signature & ~remove ) | add;

  if ( from_archetype.m_events.m_on_entity_moved_out.m_delegates.size() )
    entry.m_archetype->m_events.m_on_entity_moved_out.Notify( entity );

  auto &archetype = GetOrCreate( signature );

  auto &family = archetype.GetOrCreateFamilyFromDifferentArchetype( entity );

  if constexpr ( std::is_same_v<Func, EmptyLambda> )
    return archetype.MoveEntity( entity, family );
  else
    return archetype.MoveEntity( entity, family, std::forward<Func &&>( func ) );
}
}