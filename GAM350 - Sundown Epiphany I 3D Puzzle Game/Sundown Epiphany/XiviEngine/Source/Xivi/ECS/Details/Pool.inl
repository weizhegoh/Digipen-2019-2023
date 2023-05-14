/*****************************************************************
*\file         Pool.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::Pool
{
/*
* Constructor / Destructor
*/

inline int GetPage( const Component::Descriptor &info, i32 entity ) noexcept
{
  return ( ( entity * static_cast<i32>( info.m_size ) ) - 1 ) / Settings::page_size;
}

Instance::~Instance() noexcept
{
  Clear();
  if ( m_components.size() )
  {
    for ( auto i = 0ull, end = m_descriptors.size(); i < end; ++i )
      VirtualFree( m_components[i], 0, MEM_RELEASE );
  }
}

/*
* Modifiers
*/

void Instance::Initialise( DescriptorSpan descriptors, Family::Instance &family ) noexcept
{
  XV_CORE_ASSERT_MSG( descriptors.size(), "[Pool] Descriptor is empty" );

  m_descriptors = descriptors;
  m_family = &family;

  for ( const auto &desc : m_descriptors )
  {
    XV_CORE_ASSERT_MSG( desc->m_size <= Settings::page_size, "[Pool] Descriptor size is more than page size" );
    auto current_page = GetPage( *desc, Settings::max_entities ) + 1;
    m_components.emplace_back( std::bit_cast<Ptr<byte>>( VirtualAlloc( nullptr, current_page * Settings::page_size, MEM_RESERVE, PAGE_NOACCESS ) ) );
    XV_CORE_ASSERT_MSG( m_components.back(), "[Pool] Component not found" );
  }
}

i32 Instance::Size() const noexcept
{
  return m_current;
}

Index Instance::Append( u32 count ) noexcept
{
  for ( const auto &[i, desc] : Enumerate( m_descriptors ) )
  {
    auto next_page = GetPage( *desc, m_size + count );
    if ( i32 current_page = GetPage( *desc, m_size ); m_size == 0 || current_page != next_page )
    {
      if ( m_size == 0 ) --current_page;
      auto page_ptr = m_components[i] + Settings::page_size * ( current_page + 1 );
      auto ptr = std::bit_cast<Ptr<byte>>( VirtualAlloc( page_ptr, ( next_page - current_page ) * Settings::page_size, MEM_COMMIT, PAGE_READWRITE ) );
      (void) ptr;
      XV_CORE_ASSERT_MSG( ptr == page_ptr, "[Pool] Invalid referenced memory" );
    }

    if ( desc->m_constructor )
    {
      auto ptr = &m_components[i][m_size * desc->m_size];
      for ( auto j = count; j; --j )
      {
        desc->m_constructor( ptr );
        ptr += desc->m_size;
      }
    }
  }
  Index index { m_size };
  m_size += count;
  m_current = m_size;
  return index;
}

void Instance::Delete( Index index ) noexcept
{
  auto &entity = GetComponent<Entity>( index );
  XV_CORE_ASSERT_MSG( entity.Zombie(), "[Pool]  Entity is Zombie" );
  entity.m_validation.m_generation = m_global_delete_index;
  m_global_delete_index = entity.m_global_index;
}

void Instance::Clear() noexcept
{
  if ( m_size )
  {
    while ( m_size )
      Free( { m_size - 1 }, true );
  }
}

Index Instance::MoveInFromPool( Instance &old_pool, Index old_index, Index new_index ) noexcept
{
  i32 src_index = 0;
  i32 dest_index = 0;
  const i32 src_count = static_cast<i32>( old_pool.m_descriptors.size() );
  const i32 dest_count = static_cast<i32>( m_descriptors.size() );
  while ( true )
    if ( old_pool.m_descriptors[src_index] == m_descriptors[dest_index] )
    {
      auto &desc = *old_pool.m_descriptors[src_index];
      if ( desc.m_move )
      {
        desc.m_move( &m_components[dest_index][new_index.m_value * desc.m_size],
                     &old_pool.m_components[src_index][old_index.m_value * desc.m_size] );
      }
      else
        std::memcpy(
        &m_components[dest_index][new_index.m_value * desc.m_size],
        &old_pool.m_components[src_index][old_index.m_value * desc.m_size],
        desc.m_size );

      if ( desc.m_destructor ) desc.m_destructor( &old_pool.m_components[src_index][desc.m_size * old_index.m_value] );

      ++src_index;
      ++dest_index;
      if ( src_index >= src_count || dest_index >= dest_count ) break;
    }
    else if ( old_pool.m_descriptors[src_index]->m_id.m_value < m_descriptors[dest_index]->m_id.m_value )
    {
      auto &desc = *old_pool.m_descriptors[src_index];
      if ( desc.m_destructor ) desc.m_destructor( &old_pool.m_components[src_index][desc.m_size * old_index.m_value] );
      if ( ++src_index >= src_count ) break;
    }
    else if ( ++dest_index >= dest_count ) break;

  //
  // Destruct any pending component
  //
  while ( src_index < src_count )
  {
    auto &desc = *old_pool.m_descriptors[src_index];
    if ( desc.m_destructor )
      desc.m_destructor( &old_pool.m_components[src_index][desc.m_size * old_index.m_value] );
    ++src_index;
  }

  //
  // Put the deleted entity into the move deleted linklist
  //
  {
    auto &entity = old_pool.GetComponent<Entity>( old_index );
    entity.m_validation.m_zombie = true;
    entity.m_validation.m_generation = old_pool.m_move_delete_index;
    old_pool.m_move_delete_index = old_index.m_value;
  }

  return new_index;
}

/*
* Component Accessor
*/

i32 Instance::GetComponentIndex( const Component::Descriptor &descriptor ) const noexcept
{
  for ( const auto &[i, desc] : Enumerate( m_descriptors ) )
  {
    if ( &descriptor == desc ) return static_cast<i32>( i );
    [[unlikely]] if ( CompareDescriptor( &descriptor, desc ) ) return -1;
  }
  return -1;
}

i32 Instance::GetComponentIndex( const Component::Descriptor &descriptor, i32 &sequence ) const noexcept
{
  const auto tmp = sequence;
  for ( const auto end = static_cast<i32>( m_descriptors.size() ); sequence < end; ++sequence )
  {
    if ( &descriptor == m_descriptors[sequence] ) return sequence++;
    [[unlikely]] if ( CompareDescriptor( &descriptor, m_descriptors[sequence] ) ) break;
  }
  sequence = tmp;
  return -1;
}

template <Component::Concepts::IsComponent C>
Raw<C> &Instance::GetComponent( Index index ) noexcept
{
  using RealC = Raw<C>;
  if constexpr ( std::is_same_v<Entity, RealC> )
    return *std::bit_cast<RealC *>( &m_components[0][index.m_value * sizeof( RealC )] );
  else
  {
    const auto i = GetComponentIndex( Component::info<RealC> );
    return *std::bit_cast<RealC *>( &m_components[i][index.m_value * sizeof( RealC )] );
  }
}

template <Component::Concepts::IsComponent C>
Raw<C> &Instance::GetComponent( Index index, i32 &sequence ) noexcept
{
  using RealC = Raw<C>;
  if constexpr ( std::is_same_v<Entity, RealC> )
  {
    XV_CORE_ASSERT_MSG( sequence++ == 0, "[Pool] There is no sequence" );
    return *std::bit_cast<RealC *>( &m_components[0][index.m_value * sizeof( RealC )] );
  }
  else
  {
    const auto i = GetComponentIndex( Component::info<RealC>, sequence );
    return *std::bit_cast<RealC *>( &m_components[i][index.m_value * sizeof( RealC )] );
  }
}

Ptr<byte> Instance::GetComponent( Index index, const Component::Descriptor &descriptor ) noexcept
{
  const auto i = GetComponentIndex( descriptor );
  return &m_components[i][index.m_value * descriptor.m_size];
}

template <Component::Concepts::IsComponent... Cs>
tuplet::tuple<Cs&...> Instance::GetComponents( Index index ) noexcept
{
  return tuplet::tie( GetComponent<Cs>( index )... );
}

vector<Ptr<byte>> Instance::GetComponents( Index index, const Signature &signature ) noexcept
{
  vector<Ptr<byte>> vec;
  for ( const auto &[i, desc] : Enumerate( m_descriptors ) )
    if ( signature[desc->m_bit_index] ) vec.push_back( &m_components[i][index.m_value * desc->m_size] );
  return vec;
}

/*
* Structural Changes
*/

void Instance::UpdateStructuralChanges( EntityManager &em ) noexcept
{
  const auto old_size = m_size;

  while ( m_global_delete_index != invalid_index )
  {
    auto &entry = em.m_entities[m_global_delete_index];
    auto &entity = *std::bit_cast<Ptr<Entity>>( &m_components[0][sizeof( Entity ) * entry.m_pool_index.m_value] );
    const auto next_global_delete_index = entity.Generation();
    XV_CORE_ASSERT_MSG( entity.Zombie(), "[Pool] Entity is Zombie" );

    if ( Free( entry.m_pool_index, true ) ) em.Delete( m_global_delete_index, entity );
    else em.Delete( m_global_delete_index );
    m_global_delete_index = next_global_delete_index;
  }

  while ( m_move_delete_index != invalid_index )
  {
    auto &entity = *std::bit_cast<Ptr<Entity>>( &m_components[0][sizeof( Entity ) * m_move_delete_index] );
    const auto next_global_delete_index = entity.Generation();
    if ( Free( { static_cast<i32>( m_move_delete_index ) }, false ) )
      em.Move( { static_cast<i32>( m_move_delete_index ) }, entity );
    m_move_delete_index = next_global_delete_index;
  }

  if ( m_size < old_size )
    for ( const auto &[i, desc] : Enumerate( m_descriptors ) )
    {
      const auto page = GetPage( *desc, old_size );
      if ( auto current = GetPage( *desc, m_size ); current != page || m_size == 0 )
      {
        if ( m_size == 0 ) --current;
        auto ptr = &m_components[i][Settings::page_size * ( current + 1 )];
        auto page_count = page - current;
        XV_CORE_ASSERT_MSG( page_count > 0, "[Pool] There is no page" );
        auto b = VirtualFree( ptr, static_cast<u32>( page_count ) * Settings::page_size, MEM_DECOMMIT );
        XV_CORE_ASSERT_MSG( b, "[Pool] Problem with VirtualFree" );
      }
    }
  m_current = m_size;
}

/*
* Internals
*/

bool Instance::Free( Index index, bool destructor ) noexcept
{
  XV_CORE_ASSERT_MSG( index.m_value >= 0, "[Pool] Invalid index" );
  if ( index.m_value >= m_size )
    return false;
  XV_CORE_ASSERT_MSG( m_size, "[Pool] The pool is empty" );

  do
  {
    m_current = --m_size;
    if ( !std::bit_cast<Ptr<Entity>>( &m_components[0][sizeof( Entity ) * m_size] )->Zombie() )
    {
      if ( m_size == index.m_value )
      {
        if ( destructor )
          for ( const auto &[i, desc] : Enumerate( m_descriptors ) )
          {
            auto data = m_components[i];
            if ( desc->m_destructor ) desc->m_destructor( &data[index.m_value * desc->m_size] );
          }
        return false;
      }
      m_current = ++m_size;
      break;
    }

    if ( destructor )
      for ( const auto &[i, desc] : Enumerate( m_descriptors ) )
      {
        auto data = m_components[i];
        if ( desc->m_destructor ) desc->m_destructor( &data[index.m_value * desc->m_size] );
      }
  } while ( m_size );

  if ( index.m_value < m_size )
  {
    m_current = --m_size;
    for ( const auto &[i, desc] : Enumerate( m_descriptors ) )
    {
      auto data = m_components[i];
      if ( desc->m_move )
      {
        desc->m_move( &data[index.m_value * desc->m_size], &data[m_size * desc->m_size] );
        if ( destructor && desc->m_destructor ) desc->m_destructor( &data[m_size * desc->m_size] );
      }
      else
      {
        if ( destructor && desc->m_destructor ) desc->m_destructor( &data[index.m_value * desc->m_size] );
        std::memcpy( &data[index.m_value * desc->m_size], &data[m_size * desc->m_size], desc->m_size );
      }
    }
    return true;
  }
  return false;
}
}