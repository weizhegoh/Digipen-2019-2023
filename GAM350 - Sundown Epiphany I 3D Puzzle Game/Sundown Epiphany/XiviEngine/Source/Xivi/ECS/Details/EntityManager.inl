/*****************************************************************
*\file         EntityManager.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS
{
EntityManager::EntityManager() noexcept
{
  Initialise();
}

void EntityManager::Initialise()
{
  for ( int i = 0, end = Settings::max_entities - 2; i < end; ++i )
  {
    m_entities[i].m_pool_index = Pool::Index { i + 1 };
    m_entities[i].m_validation.m_value = 2147483649u;
    m_entities[i].m_archetype = nullptr;
    m_entities[i].m_pool = nullptr;
  }
  m_entities[Settings::max_entities - 1].m_pool_index.m_value = -1;

  m_count = m_max = m_head = 0;
}

const Entity::Descriptor &EntityManager::Get( Entity entity ) const noexcept
{
  XV_CORE_ASSERT_MSG( entity.Valid(), "[EntityManager] Invalid entity" );
  auto &entry = m_entities[entity.m_global_index];
  XV_CORE_ASSERT_MSG( entry.m_validation == entity.m_validation, "[EntityManager] Entity validation not match" );
  return entry;
}

Entity::Descriptor &EntityManager::Get( Entity entity ) noexcept
{
  return const_cast<Entity::Descriptor &>( static_cast<const EntityManager &>( *this ).Get( entity ) );
}

Entity EntityManager::Allocate( Archetype::Instance &archetype, Pool::Instance &pool, Pool::Index pool_index ) noexcept
{
  XV_CORE_ASSERT_MSG( m_head >= 0, "[EntityManager] Invalid index" );
  u32 i = static_cast<u32>( m_head );
  auto &entry = m_entities[i];
  m_head = entry.m_pool_index.m_value;

  entry.m_name = "Entity_" + std::to_string( i );
  entry.m_archetype = &archetype;
  entry.m_pool = &pool;
  entry.m_pool_index = pool_index;
  entry.m_validation.m_zombie = false;

  if ( i > m_max )
    m_max = i;

  ++m_count;
  return Entity( static_cast<u32>( i ), entry.m_validation );
}

void EntityManager::Delete( u32 index ) noexcept
{
  auto &entry = m_entities[index];
  ++entry.m_validation.m_generation;
  entry.m_validation.m_zombie = true;
  entry.m_pool_index.m_value = m_head;
  XV_CORE_INFO( "[EntityManager] Stored - {}", m_head );
  m_head = static_cast<i32>( index );
  XV_CORE_INFO( "[EntityManager] New - {}", m_head );
  --m_count;
}

void EntityManager::Delete( u32 index, const Entity &entity ) noexcept
{
  auto &entry = m_entities[index];
  m_entities[entity.m_global_index].m_pool_index = entry.m_pool_index;
  ++entry.m_validation.m_generation;
  entry.m_validation.m_zombie = true;
  entry.m_pool_index.m_value = m_head;
  XV_CORE_INFO( "[EntityManager] Stored - {}", m_head );
  m_head = static_cast<i32>( index );
  XV_CORE_INFO( "[EntityManager] New - {}", m_head );
  --m_count;
}

void EntityManager::Move( Pool::Index index, const Entity &entity ) noexcept
{
  m_entities[entity.m_global_index].m_pool_index = index;
}
}