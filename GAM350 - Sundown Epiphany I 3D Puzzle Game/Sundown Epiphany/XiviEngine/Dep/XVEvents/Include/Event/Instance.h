#pragma once

#include <Types/GUID.h>
#include <Types/Typelist/Typelist.h>
#include <STL/vector.h>
#include <Types/Traits/Specialised.h>
#include <Utils/NonCopyable.h>
#include <string_view>

namespace Xivi::Event
{
  using ID = GUID<struct EventID>;
}

namespace Xivi::Event::Type
{
  struct Global final
  {
    ID m_id{};
    std::string_view m_name{};
  };
}

namespace Xivi::Event
{

  struct Overrides
  {
    static constexpr auto info = Type::Global{};
  };

  template <typename... Args>
  struct Instance : Overrides, NonCopyable
  {
    struct Delegate final
    {
      using Callback = FuncPtr<void(Ptr<void>, Args &&...) noexcept>;
      Callback m_callback;
      Ptr<void> m_class;
    };

    Instance() = default;

    template <auto Func, typename Class>
    inline constexpr void Register(Class &c) noexcept
    {
      m_delegates.emplace_back(
          [](Ptr<void> p, Args && ...args) constexpr noexcept {
            std::invoke(Func, std::bit_cast<Class *>(p), std::forward<Args &&>(args)...);
          },
          &c);
    }

    inline constexpr void Notify(Args &&...args) const noexcept
    {
      for (auto &delegate : m_delegates)
        delegate.m_callback(delegate.m_class, std::forward<Args &&>(args)...);
    }

    vector<Delegate> m_delegates;
  };
}

namespace Xivi::Event::Concepts
{
  template <typename TEvent>
  concept IsEvent = Traits::IsSpecialised<Event::Instance, TEvent>() || std::derived_from<TEvent, Event::Overrides> || std::is_base_of_v<Event::Overrides, TEvent>;

  template <typename List>
  concept IsEventList = IsTypelist<List> &&
      []<IsEvent... TEvents>(Typelist<TEvents...>) constexpr noexcept
  {
    return true;
  }
  (List());
}