#pragma once
#include <Utils/NonCopyable.h>
#include <Types/Base.h>

namespace Xivi
{
  template <typename... ActionArgs>
  class ActionList : NonCopyable
  {
    using HandleID = size_t;
    using Handle = std::pair<HandleID, Action<const ActionArgs &...>>;
    std::list<Handle> m_handlers;
    HandleID m_top{1ULL};

  public:
    ActionList() = default;
    ~ActionList() = default;

    HandleID Subscribe(Action<const ActionArgs &...> action)
    {
      m_handlers.push_back({m_top, action});
      return m_top++;
    }

    void Unsubscribe(HandleID id)
    {
      auto result = std::find_if(m_handlers.begin(),
                                 m_handlers.end(),
                                 [id](const Handle &handle) {
                                   return handle.first == id;
                                 });

      if (result != m_handlers.end())
        m_handlers.remove_if(
            [id](const Handle &handle) {
              return handle.first == id;
            });
    }

    void Invoke(const ActionArgs &...args)
    {
      for (const Handle &ref : m_handlers)
        ref.second(args...);
    }

    void Clear()
    {
      m_handlers.clear();
      m_top = 1ULL;
    }
  };
}