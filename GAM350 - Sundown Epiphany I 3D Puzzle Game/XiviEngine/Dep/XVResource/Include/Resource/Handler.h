#pragma once
#include <type_traits>
#include "Enum.h"
#include "Cache.h"

namespace Xivi::Resource
{

template <typename Base, typename Specialised = Base>
class Handler
{
public:
  // Default
  explicit Handler() = default;

  // Copy
  Handler( const Handler &rhs );
  Handler &operator=( const Handler &rhs );

  // Move
  Handler( Handler &&rhs );
  Handler &operator=( Handler &&rhs );

  // Destructor
  ~Handler();

  bool operator==( const Handler &rhs ) const;
  bool operator!=( const Handler &rhs ) const;
  operator bool();
  operator Specialised *( );
  Specialised &operator*();
  Specialised *operator->();

  const Resource::ID &Key() const;
  const std::string &File() const;
  const Resource::State &ResourceState();

  // RTTR
  void SetKey( const Resource::ID &key );
  const Resource::ID &GetKey() const;

private:
  friend Cache<Base>;

  Handler( Ptr<Cache<Base>> cache, Resource::ID key );
  void Sync();

  Resource::ID m_key {};
  Ptr<Cache<Base>> m_cache { nullptr };
  Ptr<Base> m_data { nullptr };
  u64 m_timestamp { 0 };
  Resource::State m_state { Resource::State::Final };
};

}

#include "Handler.inl"