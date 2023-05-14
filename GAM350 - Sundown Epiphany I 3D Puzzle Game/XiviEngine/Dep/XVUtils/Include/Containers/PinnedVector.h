#pragma once

#include <Core.h>
#include <Types/Base.h>
#include <Utils/Assert.h>

namespace Xivi
{

template <typename T,
  u64 MaxEntries = 65536,
  u64 PageSize = 4096,
  u64 PageSizePower = std::bit_width( PageSize ) - 1,
  u64 PageCount = []
{
  u64 count = ( MaxEntries * sizeof( T ) ) / PageSize;
  return count + ( ( count * PageSize ) < ( MaxEntries * sizeof( T ) ) );
}( ) >
class PinnedVector
{
public:
  using value_type = T;
  using iterator = Ptr<value_type>;
  using const_iterator = Ptr<const value_type>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = u64;

private:
  value_type *m_container { nullptr };
  size_type m_page_count { 0 };
  size_type m_size { 0 };

public:
  // Constructors
  PinnedVector()
  {
    Initialise();
  }

  PinnedVector( const PinnedVector &rhs )
  {
    Initialise();
    ExpandInternal( rhs.m_size );
    std::copy( rhs.begin(), rhs.end(), begin() );
  }

  PinnedVector( PinnedVector &&rhs ) : m_container { rhs.m_container }, m_page_count { rhs.m_page_count }, m_size { rhs.m_size }
  {
    rhs.m_container = nullptr;
    rhs.m_page_count = rhs.m_size = 0;

    rhs.Initialise();
  }

  ~PinnedVector()
  {
    free();
  }

  // Assignment
  PinnedVector &operator=( const PinnedVector &rhs )
  {
    if (this == &rhs)
      return *this;

    Initialise();
    ExpandInternal( rhs.m_size );
    std::copy( rhs.begin(), rhs.end(), begin() );
    return *this;
  }

  PinnedVector &operator=( PinnedVector &&rhs )
  {
    if (this == &rhs)
      return *this;

    free();

    m_container = rhs.m_container;
    m_page_count = rhs.m_page_count;
    m_size = rhs.m_size;

    rhs.m_container = nullptr;
    rhs.m_page_count = rhs.m_size = 0;

    rhs.Initialise();
    return *this;
  }

  // Accessor
   reference at( u64 index )
  {
    XV_CORE_ASSERT_MSG( index < m_size, "[PinnedVector] Out of bound" );
    return m_container[index];
  }

  const_reference at( u64 index ) const
  {
    XV_CORE_ASSERT_MSG(index < m_size, "[PinnedVector] Out of bound");
    return m_container[index];
  }

  reference operator[]( u64 index )
  {
    return m_container[index];
  }

  const_reference operator[]( u64 index ) const
  {
    return m_container[index];
  }

  reference front()
  {
    return m_container[0];
  }

  const_reference front() const
  {
    return m_container[0];
  }

  reference back()
  {
    return m_container[m_size - 1];
  }

  const_reference back() const
  {
    return m_container[m_size - 1];
  }

  Ptr<value_type> data()
  {
    return m_container;
  }

  // Iterators
  iterator begin() noexcept
  {
    return m_size ? m_container : nullptr;
  }

  const_iterator begin() const noexcept
  {
    return m_size ? m_container : nullptr;
  }

  const_iterator cbegin() const noexcept
  {
    return m_size ? m_container : nullptr;
  }

  iterator end() noexcept
  {
    return m_size ? &m_container[m_size] : nullptr;
  }

  const_iterator end() const noexcept
  {
    return m_size ? &m_container[m_size] : nullptr;
  }

  const_iterator cend() const noexcept
  {
    return m_size ? &m_container[m_size] : nullptr;
  }

  // Capacity
  bool empty() const noexcept
  {
    return m_size == 0;
  }

  size_type size() const noexcept
  {
    return m_size;
  }

  size_type max_size() const noexcept
  {
    return MaxEntries;
  }

  size_type capacity() const noexcept
  {
    return ( PageSize * m_page_count ) / sizeof( value_type );
  }

  // Modifiers
  void clear()
  {
    if ( std::is_destructible_v<value_type> )
    {
      for ( auto i = 0ull; i < m_size; ++i )
        std::destroy_at( &m_container[i] );
    }

    VirtualFree( m_container, m_page_count * PageSize, MEM_DECOMMIT );

    m_page_count = m_size = 0;
  }

  void push_back( const T &val )
  {
    ExpandInternal( m_size + 1 );
    m_container[m_size - 1] = val;
  }

  void push_back( T &&val )
  {
    emplace_back( std::move( val ) );
  }

  template <typename... Args>
  reference emplace_back( Args &&...args )
  {
    ExpandInternal( m_size + 1 );
    new ( &m_container[m_size - 1] ) value_type { std::forward<Args>( args )... };
    return m_container[m_size - 1];
  }

  iterator erase( const_iterator c_iter )
  {
    iterator iter (begin() + (c_iter - begin()));

    if ( std::is_destructible_v<value_type> )
      std::destroy_at( iter );


    iterator next = std::next( iter );
    while ( next != end() )
    {
      *iter = std::move( *next);
      ++next;
      ++iter;
    }

    DownsizeInternal(m_size - 1);
    return iter;
  }

  void pop_back()
  {
    if ( std::is_destructible_v<value_type> )
      std::destroy_at( &m_container[m_size - 1] );

    DownsizeInternal( m_size - 1 );
  }

  void swap( PinnedVector &rhs ) noexcept
  {
    std::swap( m_container, rhs.m_container );
    std::swap( m_page_count, rhs.m_page_count );
    std::swap( m_size, rhs.m_size );
  }

private:
  void Initialise()
  {
    if ( m_container )
      free();

    m_container = reinterpret_cast<value_type *>( VirtualAlloc( nullptr,
                                                  PageCount * PageSize,
                                                  MEM_RESERVE,
                                                  PAGE_NOACCESS ) );
  }

  void ExpandInternal( u64 count )
  {
    if (m_page_count * PageSize < count * sizeof(T))
    {
      const u64 offset = m_size * sizeof( value_type );
      const u64 new_page_count = 1 + ( ( offset + sizeof( value_type ) * ( count - m_size ) ) >> PageSizePower );

      if ( new_page_count > m_page_count )
      {
        auto raw = &std::bit_cast<Ptr<byte>>( m_container )[m_page_count * PageSize];
        auto allocation = VirtualAlloc( raw, ( new_page_count - m_page_count ) * PageSize, MEM_COMMIT, PAGE_READWRITE );
        XV_CORE_ASSERT_MSG( raw == allocation, "[PinnedVector] Allocation does not match");
        m_page_count = new_page_count;
      }
    }
    m_size = count;
  }

  void DownsizeInternal( u64 count )
  {
    if (m_page_count * PageSize - PageSize  > count * sizeof(T))
    {
      const u64 offset = m_size * sizeof( value_type );
      const u64 new_page_count = 1 + ( ( offset + sizeof( value_type ) * ( m_size - count ) ) >> PageSizePower );

      if ( m_page_count > new_page_count )
      {
        auto raw = &std::bit_cast<Ptr<byte>>( m_container )[new_page_count * PageSize];
        auto allocation = VirtualAlloc( raw, ( m_page_count - new_page_count ) * PageSize, MEM_DECOMMIT, PAGE_READWRITE );
        XV_CORE_ASSERT_MSG( raw == allocation, "[PinnedVector] Allocation does not match");
        m_page_count = new_page_count;
      }
    }
    m_size = count;
  }

  void free()
  {
    clear();
    VirtualFree( m_container, 0, MEM_RELEASE );
  }
};

}