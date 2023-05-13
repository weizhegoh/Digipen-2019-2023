/*****************************************************************
*\file         DeserialisePrefabSystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void DeserialisePrefabSystem::OnEvent( Entity &entity, const std::string &filename )
{
  XV_CORE_INFO( "[World] Deserialise entity from {}...", filename );

  std::unordered_map<u32, Entity> entity_map;
  auto counter = 0u;

  JsonFile file; // Writer

  XV_CORE_TRACE( "[World] Deserialising entities..." );
  file.StartReader( filename );
  auto m_itr = file.FindMember( "Entities" );
  for ( auto entity_itr = m_itr->value.Begin(); entity_itr != m_itr->value.End(); ++entity_itr )
  {
    XV_CORE_TRACE( "[World] Deserialising entity's component descriptors..." );
    std::vector<Ptr<const ECS::Component::Descriptor>> descriptor_vec;
    auto descriptors_itr = entity_itr->FindMember( "ComponentDescriptors" );
    for ( auto desc_it = descriptors_itr->value.Begin(); desc_it != descriptors_itr->value.End(); ++desc_it )
    {
      auto id = desc_it->FindMember( "ID" )->value.GetUint64();
      auto desc = ECS::Component::DescriptorManager::Instance().Get( { id } );
      if ( !desc )
        XV_CORE_ASSERT_MSG( false, "[World] Incorrect serialisation error" );
      descriptor_vec.push_back( desc );
    }

    XV_CORE_TRACE( "[World] Creating archetype..." );
    auto &archetype = GetOrCreateArchetype( { descriptor_vec } );

    XV_CORE_TRACE( "[World] Creating entity - {}...", counter );
    Entity curr_entity = archetype.CreateEntity();
    entity_map[counter++] = curr_entity;
    auto &entry = m_world.m_entity_manager.Get( curr_entity );
    entry.m_name = entity_itr->FindMember( "Name" )->value.GetString();
    auto &pool = *entry.m_pool;

    XV_CORE_TRACE( "[World] Deserialising entity's components..." );
    auto components_itr = entity_itr->FindMember( "Components" );
    auto curr_com = 1;
    constexpr auto parent_id = Hash::XXH3_64( "Parent" );
    constexpr auto child_id = Hash::XXH3_64( "Child" );
    i64 parent_mapped_id = -1;

    for ( auto com_itr = components_itr->value.Begin(); com_itr != components_itr->value.End(); ++com_itr, ++curr_com )
      for ( auto value_itr = com_itr->MemberBegin(); value_itr != com_itr->MemberEnd(); ++value_itr )
      {
        if ( Hash::XXH3_64( value_itr->name.GetString() ) == parent_id )
          parent_mapped_id = value_itr->value.GetUint();
        else if ( Hash::XXH3_64( value_itr->name.GetString() ) == child_id )
          continue;
        else
        {
          auto desc = pool.m_descriptors[curr_com];
          auto instance = desc->m_reflect( &pool.m_components[curr_com][desc->m_size * entry.m_pool_index.m_value] );
          Deserialise::ReadRecursive( instance, value_itr->value );
        }
      }

    if ( parent_mapped_id != -1 )
    {
      XV_CORE_TRACE( "[World] Linking parent-child..." );
      SendGlobalEvent<ParentChildLink>( curr_entity, entity_map[static_cast<u32>( parent_mapped_id )] );
    }
  }
  file.EndReader();
  XV_CORE_INFO( "[World] Deserialised entity" );
  entity = entity_map[0];
}
}