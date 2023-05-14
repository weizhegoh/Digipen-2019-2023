#pragma once

#include "Cache.h"

namespace Xivi::Resource
{
  template <typename Base>
  class ILoader
  {
    friend class Cache<Base>;

    Ptr<Cache<Base>> m_cache{nullptr};
    u64 m_requests{0};
    u64 m_loaded{0};
    u64 m_notFound{0};

  public:
    explicit ILoader() = default;

    virtual ~ILoader();

    u64 RequestCount() const;
    u64 LoadedCount() const;
    u64 NotFoundCount() const;
    
    void Load( const std::string& file );

  protected:
    void Set(const std::string& file, Ptr<Base> data, Resource::State state, Resource::Policy policy);

    template <typename Specialised>
    requires(!std::is_null_pointer_v<Raw<Specialised>>)
    void Set(const std::string& file, Specialised &&data, Resource::State state, Resource::Policy policy);

    void Set(const std::string& file, Ptr<Base> data);

    template <typename Specialised>
    requires(!std::is_null_pointer_v<Raw<Specialised>>)
    void Set(const std::string& file, Specialised &&data);

    void SetNotFound(const std::string& file);
    
    virtual void DoLoad( const std::string& file ) = 0;  
  };
} // namespace Xivi

#include "ILoader.inl"