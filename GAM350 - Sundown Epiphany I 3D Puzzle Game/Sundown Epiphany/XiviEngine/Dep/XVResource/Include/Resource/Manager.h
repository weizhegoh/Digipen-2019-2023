#pragma once

#include <Types/Base.h>
#ifdef XV_STL
#include <tuplet/tuple.hpp>
#endif
#include "Handler.h"
#include "ILoader.h"

namespace Xivi::Resource
{
  template <typename... Ts>
  class Manager : private Cache<Ts>...
  {
    template <typename... Resources>
    using ResourceList = Typelist<Resources...>;

  public:
    explicit Manager() = default;

    ~Manager();

    // Cache
    template <typename Base>
    u64 Size() const;

    template <typename Base>
    u64 ReferenceCount(Resource::ID key) const;

    template <typename Base>
    Resource::State ResourceState(Resource::ID key) const;

    // Handler
    template <typename Base>
    Manager<Ts...> &Load( const std::string& file );

    template <typename Base, typename Specialised = Base>
    requires (std::derived_from<Specialised, Base>)
    Handler<Base, Specialised> Get( const Resource::ID key);

    template <typename Base, typename Specialised = Base>
    requires (std::derived_from<Specialised, Base>)
    Handler<Base, Specialised> Get(const std::string& file);

    template <typename Base>
    Manager<Ts...> &Set(const std::string& file, Ptr<Base> data, Resource::State state, Resource::Policy policy);

    template <typename Base>
    Manager<Ts...> &Set(const std::string& file, Base &&data, Resource::State state, Resource::Policy policy);

    template <typename Base>
    Manager<Ts...> &Set(const std::string& file, Ptr<Base> data);

    template <typename Base>
    Manager<Ts...> &Set(const std::string& file, Base &&data);

    template <typename Base>
    Manager<Ts...> &Free();

    Manager<Ts...> &Free();

    template <typename Base>
    Manager<Ts...> &Clear();

    Manager<Ts...> &Clear();

    // Fallbacks
    template <typename Base>
    Ptr<Base> Fallback();

    template <typename Base>
    Ptr<const Base> Fallback() const;

    template <typename Base>
    Manager<Ts...> &SetFallback(Ptr<Base> data);

    template <typename Base>
    Manager<Ts...> &SetFallback(Base &&data);

    // Loaders
    template <typename Base>
    Ptr<ILoader<Base>> Loader();

    template <typename Base>
    Ptr<const ILoader<Base>> Loader() const;

    template <typename Base>
    Manager<Ts...> &SetLoader(Ptr<ILoader<Base>> loader);

    template <typename Base>
    const std::vector<std::string> List() const;

  private:
    template <typename... Resources>
    void FreeInternal();

    template <typename... Resources>
    void ClearInternal();

    template <typename... Resources>
    void FreeLoaders();
  };
} // namespace Xivi

#include "Manager.inl"

#include <Types/Traits/Specialised.h>
namespace Xivi::Concepts
{
template <typename Base>
concept IsResourceType = Xivi::Traits::IsSpecialised<Resource::Handler, Base>();
}