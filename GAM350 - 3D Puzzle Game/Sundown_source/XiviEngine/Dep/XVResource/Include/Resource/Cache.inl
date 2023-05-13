namespace Xivi::Resource
{

  //****************************************************************************
  // Resource Cache
  //****************************************************************************

  template <typename Base>
  u64 Cache<Base>::Size() const
  {
    return m_datas.size();
  }

  template <typename Base>
  u64 Cache<Base>::Timestamp() const
  {
    return m_timestamp;
  }

  template <typename Base>
  void Cache<Base>::Free()
  {
    for (auto it = m_datas.begin(); it != m_datas.end();)
      if (it->second._lifetime == Resource::Policy::Managed)
        it = m_datas.erase(it);
      else
        ++it;
  }

  template <typename Base>
  void Cache<Base>::Clear()
  {
    m_datas.clear();
  }

  //****************************************************************************
  // Resource Data
  //****************************************************************************

  template <typename Base>
  void Cache<Base>::Load(const std::string &file)
  {
    if (m_loader)
      m_loader->Load(file);
    else
      XV_CORE_ASSERT_MSG(false, "[Cache] No loader set");
  }

  template <typename Base>
  template <typename Specialised>
  requires(std::derived_from<Specialised, Base>)
      Handler<Base, Specialised> Cache<Base>::Get(const std::string &file)
  {
    Resource::ID key(file);
    if (auto it = m_datas.find(key); it == m_datas.end())
    {
      Set(file, key, nullptr, Resource::State::Loading, Resource::Policy::Managed);
      Load(file);
    }
    return Handler<Base, Specialised>(this, key);
  }

  template <typename Base>
  template <typename Specialised>
  requires(std::derived_from<Specialised, Base>)
      Handler<Base, Specialised> Cache<Base>::Get(const Resource::ID key)
  {
    if (auto it = m_datas.find(key); it == m_datas.end())
      Set("Unreferenced File", key, nullptr, Resource::State::NotFound, Resource::Policy::Managed);
    return Handler<Base, Specialised>(this, key);
  }

  template <typename Base>
  void Cache<Base>::Set(const std::string &file, const ID key, const Ptr<Base> data, const Resource::State state, const Resource::Policy policy)
  {
    XV_CORE_ASSERT_MSG((data == nullptr) == (state == Resource::State::NotFound || state == Resource::State::Loading), "[Cache] Data is nullptr");

    auto it = m_datas.find(key);

    XV_CORE_ASSERT_MSG(it == m_datas.end() || it->second.m_state != Resource::State::Final, "[Cache] Data is Final");

    if (it == m_datas.end())
#ifdef XV_STL
      m_datas.emplace(std::piecewise_construct, tuplet::forward_as_tuple(key), tuplet::forward_as_tuple(file, data, state, policy));
#else
      m_datas.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(file, data, state, policy));
#endif
    else
    {
      it->second.m_file = file;
      it->second.m_data.reset(data);
      it->second.m_state = state;
      it->second.m_policy = policy;
    }
    ++m_timestamp;
  }

  template <typename Base>
  u64 Cache<Base>::ReferenceCount(const Resource::ID key) const
  {
    if (const auto it = m_datas.find(key); it != m_datas.end())
      return it->second.m_reference_count;
    return 0;
  }

  template <typename Base>
  Resource::State Cache<Base>::ResourceState(const Resource::ID key) const
  {
    const auto it = m_datas.find(key);

    if (it == m_datas.end())
    {
      if (m_fallback)
        return Resource::State::NotLoadedFallback;
      else
        return Resource::State::NotLoaded;
    }

    if (!it->second.m_data)
    {
      if (m_fallback)
      {
        if (it->second.m_state == Resource::State::Loading)
          return Resource::State::LoadingFallback;
        else if (it->second.m_state == Resource::State::NotFound)
          return Resource::State::NotFoundFallback;
        else
          return Resource::State::NotLoadedFallback;
      }
      else
        return Resource::State::NotLoaded;
    }

    return it->second.m_state;
  }

  //****************************************************************************
  // Fallback
  //****************************************************************************
  template <typename Base>
  Ptr<Base> Cache<Base>::Fallback()
  {
    return m_fallback.get();
  }

  template <typename Base>
  Ptr<const Base> Cache<Base>::Fallback() const
  {
    return m_fallback.get();
  }

  template <typename Base>
  void Cache<Base>::SetFallback(const Ptr<Base> data)
  {
    m_fallback.reset(data);
    ++m_timestamp;
  }

  //****************************************************************************
  // Loaders
  //****************************************************************************

  template <typename Base>
  Ptr<ILoader<Base>> Cache<Base>::Loader()
  {
    return m_loader;
  }

  template <typename Base>
  Ptr<const ILoader<Base>> Cache<Base>::Loader() const
  {
    return m_loader;
  }

  template <typename Base>
  void Cache<Base>::SetLoader(const Ptr<ILoader<Base>> loader)
  {
    if (m_loader.reset(loader); m_loader)
      m_loader->m_cache = this;
  }

  template <typename Base>
  void Cache<Base>::FreeLoader()
  {
    m_loader.reset();
  }

  //****************************************************************************
  // Reference Counting
  //****************************************************************************

  template <typename Base>
  void Cache<Base>::IncreaseReference(Resource::ID key)
  {
    if (auto it = m_datas.find(key); it != m_datas.end())
      it->second.m_reference_count++;
  }

  template <typename Base>
  void Cache<Base>::DecreaseReference(Resource::ID key)
  {
    if (auto it = m_datas.find(key); it != m_datas.end())
      if (--(it->second.m_reference_count ) == 0 &&
          it->second.m_policy == Resource::Policy::Referenced)
        m_datas.erase(it);
  }

  template <typename Base>
  const std::vector<std::string> Cache<Base>::List() const
  {
    std::vector<std::string> vec;
    for (auto it = m_datas.begin(); it != m_datas.end(); ++it)
      vec.push_back(it->second.m_file);
    return vec;
  }

} // namespace Xivi