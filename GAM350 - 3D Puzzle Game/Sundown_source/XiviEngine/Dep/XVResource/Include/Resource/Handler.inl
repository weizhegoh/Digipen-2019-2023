namespace Xivi::Resource
{
  //*******************************************************************
  // Class Handler
  //*******************************************************************

  // Copy Constructor
template <typename Base, typename Specialised>
Handler<Base, Specialised>::Handler( const Handler &rhs ) :
  m_key { rhs.m_key },
  m_cache { rhs.m_cache },
  m_data { rhs.m_data },
  m_timestamp { rhs.m_timestamp },
  m_state { rhs.m_state }
{
  if ( m_cache )
    m_cache->IncreaseReference( m_key );
}

// Copy Assignment
template <typename Base, typename Specialised>
Handler<Base, Specialised> &Handler<Base, Specialised>::operator=( const Handler &rhs )
{
  if ( m_cache )
    m_cache->DecreaseReference( m_key );

  m_cache = rhs.m_cache;
  m_data = rhs.m_data;
  m_key = rhs.m_key;
  m_timestamp = rhs.m_timestamp;
  m_state = rhs.m_state;

  if ( m_cache )
    m_cache->IncreaseReference( m_key );

  return *this;
}

// Move Constructor
template <typename Base, typename Specialised>
Handler<Base, Specialised>::Handler( Handler &&rhs ) :
  m_key { rhs.m_key },
  m_cache { rhs.m_cache },
  m_data { rhs.m_data },
  m_timestamp { rhs.m_timestamp },
  m_state { rhs.m_state }
{
  rhs.m_cache = nullptr;
  rhs.m_data = nullptr;
  rhs.m_key = {};
  rhs.m_timestamp = 0;
  rhs.m_state = Resource::State::Final;
}

// Move Assignment
template <typename Base, typename Specialised>
Handler<Base, Specialised> &Handler<Base, Specialised>::operator=( Handler &&rhs )
{
  std::swap( m_cache, rhs.m_cache );
  std::swap( m_data, rhs.m_data );
  std::swap( m_key, rhs.m_key );
  std::swap( m_timestamp, rhs.m_timestamp );
  std::swap( m_state, rhs.m_state );
  return *this;
}

// Destructor
template <typename Base, typename Specialised>
Handler<Base, Specialised>::~Handler()
{
  if ( m_cache )
    m_cache->DecreaseReference( m_key );
}

template <typename Base, typename Specialised>
bool Handler<Base, Specialised>::operator==( const Handler &rhs ) const
{
  return m_cache == rhs.m_cache && m_key == rhs.m_key;
}

template <typename Base, typename Specialised>
bool Handler<Base, Specialised>::operator!=( const Handler &rhs ) const
{
  return !operator==( rhs );
}
template <typename Base, typename Specialised>
Handler<Base, Specialised>::operator bool()
{
  Sync();
  return m_data;
}

template <typename Base, typename Specialised>
Handler<Base, Specialised>::operator Specialised *( )
{
  Sync();
  return static_cast<Ptr<Specialised>>( m_data );
}

template <typename Base, typename Specialised>
Specialised &Handler<Base, Specialised>::operator*()
{
  Sync();
  return *static_cast<Ptr<Specialised>>( m_data );
}

template <typename Base, typename Specialised>
Specialised *Handler<Base, Specialised>::operator->()
{
  Sync();
  return static_cast<Ptr<Specialised>>( m_data );
}

template <typename Base, typename Specialised>
const Resource::ID &Handler<Base, Specialised>::Key() const
{
  return m_key;
}

template <typename Base, typename Specialised>
const std::string &Handler<Base, Specialised>::File() const
{
  return m_cache->m_datas[m_key].m_file;
}

template <typename Base, typename Specialised>
const State &Handler<Base, Specialised>::ResourceState()
{
  Sync();
  return m_state;
}

// Private Constructor
template <typename Base, typename Specialised>
Handler<Base, Specialised>::Handler( Ptr<Cache<Base>> cache, Resource::ID key ) :
  m_key { key },
  m_cache { cache },
  m_data { nullptr },
  m_timestamp { 0 },
  m_state { Resource::State::NotLoaded }
{
  if ( m_cache )
    m_cache->IncreaseReference( m_key );
}

// Private Internal Sync Handler
template <typename Base, typename Specialised>
void Handler<Base, Specialised>::Sync()
{
  if ( m_state == Resource::State::Final )
    return;

  if ( m_cache->m_timestamp <= m_timestamp )
    return;

  m_timestamp = m_cache->m_timestamp;

  const Resource::Instance<Base> &wrapper = m_cache->m_datas[m_key];
  m_data = wrapper.m_data.get();
  m_state = wrapper.m_state;

  if ( !m_data )
  {
    if ( m_data = m_cache->m_fallback.get(), m_data )
    {
      if ( m_state == Resource::State::Loading )
        m_state = Resource::State::LoadingFallback;
      else if ( m_state == Resource::State::NotFound )
        m_state = Resource::State::NotFoundFallback;
      else
        m_state = Resource::State::NotLoadedFallback;
    }
    else if ( m_state != Resource::State::Loading && m_state != Resource::State::NotFound )
      m_state = Resource::State::NotLoaded;
  }
}

// RTTR
template <typename Base, typename Specialised>
void Handler<Base, Specialised>::SetKey( const Resource::ID &key )
{
  m_key = key;
}

template <typename Base, typename Specialised>
const Resource::ID &Handler<Base, Specialised>::GetKey() const
{
  return m_key;
}

} // namespace Xivi