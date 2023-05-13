namespace Xivi::Resource
{

template <typename... Ts>
Manager<Ts...>::~Manager()
{
  FreeLoaders<Ts...>();
}

template <typename... Ts>
template <typename Base>
u64 Manager<Ts...>::Size() const
{
  return Cache<Base>::Size();
}

template <typename... Ts>
template <typename Base>
u64 Manager<Ts...>::ReferenceCount( Resource::ID key ) const
{
  return Cache<Base>::ReferenceCount( key );
}

template <typename... Ts>
template <typename Base>
Resource::State Manager<Ts...>::ResourceState( Resource::ID key ) const
{
  return Cache<Base>::ResourceState( key );
}

template <typename... Ts>
template <typename Base>
Manager<Ts...> &Manager<Ts...>::Load( const std::string& file )
{
  Cache<Base>::Load( file );
  return *this;
}

template <typename... Ts>
template <typename Base, typename Specialised>
requires (std::derived_from<Specialised, Base>)
Handler<Base, Specialised> Manager<Ts...>::Get( const Resource::ID key )
{
  return Cache<Base>::template Get<Specialised>( key );
}

template <typename... Ts>
template <typename Base, typename Specialised>
requires (std::derived_from<Specialised, Base>)
Handler<Base, Specialised> Manager<Ts...>::Get( const std::string& file )
{
  return Cache<Base>::template Get<Specialised>( file );
}

template <typename... Ts>
template <typename Base>
Manager<Ts...> &Manager<Ts...>::Set( const std::string& file, Ptr<Base> data, Resource::State state, Resource::Policy policy )
{
  Cache<Base>::Set( file, Resource::ID(file), data, state, policy );
  return *this;
}

template <typename... Ts>
template <typename Base>
Manager<Ts...> &Manager<Ts...>::Set( const std::string& file, Base &&data, Resource::State state, Resource::Policy policy )
{
  return Set( file, new Raw<Base>( std::forward<Base>( data ) ), state, policy );
}

template <typename... Ts>
template <typename Base>
Manager<Ts...> &Manager<Ts...>::Set( const std::string& file, Ptr<Base> data )
{
  return Set( file, data, Resource::State::Final, Resource::Policy::Persistant );
}

template <typename... Ts>
template <typename Base>
Manager<Ts...> &Manager<Ts...>::Set( const std::string& file, Base &&data )
{
  return Set( file, new Raw<Base>( std::forward<Base>( data ) ) );
}

template <typename... Ts>
template <typename Base>
Manager<Ts...> &Manager<Ts...>::Free()
{
  Cache<Base>::Free();
  return *this;
}

template <typename... Ts>
Manager<Ts...> &Manager<Ts...>::Free()
{
  FreeInternal<Ts...>();
  return *this;
}

template <typename... Ts>
template <typename Base>
Manager<Ts...> &Manager<Ts...>::Clear()
{
  Cache<Base>::Clear();
  return *this;
}

template <typename... Ts>
Manager<Ts...> &Manager<Ts...>::Clear()
{
  ClearInternal<Ts...>();
  return *this;
}

template <typename... Ts>
template <typename Base>
Ptr<Base> Manager<Ts...>::Fallback()
{
  return Cache<Base>::Fallback();
}

template <typename... Ts>
template <typename Base>
Ptr<const Base> Manager<Ts...>::Fallback() const
{
  return Cache<Base>::Fallback();
}

template <typename... Ts>
template <typename Base>
Manager<Ts...> &Manager<Ts...>::SetFallback( Ptr<Base> data )
{
  Cache<Base>::SetFallback( data );
  return *this;
}

template <typename... Ts>
template <typename Base>
Manager<Ts...> &Manager<Ts...>::SetFallback( Base &&data )
{
  return SetFallback( new Raw<Base>( std::forward<Base>( data ) ) );
}

template <typename... Ts>
template <typename Base>
Ptr<ILoader<Base>> Manager<Ts...>::Loader()
{
  return Cache<Base>::Loader();
}

template <typename... Ts>
template <typename Base>
Ptr<const ILoader<Base>> Manager<Ts...>::Loader() const
{
  return Cache<Base>::Loader();
}

template <typename... Ts>
template <typename Base>
Manager<Ts...> &Manager<Ts...>::SetLoader( Ptr<ILoader<Base>> loader )
{
  Cache<Base>::SetLoader( loader );
  return *this;
}

template <typename... Ts>
template <typename Base>
const std::vector<std::string> Manager<Ts...>::List() const
{
  return Cache<Base>::List();
}

template <typename... Ts>
template <typename... Resources>
void Manager<Ts...>::FreeInternal()
{
  ( Free<Resources>(), ... );
}

template <typename... Ts>
template <typename... Resources>
void Manager<Ts...>::ClearInternal()
{
  ( Clear<Resources>(), ... );
}

template <typename... Ts>
template <typename... Resources>
void Manager<Ts...>::FreeLoaders()
{
  (Cache<Resources>::FreeLoader(),...);
}

} // namespace Xivi