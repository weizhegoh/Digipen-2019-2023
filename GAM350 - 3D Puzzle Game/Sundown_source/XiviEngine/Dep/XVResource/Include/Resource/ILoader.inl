namespace Xivi::Resource
{
  template <typename Base>
  ILoader<Base>::~ILoader()
  {
    if (m_cache)
      m_cache->m_loader.reset();
  }

  template <typename Base>
  u64 ILoader<Base>::RequestCount() const
  {
    return m_requests;
  }

  template <typename Base>
  u64 ILoader<Base>::LoadedCount() const
  {
    return m_loaded;
  }

  template <typename Base>
  u64 ILoader<Base>::NotFoundCount() const
  {
    return m_notFound;
  }

  template <typename Base>
  void ILoader<Base>::Load(const std::string &file)
  {
    m_requests++;
    DoLoad(file);
  }

  template <typename Base>
  void ILoader<Base>::Set(const std::string& file, Ptr<Base> data, Resource::State state, Resource::Policy policy)
  {
    if (data)
      m_loaded++;

    if (!data && state == Resource::State::NotFound)
      m_notFound++;

    m_cache->Set(file, Resource::ID(file), data, state, policy);
  }

  template <typename Base>
  template <typename Specialised>
  requires(!std::is_null_pointer_v<Raw<Specialised>>) void ILoader<Base>::Set(const std::string& file, Specialised &&data, Resource::State state, Resource::Policy policy)
  {
    Set(file, new Raw<Specialised>(std::forward<Specialised>(data)), state, policy);
  }

  template <typename Base>
  void ILoader<Base>::Set(const std::string& file, Ptr<Base> data)
  {
    Set(file, data, Resource::State::Final, Resource::Policy::Persistant);
  }

  template <typename Base>
  template <typename Specialised>
  requires(!std::is_null_pointer_v<Raw<Specialised>>) void ILoader<Base>::Set(const std::string& file, Specialised &&data)
  {
    Set(file, new Raw<Specialised>(std::forward<Specialised>(data)));
  }

  template <typename Base>
  void ILoader<Base>::SetNotFound(const std::string& file)
  {
    Set(file, nullptr, Resource::State::NotFound, Resource::Policy::Persistant);
  }

} // namespace Xivi