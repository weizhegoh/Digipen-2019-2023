#pragma once
#include "Instance.h"
#include <STL/unordered_map.h>

namespace Xivi::Resource::Details
{
struct ResourceID {};
}

namespace Xivi::Resource
{

using ID = GUID<Details::ResourceID>;

template <typename>
class ILoader;

template <typename, typename>
class Handler;

template <typename Base>
class Cache
{
  template <typename, typename>
  friend class Handler;

  friend class ILoader<Base>;

  void IncreaseReference( Resource::ID key );
  void DecreaseReference( Resource::ID key );

  unordered_map<Resource::ID, Resource::Instance<Base>> m_datas;
  UniquePtr<Base> m_fallback { nullptr };
  UniquePtr<ILoader<Base>> m_loader { nullptr };
  u64 m_timestamp { 0 };

protected:
  Cache() = default;

public:
  // Disable copy
  Cache( const Cache & ) = delete;
  Cache &operator=( const Cache & ) = delete;

  // Disable move
  Cache( Cache && ) = delete;
  Cache &operator=( Cache && ) = delete;

  // Destructor
  virtual ~Cache() = default;

  // Resource Cache
  u64 Size() const;
  u64 Timestamp() const;
  void Free();
  void Clear();

  // Resource Data
  void Load( const std::string &file );

  template <typename Specialised>
  requires( std::derived_from<Specialised, Base> )
    Handler<Base, Specialised> Get( const std::string &file );

  template <typename Specialised>
  requires( std::derived_from<Specialised, Base> )
    Handler<Base, Specialised> Get( const Resource::ID key );

  void Set( const std::string &file, Resource::ID key, Ptr<Base> data, State state, Policy policy );
  u64 ReferenceCount( Resource::ID key ) const;
  Resource::State ResourceState( Resource::ID key ) const;

  // Fallback
  Ptr<Base> Fallback();
  Ptr<const Base> Fallback() const;
  void SetFallback( Ptr<Base> fallback_data );

  // Loaders
  Ptr<ILoader<Base>> Loader();
  Ptr<const ILoader<Base>> Loader() const;
  void SetLoader( Ptr<ILoader<Base>> loader );
  void FreeLoader();

  const std::vector<std::string> List() const;
};

} // namespace Xivi

#include "Cache.inl"