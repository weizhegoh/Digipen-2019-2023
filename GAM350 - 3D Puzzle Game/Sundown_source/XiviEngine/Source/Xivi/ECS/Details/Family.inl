/*****************************************************************
*\file         Family.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::Pool::Family
{
ID GenerateID( const Archetype::ID id, DescriptorSpan descriptors, ConstDataSpan datas ) noexcept
{
  XV_CORE_ASSERT_MSG( descriptors.size() == datas.size(), "[Family] The size of data and descriptor does not match" );
  ID family_id { id.m_value };
  for ( const auto &[i, desc] : Enumerate( descriptors ) )
    family_id.m_value += Component::GenerateSharedKey( id, *desc, datas[i] ).m_value;
  return family_id;
}

void Instance::Initialise( ID id,
                           Archetype::Instance &archetype,
                           DescriptorSpan data_descriptors,
                           DescriptorSpan shared_descriptors,
                           SharedKeySpan shared_keys,
                           EntitySpan shared_entities ) noexcept
{
  XV_CORE_ASSERT_MSG( id.Valid(), "[Family] ID not valid" );
  XV_CORE_ASSERT_MSG( data_descriptors.data(), "[Family] data descriptor is nullptr" );
  XV_CORE_ASSERT_MSG( shared_entities.size() == shared_descriptors.size(), "[Family] The size of shared entites and shared descriptor does not match" );
  XV_CORE_ASSERT_MSG( shared_entities.size() == shared_keys.size(), "[Family] The size of shared entites and shared keys does not match" );

  m_id = id;
  m_archetype = &archetype;
  m_shared_descriptors = shared_descriptors;
  for ( const auto &[i, key] : Enumerate( shared_keys ) )
  {
    auto &entry = m_shared_details[i];
    entry.m_entity = shared_entities[i];
    entry.m_key = key;
  }

  m_pool.Initialise( data_descriptors, *this );
}

template <Traits::Concepts::InvokableFunction Func>
void Instance::Append( u32 count, Archetype::Manager &archetype_manager, Func &&func ) noexcept
{
  auto index = m_pool.Append( count );
  archetype_manager.AddToStructuralPendingList( m_pool );
  func( m_pool, index, count );
}

void Instance::MoveIn( World::Instance &m_world, Instance &src_family, Pool::Instance &src_pool, Pool::Index src_index ) noexcept
{
  (void) src_family;
  m_world.m_archetype_manager.AddToStructuralPendingList( src_pool );
  Append( 1, m_world.m_archetype_manager, [&]( Pool::Instance &dest_pool, Pool::Index dest_index, u32 )
  {
    XV_CORE_ASSERT_MSG( &src_pool != &dest_pool, "[Family] Moving into the same pool is not allowed" );
    const auto entity = src_pool.GetComponent<Entity>( src_index );
    auto &entity_desc = m_world.m_entity_manager.m_entities[entity.m_global_index];

    dest_pool.MoveInFromPool( src_pool, src_index, dest_index );
    entity_desc.m_pool = &dest_pool;
    entity_desc.m_pool_index = dest_index;
  } );
}
}