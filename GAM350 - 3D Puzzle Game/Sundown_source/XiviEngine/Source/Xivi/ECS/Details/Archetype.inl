/*****************************************************************
*\file         Archetype.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::Archetype::Details
{
template <typename Func>
constexpr auto ContainReferenceParameters = []<typename... Cs>( tuplet::tuple<Cs...>* ) constexpr
{
  return ( std::is_reference_v<Cs> && ... );
}( Traits::Tuple::null<Traits::Function::Type<Func>::ArgsTuple> );

template<ECS::Concepts::ComponentReference... Cs>
inline auto GetComponentPointerArray( const Pool::Instance &pool, const Pool::Index starting_index, tuplet::tuple<Cs...> * ) noexcept
{
  XV_CORE_ASSERT_MSG( ( ( pool.GetComponentIndex( Component::info<Cs> ) >= 0 )&&... ), "[Archetype] Invalid component index" );
  using DataTuple = tuplet::tuple<Cs...>;
  using SortedTuple = SortComponentsTuple<DataTuple>;

  array<Ptr<byte>, sizeof...( Cs )> cached_pointers;
  [&] <typename... SortedCs> ( tuplet::tuple<SortedCs...>* ) constexpr noexcept
  {
    i32 sequence = 0;
    ( ( cached_pointers[Traits::Tuple::Index<SortedCs, DataTuple>] = &pool.m_components[pool.GetComponentIndex( Component::info<SortedCs>, sequence )][sizeof( Raw<SortedCs> ) * starting_index.m_value] ), ... );
  }( Traits::Tuple::null<SortedTuple> );
  return cached_pointers;
}

template<typename... Cs>
requires( ECS::Concepts::ComponentPointer<Cs> || ... )
inline auto GetComponentPointerArray( const Pool::Instance &pool, const Pool::Index starting_index, tuplet::tuple<Cs...> * ) noexcept
{
  using DataTuple = tuplet::tuple<Cs...>;
  using SortedTuple = SortComponentsTuple<DataTuple>;

  array<Ptr<byte>, sizeof...( Cs )> cached_pointers;
  [&] <typename... SortedCs> ( tuplet::tuple<SortedCs...>* ) constexpr noexcept
  {
    i32 sequence = 0;
    ( ( cached_pointers[Traits::Tuple::Index<SortedCs, DataTuple>] = [&]<typename C>( tuplet::tuple<C>* ) constexpr noexcept
    {
      const auto i = pool.GetComponentIndex( Component::info<C>, sequence );
      if constexpr ( std::is_pointer_v<C> )
        return ( i < 0 ) ? nullptr : &pool.m_components[i][sizeof( Raw<C> ) * starting_index.m_value];
      else
        return &pool.m_components[i][sizeof( Raw<C> ) * starting_index.m_value];
    }( Traits::Tuple::null<tuplet::tuple<SortedCs>> ) ), ... );
  }( Traits::Tuple::null<SortedTuple> );
  return cached_pointers;
}

template <typename Func, typename Array>
requires( ContainReferenceParameters<Func> &&std::is_same_v< typename Traits::Function::Type<Func>::ReturnType, void > )
inline void Invoke( Func &&func, Array &cached_pointers ) noexcept
{
  [&] <typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
  {
    using ArgsTuple = tuplet::tuple<Cs...>;
    static_assert( ( ( std::is_reference_v<Cs> )&&... ) );
    func( reinterpret_cast<Cs>( *cached_pointers[Traits::Tuple::Index<Cs, ArgsTuple>] )... );
    ( ( cached_pointers[Traits::Tuple::Index<Cs, ArgsTuple>] += sizeof( Raw<Cs> ) ), ... );
  } ( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );
}

template <typename Func, typename Array>
requires( ContainReferenceParameters<Func> &&std::is_same_v< typename Traits::Function::Type<Func>::ReturnType, bool > )
inline void Invoke( Func &&func, Array &cached_pointers ) noexcept
{
  return[&] <typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
  {
    using ArgsTuple = tuplet::tuple<Cs...>;
    static_assert( ( ( std::is_reference_v<Cs> )&&... ) );
    if ( func( reinterpret_cast<Cs>( *cached_pointers[Traits::Tuple::Index<Cs, ArgsTuple>] )... ) )
      return true;
    ( ( cached_pointers[Traits::Tuple::Index<Cs, ArgsTuple>] += sizeof( Raw<Cs> ) ), ... );
    return false;
  } ( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );
}

template <Traits::Concepts::InvokableFunction Func, typename Array>
requires( ContainReferenceParameters<Func> == false && std::is_same_v< typename Traits::Function::Type<Func>::ReturnType, void > )
inline void Invoke( Func &&func, Array &cached_pointers ) noexcept
{
  [&] <typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
  {
    using ArgsTuple = tuplet::tuple<Cs...>;
    func( [&]<typename C>( tuplet::tuple<C>* ) constexpr noexcept -> C
    {
      auto &m_ptr = cached_pointers[Traits::Tuple::Index<C, ArgsTuple>];
      if constexpr ( std::is_pointer_v<C> )
        if ( m_ptr == nullptr ) return std::bit_cast<C>( nullptr );

      auto ptr = m_ptr;
      m_ptr += sizeof( Raw<C> );

      if constexpr ( std::is_pointer_v<C> ) return std::bit_cast<C>( ptr );
      else return reinterpret_cast<C>( *ptr );
    }( Traits::Tuple::null<tuplet::tuple<Cs>> )... );
  }( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );
}

template<Traits::Concepts::InvokableFunction Func, typename Array>
requires( ContainReferenceParameters<Func> == false && std::is_same_v< typename Traits::Function::Type<Func>::ReturnType, bool > )
inline bool Invoke( Func &&func, Array &cached_pointers ) noexcept
{
  return[&] <typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
  {
    using ArgsTuple = tuplet::tuple<Cs...>;
    return func( [&]<typename C>( tuplet::tuple<C>* ) constexpr noexcept -> C
    {
      auto &m_ptr = cached_pointers[Traits::Tuple::Index<C, ArgsTuple>];
      if constexpr ( std::is_pointer_v<C> )
        if ( m_ptr == nullptr ) return std::bit_cast<C>( nullptr );

      auto ptr = m_ptr;
      m_ptr += sizeof( Raw<C> );

      if constexpr ( std::is_pointer_v<C> ) return std::bit_cast<C>( ptr );
      else return reinterpret_cast<C>( *ptr );
    }( Traits::Tuple::null<tuplet::tuple<Cs>> )... );
  }( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );
}
}

namespace Xivi::ECS::Archetype
{
inline ID GenerateID( Signature signature ) noexcept
{
  return Hash::XXH3_64( signature );
}

/*
* Constructor / Destructor
*/

Instance::Instance( Manager &manager ) noexcept : m_manager( manager )
{}

Instance::~Instance()
{
  m_default_family.~Instance();
}

/*
* Initialise
*/

void Instance::Initialise( ID id, const Signature &signature, const std::string &str ) noexcept
{
  auto &c_desc_manager = Component::DescriptorManager::Instance();

  m_id = id;
  m_signature = signature;
  m_name = str;
  m_exclusive_signature = signature & c_desc_manager.m_exclusive_tag_bits;
  m_tag_component_count = c_desc_manager.CountTagComponent( signature );
  m_exclusive_tag_component_count = c_desc_manager.CountExclusiveTagomponent( signature );
  m_descriptors = c_desc_manager.DescriptorVector( signature );

  if ( signature[Component::info<ShareAsData>.m_bit_index] )
    m_shared_component_count = 0;
  else
    m_shared_component_count = static_cast<u8>( c_desc_manager.CountSharedComponent( signature ) );

  m_data_component_count = c_desc_manager.CountDataComponent( signature );

  if ( m_shared_component_count )
    for ( auto i = 0u; i < m_shared_component_count; ++i )
    {
      Signature shared( Component::GenerateSignature<Entity, ReferenceCount, ShareAsData>() );
      if ( m_descriptors[m_data_component_count + i]->m_build_filter )
        shared.set( Component::info<SharedFilter>.m_bit_index );
      shared.set( m_descriptors[m_data_component_count + i]->m_bit_index );
      m_shared_archetypes.emplace_back( &m_manager.GetOrCreate( shared ) );
    }
}

template <ECS::Concepts::SetterCallback Func>
void Instance::SetConstructor( Func &&func ) noexcept
{
  m_constructor = [&]( Entity entity )
  {
    auto &entry = m_manager.m_world.m_entity_manager.Get( entity );
    using DataTuple = typename Traits::Function::Type<Func>::ArgsTuple;
    auto cached_ptrs = Details::GetComponentPointerArray( *entry.m_pool, entry.m_pool_index, Traits::Tuple::null<DataTuple> );

    Archetype::Details::Invoke( func, cached_ptrs );
  };
  // Handle shared components in the future
}

/*
* ID / Signature
*/

ID Instance::GetID() const noexcept
{
  return m_id;
}

const Signature &Instance::GetSignature() const noexcept
{
  return m_signature;
}

const Signature &Instance::GetExclusiveSignature() const noexcept
{
  return m_exclusive_signature;
}

template <Component::Concepts::IsComponent C>
bool Instance::HaveComponent() const noexcept
{
  return m_signature[Component::info<C>.m_bit_index];
}

bool Instance::HaveComponent( const Component::Descriptor &descriptor ) const noexcept
{
  return m_signature[descriptor.m_bit_index];
}

template <Component::Concepts::IsComponent... Cs>
bool Instance::HaveAllComponents() const noexcept
{
  return ( m_signature[Component::info<Cs>.m_bit_index]  && ... );
}

template <Component::Concepts::IsComponent... Cs>
bool Instance::HaveAllComponents( tuplet::tuple<Cs...> * ) const noexcept
{
  (
    ( "{}", Component::info<Cs>.m_name ), ... );
  return ( m_signature[Component::info<Cs>.m_bit_index]  && ... );
}

bool Instance::HaveAllComponents( DescriptorSpan descriptors ) const noexcept
{
  for ( auto &desc : descriptors )
    if ( !m_signature[desc->m_bit_index] )
      return false;
  return true;
}

/*
* Get SharedComponent
*/

template <Component::Concepts::SharedComponent C>
const C &Instance::GetSharedComponent( const Pool::Family::Instance &family ) const noexcept
{
  XV_CORE_ASSERT_MSG( m_shared_component_count == family.m_shared_descriptors.size(), "[Archetype] Wrong count" );
  for ( u8 i = 0, end = m_shared_component_count; i != end; ++i )
    if ( family.m_shared_descriptors[i] == &Component::info<C> )
    {
      const auto &detail = family.m_shared_details[i];
      const auto &entry = m_manager.m_world.m_entity_manager.Get( detail.m_entity );
      return entry.m_pool->GetComponent<C>( entry.m_pool_index );
    }
  XV_CORE_ASSERT_MSG( false, "[Archetype] Shared component '{}' not found", Component::info<C>.m_name );
  return GetSharedComponent<C>( family );
}

/*
* Families
*/

Pool::Family::Instance &Instance::DefaultFamily() noexcept
{
  return m_default_family;
}

Pool::Family::Instance &Instance::GetOrCreateFamily( DescriptorSpan descriptors, DataSpan datas ) noexcept
{
  XV_CORE_ASSERT_MSG( descriptors.size() == datas.size(), "[Archetype] Size of descriptor and data does not match" );

  if ( m_shared_component_count == 0 )
  {
    if ( m_default_family.m_id.Valid() )
      return m_default_family;
    XV_CORE_ASSERT_MSG( descriptors.size() == 0, "[Archetype] Descriptor is empty" );
    return CreateFamily( Pool::Family::ID { 42ull }, {}, {} );
  }

  Pool::Family::ID id {};
  if ( descriptors.size() == m_shared_component_count )
  {
    id = Pool::Family::GenerateID( m_id, descriptors, datas );
    if ( auto it = m_manager.m_family_map.find( id ); it != m_manager.m_family_map.end() )
      return *it->second;
  }

  array<Component::Type::Shared::Key, Settings::max_shared_components_per_entity> keys;
  array<std::byte *, Settings::max_shared_components_per_entity> shared_datas;
  array<Entity, Settings::max_shared_components_per_entity> shared_entities;
  for ( auto i = 0u; i < m_shared_component_count; ++i )
  {
    auto desc = m_descriptors[m_data_component_count + i];
    i32 index = -1;
    for ( auto j = 0u; j < descriptors.size(); ++j )
      if ( descriptors[j] == desc )
      {
        index = j;
        break;
      }

    if ( index == -1 )
      shared_datas[i] = nullptr;
    else
      shared_datas[i] = datas[index];

    keys[i] = Component::GenerateSharedKey( m_id, *desc, shared_datas[i] );
  }

  for ( auto i = 0u; i < m_shared_component_count; ++i )
  {
    auto desc = m_descriptors[m_data_component_count + i];

    if ( auto it = m_manager.m_shared_component_map.find( keys[i] ); it == m_manager.m_shared_component_map.end() )
    {
      if ( shared_datas[i] )
        shared_entities[i] = m_shared_archetypes[i]->CreateEntity( { &desc, 1u }, { &shared_datas[i], 1u } );
      else
        shared_entities[i] = m_shared_archetypes[i]->CreateEntity( {}, {} );
      m_manager.m_shared_component_map.emplace( keys[i], shared_entities[i] );
    }
    else
    {
      shared_entities[i] = it->second;
      m_manager.m_world.GetEntity( shared_entities[i],
                                   []( ReferenceCount &ref_count )
      {
        ++ref_count.m_value;
      } );
    }
  }

  return CreateFamily( { m_id.m_value }, { shared_entities.data(), m_shared_component_count }, { keys.data(), m_shared_component_count } );
}

Pool::Family::Instance &Instance::GetOrCreateFamilyFromSameArchetype( Pool::Family::Instance &family, Signature signature, DataSpan datas, SharedKeySpan keys ) noexcept
{
  XV_CORE_ASSERT_MSG( signature.count() == keys.size(), "[Archetype] Signature count does not match with key size" );

  Pool::Family::ID id { m_id.m_value };
  array<Component::Type::Shared::Key, Settings::max_shared_components_per_entity> shared_keys;
  array<Entity, Settings::max_shared_components_per_entity> shared_entities;
  array<i32, Settings::max_shared_components_per_entity> remap;
  bool update_filter = false;

  for ( auto i = 0ull, j = 0ull, end = family.m_shared_descriptors.size(); i != end; ++i )
  {
    if ( family.m_shared_descriptors[i]->m_build_filter )
      update_filter = true;

    if ( signature[family.m_shared_descriptors[i]->m_bit_index] )
    {
      remap[j] = static_cast<i32>( i );
      shared_keys[i] = keys[j++];
    }
    else
    {
      shared_keys[i] = family.m_shared_details[i].m_key;
      shared_entities[i] = family.m_shared_details[i].m_entity;
    }
    id.m_value += shared_keys[i].m_value;
  }

  if ( auto it = m_manager.m_family_map.find( id ); it != m_manager.m_family_map.end() )
    return *it->second;

  for ( auto i = 0ull, end = keys.size(); i != end; ++i )
    if ( auto it = m_manager.m_shared_component_map.find( keys[i] ); it != m_manager.m_shared_component_map.end() )
    {
      m_manager.m_world.GetEntity( it->second, []( ReferenceCount &ref_count )
      {
        ref_count.m_value++;
      } );
      XV_CORE_ASSERT_MSG( !it->second.Zombie(), "[Archetype] Entity is a zombie" );
      shared_entities[remap[i]] = it->second;
    }
    else
    {
      Entity entity = m_shared_archetypes[remap[i]]->CreateEntity( { &family.m_shared_descriptors[remap[i]], 1u }, { &datas[i], 1u } );
      m_manager.m_shared_component_map.emplace( keys[i], entity );
      shared_entities[remap[i]] = entity;
    }

  return CreateFamily( id, { shared_entities.data(), m_shared_component_count }, { shared_keys.data(), m_shared_component_count } );
}

Pool::Family::Instance &Instance::GetOrCreateFamilyFromDifferentArchetype( Entity entity ) noexcept
{
  if ( !m_shared_component_count )
  {
    if ( m_default_family.m_id.Valid() )
      return m_default_family;
    return GetOrCreateFamily( {}, {} );
  }

  auto &c_desc_manager = Component::DescriptorManager::Instance();
  auto &entry = m_manager.m_world.m_entity_manager.Get( entity );
  auto &archetype = *entry.m_archetype;
  auto &family = *entry.m_pool->m_family;

  auto descriptor_of_movable = c_desc_manager.DescriptorVector( ( m_signature & archetype.m_signature ) & c_desc_manager.m_shared_bits );

  auto movable_count = descriptor_of_movable.size();

  array<Ptr<byte>, Settings::max_shared_components_per_entity> movable_ptr_array;
  Signature move_signature = archetype.m_signature & c_desc_manager.m_shared_bits;
  for ( auto i = 0u; i < movable_count; ++i )
  {
    const auto &desc = *descriptor_of_movable[i];
    const auto index = GetComponentIndexFromSignature( move_signature, desc.m_bit_index );
    const auto &share_entry = m_manager.m_world.m_entity_manager.Get( family.m_shared_details[index].m_entity );
    const auto index_type = share_entry.m_pool->GetComponentIndex( desc );
    movable_ptr_array[i] = &share_entry.m_pool->m_components[index_type][entry.m_pool_index.m_value * desc.m_size];
  }

  return GetOrCreateFamily( { descriptor_of_movable.data(), movable_count }, { movable_ptr_array.data(), movable_count } );
}

Pool::Family::Instance &Instance::CreateFamily( Pool::Family::ID id, EntitySpan entities, SharedKeySpan keys ) noexcept
{
  UniquePtr<Pool::Family::Instance> family;
  if ( m_default_family.m_id.Valid() )
  {
    if ( id != m_default_family.m_id )
      family = MakeUnique<Pool::Family::Instance>();
    else
      return m_default_family;
  }
  else
    family = UniquePtr<Pool::Family::Instance>( &m_default_family );

  family->Initialise( id, *this, { m_descriptors.data(), m_data_component_count }, { m_descriptors.data() + m_data_component_count, m_shared_component_count }, keys, entities );

  if ( m_shared_component_count )
  {
    XV_CORE_ASSERT_MSG( m_manager.m_family_map.find( id ) == m_manager.m_family_map.end(), "[Archetype] Family not found" );
    m_manager.m_family_map.emplace( id, family.get() );
  }

  auto ptr = family.get();
  AddFamilyToPending( std::move( family ) );
  return *ptr;
}

/*
* Structural Changes
*/

void Instance::AddFamilyToPending( UniquePtr<Pool::Family::Instance> &&family ) noexcept
{
  XV_CORE_ASSERT( nullptr == family->m_next.get() );
  if ( family.get() == &m_default_family )
  {
    family->m_prev = family.get();
    family.release();
  }
  else
  {
    family->m_next = std::move( m_family_pending );
    m_family_pending = std::move( family );
  }
  m_manager.AddToStructuralPendingList( *this );
}

void Instance::UpdateStructuralChanges() noexcept
{
  auto add_shared_filter = [&]( Pool::Family::Instance &family ) noexcept
  {
    for ( auto i = 0ull, end = family.m_shared_descriptors.size(); i != end; ++i )
    {
      auto desc = family.m_shared_descriptors[i];
      if ( desc->m_build_filter )
      {
        m_manager.m_world.GetEntity( family.m_shared_details[i].m_entity, [&]( SharedFilter &filter )
        {
          for ( auto &entry : filter.m_entries )
            if ( entry.m_archetype == this )
            {
              entry.m_families.push_back( &family );
              return;
            }

          filter.m_entries.emplace_back().m_archetype = this;
          filter.m_entries.back().m_families.push_back( &family );
        } );
      }
    }
  };

  if ( m_default_family.m_prev )
  {
    m_default_family.m_prev = nullptr;
    add_shared_filter( m_default_family );
  }

  for ( auto ptr = std::move( m_family_pending ); ptr.get();)
  {
    auto next = std::move( ptr->m_next );
    add_shared_filter( *ptr );
    XV_CORE_ASSERT( ptr.get() != &m_default_family );
    if ( m_default_family.m_next.get() )
      m_default_family.m_next->m_prev = ptr.get();
    ptr->m_next = std::move( m_default_family.m_next );
    m_default_family.m_next = std::move( ptr );
    ptr = std::move( next );
  }
}

/*
* Entities
*/

Entity Instance::CreateEntity( Pool::Family::Instance &family, DescriptorSpan descriptors, DataSpan datas ) noexcept
{
  XV_CORE_ASSERT( HaveAllComponents( descriptors ) );
  Entity result_entity;
  _CreateEntity( family, 1, [&]( Entity entity, u32 )
  {
    auto &entry = m_manager.m_world.m_entity_manager.Get( entity );
    auto &pool = *entry.m_pool;
    auto pool_descriptors = pool.m_descriptors;

    for ( auto i = 0; i < descriptors.size(); ++i )
    {
      auto &desc = *descriptors[i];
      auto index = pool.GetComponentIndex( desc );
      XV_CORE_ASSERT_MSG( index >= 0, "[Archetype] Invalid index" );

      if ( datas[i] )
      {
        if ( desc.m_move )
          desc.m_move( &pool.m_components[index][entry.m_pool_index.m_value * desc.m_size], datas[i] );
        else
        {
          if ( desc.m_destructor )
            desc.m_destructor( &pool.m_components[index][entry.m_pool_index.m_value * desc.m_size] );
          std::memcpy( &pool.m_components[index][entry.m_pool_index.m_value * desc.m_size], datas[i], desc.m_size );
        }
      }
    }
    result_entity = entity;
  } );
  return result_entity;
}

Entity Instance::CreateEntity( DescriptorSpan descriptors, DataSpan datas ) noexcept
{
  XV_CORE_ASSERT( m_shared_component_count == 0 || datas.size() == 1 );
  return CreateEntity( GetOrCreateFamily( {}, {} ), descriptors, datas );
}

template <ECS::Concepts::SetterCallback Func>
Entity Instance::CreateEntity( Func &&func ) noexcept
{
  return _CreateEntities( 1, std::forward<Func &&>( func ) );
}

template <ECS::Concepts::SetterCallback Func>
Entity Instance::CreateEntity( Pool::Family::Instance &family, Func &&func ) noexcept
{
  return _CreateEntities( 1, family, std::forward<Func &&>( func ) );
}

template <ECS::Concepts::SetterCallback Func>
Entity Instance::CreateEntities( u32 count, Func &&func ) noexcept
{
  return _CreateEntities( count, std::forward<Func &&>( func ) );
}

template <typename Func>
void Instance::_CreateEntity( Pool::Family::Instance &family, u32 count, Func &&func ) noexcept
{
  family.Append( count, m_manager, [&]( Pool::Instance &pool, Pool::Index index, u32 alloc_count ) noexcept
  {
    if ( m_events.m_on_entity_created.m_delegates.size() )
      for ( auto i = 0u; i < alloc_count; ++i )
      {
        Pool::Index new_index { index.m_value + static_cast<i32>( i ) };
        auto &entity = pool.GetComponent<Entity>( new_index ) = m_manager.m_world.m_entity_manager.Allocate( *this, pool, new_index );

        if constexpr ( !std::is_same_v<EmptyLambda, Func> )
          func( entity, count );

        m_events.m_on_entity_created.Notify( entity );
      }
    else
      for ( auto i = 0u; i < alloc_count; ++i )
      {
        Pool::Index new_index { index.m_value + static_cast<i32>( i ) };
        auto &entity = pool.GetComponent<Entity>( new_index ) = m_manager.m_world.m_entity_manager.Allocate( *this, pool, new_index );

        if constexpr ( !std::is_same_v<EmptyLambda, Func> )
          func( entity, count );
      }
  } );
}

template <ECS::Concepts::SetterCallback Func>
requires( false == ECS::Concepts::HasSharedParameters<Func> )
Entity Instance::_CreateEntities( i32 count, Func &&func ) noexcept
{
  return _CreateEntities( count, GetOrCreateFamily( {}, {} ), std::forward<Func &&>( func ) );
}

template <ECS::Concepts::SetterCallback Func >
requires( false == ECS::Concepts::HasSharedParameters<Func> )
Entity Instance::_CreateEntities( i32 count, Pool::Family::Instance &family, Func &&func ) noexcept
{
  XV_CORE_ASSERT( HaveAllComponents( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> ) );
  Entity first;
  _CreateEntity( family, count, [&]( Entity entity, i32 ) constexpr noexcept
  {
    if ( !first.Valid() )
      first = entity;

    if constexpr ( !std::is_same_v<EmptyLambda, Func> )
    {
      auto &entry = m_manager.m_world.m_entity_manager.Get( entity );
      using DataTuple = typename Traits::Function::Type<Func>::ArgsTuple;
      auto cached_ptrs = Details::GetComponentPointerArray( *entry.m_pool, entry.m_pool_index, Traits::Tuple::null<DataTuple> );

      Archetype::Details::Invoke( func, cached_ptrs );
    }
    else
    {
      if ( m_constructor )
        m_constructor( entity );
    }
  } );
  return first;
}

template <ECS::Concepts::SetterCallback Func>
requires( true == ECS::Concepts::HasSharedParameters<Func> )
Entity Instance::_CreateEntities( i32 count, Func &&func ) noexcept
{
  XV_CORE_ASSERT( HaveAllComponents( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> ) );

  using ParameterTuple = typename Traits::Function::Type<Func>::ArgsTuple;

  using NoRefsTuple = std::invoke_result_t<decltype( []<typename... Cs>( tuplet::tuple<Cs...> * ) -> tuplet::tuple<std::remove_reference_t<Cs>...>
  {} ), Traits::Tuple::null<ParameterTuple >> ;

  using SharedTuple = SharedOnlyTuple<NoRefsTuple>;
  using DataTuple = DataOnlyTuple<NoRefsTuple>;
  using SortedDataTuple = SortComponentsTuple<DataTuple>;

  static constexpr auto shared_descriptors = [&]<typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
  {
    return array { &Component::info<Cs>... };
  }( Traits::Tuple::null<SharedTuple> );

  const auto component_index = [&]<typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
  {
    i32 sequence = 0;
    return array
    { [&] <typename C>( Ptr<C> ) constexpr noexcept
      {
        while ( m_descriptors[sequence] != &Component::info<C> )
          ++sequence;
        return sequence;
      }( std::bit_cast<Ptr<Cs>>( nullptr ) )...
    };
  }( Traits::Tuple::null<SortedDataTuple> );

  XV_CORE_ASSERT_MSG( component_index[0] != -1, "[Archetype] Invalid index" );

  Entity first;

  for ( i32 i = 0; i < count; ++i )
  {
    NoRefsTuple components {};
    [&] <typename... Cs>( tuplet::tuple<Cs...> &tuple ) constexpr noexcept
    {
      func( tuplet::get<Cs>( tuple )... );
    }( components );

    auto shared_data = [&]<typename... Cs>( tuplet::tuple<Cs...>* )
    {
      return array { std::bit_cast<Ptr<byte>>( &tuplet::get<Cs>( components ) )... };
    }( Traits::Tuple::null<SharedTuple> );

    auto &family = GetOrCreateFamily( shared_descriptors, shared_data );

    _CreateEntity( family, 1, [&]( Entity entity, i32 ) constexpr noexcept
    {
      auto &entry = m_manager.m_world.m_entity_manager.Get( entity );
      if ( i == 0 )
        first = entity;
      [&] <typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
      {
        ( ( std::bit_cast<Cs &>( entry.m_pool->m_components[component_index[TupleIndex<Cs, SortedDataTuple>]][entry.m_pool_index.m_value * sizeof( Cs )] ) = std::move( tuplet::get<Cs>( components ) ) ), ... );
      }( Traits::Tuple::null<SortedDataTuple> );
    } );
  }

  return first;
}

void Instance::DestroyEntity( Entity &entity ) noexcept
{
  XV_CORE_ASSERT_MSG( !entity.Zombie(), "[Archetype] Entity is a zombie" );

  auto &entry = m_manager.m_world.m_entity_manager.m_entities[entity.m_global_index];

  if ( entity.m_validation != entry.m_validation || entry.m_archetype != this )
    XV_CORE_ASSERT_MSG( entity.m_validation != entry.m_validation, "[Archetype] Wrong entry" );

  auto &pool_entity = entry.m_pool->GetComponent<Entity>( entry.m_pool_index );
  if ( pool_entity != entity )
    return;

  m_manager.AddToStructuralPendingList( *entry.m_pool );

  if ( m_events.m_on_entity_destroyed.m_delegates.size() )
    m_events.m_on_entity_destroyed.Notify( pool_entity );

  entity.m_validation.m_zombie = pool_entity.m_validation.m_zombie = entry.m_validation.m_zombie = true;
  entry.m_pool->Delete( entry.m_pool_index );
}

template <ECS::Concepts::SetterCallback Func>
Entity Instance::MoveEntity( Entity &entity, Pool::Family::Instance &family, Func &&func ) noexcept
{
  XV_CORE_ASSERT_MSG( !entity.Zombie(), "[Archetype] Entity is a zombie" );
  auto &entry = m_manager.m_world.m_entity_manager.m_entities[entity.m_global_index];
  auto &from_pool = *entry.m_pool;

  Entity new_entity { entity };
  family.Append( 1, m_manager, [&]( Pool::Instance &pool, Pool::Index index, i32 ) noexcept
  {
    m_manager.AddToStructuralPendingList( from_pool );
    const auto new_index = pool.MoveInFromPool( from_pool, entry.m_pool_index, index );
    entry.m_archetype = this;
    entry.m_pool_index = new_index;
    entry.m_pool = &pool;

    if constexpr ( !std::is_same_v<Func, EmptyLambda> )
    {
      using DataTuple = typename Traits::Function::Type<Func>::ArgsTuple;
      auto cached_ptr = Details::GetComponentPointerArray( pool, new_index, Traits::Tuple::null<DataTuple> );
      Details::Invoke( func, cached_ptr );
    }

    if ( m_events.m_on_entity_moved_in.m_delegates.size() )
    {
      auto &pool_entity = pool.GetComponent<Entity>( entry.m_pool_index );
      m_events.m_on_entity_moved_in.Notify( pool_entity );
      if ( entry.m_validation.m_zombie )
        new_entity = Entity();
    }
  } );
  return new_entity;
}
}