/*****************************************************************
*\file         SerialisePrefabSystem.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi
{
void SerialisePrefabSystem::OnEvent( Entity &entity, const std::string &filename )
{
  XV_CORE_INFO( "[SerialisePrefabSystem] Serialise entity - {} to {}...", entity.Index(), filename );
  m_world.m_archetype_manager.UpdateStructuralChanges();
  auto &c_desc_mgr = ECS::Component::DescriptorManager::Instance();

  JsonFile file; // Writer

  XV_CORE_TRACE( "[SerialisePrefabSystem] Mapping entity and its child indexes..." );
  std::unordered_map<u32, u32> remap;
  auto index = remap[entity.Index()] = 0;
  std::function<void( Entity &entity )> map_child = [&]( Entity &entity )
  {
    if ( HaveComponent<Child>( entity ) )
    {
      auto child_entity = GetComponent<Child>( entity );
      for ( auto &child : child_entity.m_entities )
      {
        remap[child.Index()] = ++index;
        map_child( child );
      }
    }
  };
  map_child( entity );

  std::function<void( Entity &entity )> serialise = [&]( Entity &s_entity )
  {
    auto &s_entry = m_world.m_entity_manager.Get( s_entity );
    auto s_signature = s_entry.m_archetype->GetSignature();
    auto &s_pool = *s_entry.m_pool;
    auto &s_pool_index = s_entry.m_pool_index.m_value;

    XV_CORE_TRACE( "[SerialisePrefabSystem] Serialsing entity {}, {}...", s_entity.Index(), s_entry.m_name );

    if ( remap[s_entity.Index()] == 0 )
      s_signature.set( ECS::Component::info<Parent>.m_bit_index, false );

    auto descriptors = c_desc_mgr.DescriptorVector( s_signature );

    file.StartObject();

    XV_CORE_TRACE( "[SerialisePrefabSystem] Serialsing entity - {}, {}...", s_entity.Index(), s_entry.m_name );

    XV_CORE_TRACE( "[SerialisePrefabSystem] Serialsing details..." );
    file.WriteKey( "Name" ).WriteString( s_entry.m_name );

    file.WriteKey( "Mapped ID" ).WriteU32( remap[s_entity.Index()] );

    XV_CORE_TRACE( "[SerialisePrefabSystem] Serialsing component descriptors..." );
    file.WriteKey( "ComponentDescriptors" ).StartArray();
    for ( const auto &desc_ : descriptors )
    {
      file.StartObject();
      file.WriteKey( "ID" ).WriteU64( desc_->m_id.m_value );
      file.WriteKey( "Name" ).WriteString( std::string( desc_->m_name ) );
      file.WriteKey( "Type" ).WriteString( desc_->m_type == ECS::Component::Type::ID::Data ? "Data" :
                                           desc_->m_type == ECS::Component::Type::ID::Shared ? "Shared" :
                                           desc_->m_type == ECS::Component::Type::ID::Tag ? "Tag" : "ExclusiveTag" );
      file.EndObject();
    }
    file.EndArray(); // Descriptors End

    XV_CORE_TRACE( "[SerialisePrefabSystem] Serialsing components..." );
    file.WriteKey( "Components" ).StartArray(); // Entity Details
    for ( const auto &[i, desc_] : Enumerate( s_pool.m_descriptors ) )
    {
      if ( desc_->m_id == ECS::Component::info<Entity>.m_id )
        continue;
      else if ( desc_->m_id == ECS::Component::info<Parent>.m_id )
      {
        if ( remap[s_entity.Index()] == 0 )
          continue;
        auto &parent = *std::bit_cast<Ptr<Parent>>( &s_pool.m_components[i][s_pool_index * desc_->m_size] );
        file.StartObject();
        file.WriteKey( "Parent" ).WriteU32( remap[parent.m_entity.Index()] );
        file.EndObject();
      }
      else if ( desc_->m_id == ECS::Component::info<Child>.m_id )
      {
        auto &childrens = *std::bit_cast<Ptr<Child>>( &s_pool.m_components[i][s_pool_index * desc_->m_size] );
        std::vector<u32> childrens_vec;
        for ( const auto &c : childrens.m_entities )
          childrens_vec.push_back( remap[c.Index()] );

        rttr::variant v = childrens_vec;

        file.StartObject();
        file.WriteKey( "Child" ).WriteArray( v.create_sequential_view() );
        file.EndObject();
      }
      else
      {
        auto obj = desc_->m_reflect( &s_pool.m_components[i][s_pool_index * desc_->m_size] );
        file.StartObject();
        file.WriteKey( obj.get_type().get_name().to_string() ).StartObject().WriteObject( obj ).EndObject();
        file.EndObject();
      }
    }
    file.EndArray().EndObject(); // Entity End

    XV_CORE_TRACE( "[SerialisePrefabSystem] Serialising child entities" );
    if ( HaveComponent<Child>( s_entity ) )
      GetEntity( s_entity, [&]( Child &child )
    {
      for ( auto &c_entity : child.m_entities )
        serialise( c_entity );
    } );
  };

  file.StartWriter( filename ).StartObject();
  file.WriteKey( "Entities" ).StartArray();
  serialise( entity );
  file.EndArray();
  file.EndObject().EndWriter();
  XV_CORE_INFO( "[SerialisePrefabSystem] Entity serialised" );
}
}