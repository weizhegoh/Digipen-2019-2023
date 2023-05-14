#pragma once

#include "Instance.h"

namespace Xivi::Event
{
  struct Descriptor final
  {
    using Destructor = FuncPtr<void(Ptr<byte>) noexcept>;
    Destructor m_destructor;
    ID m_id;
    std::string_view m_name;
  };
}

namespace Xivi::Event::Details
{
  template <Concepts::IsEvent T>
  consteval Descriptor CreateDescriptor() noexcept
  {
    Descriptor descriptor;
    if constexpr (T::info.m_id.m_value)
      descriptor.m_id = T::info.m_id;
    else
      descriptor.m_id = ID(__FUNCSIG__);

    if constexpr (std::is_trivially_destructible_v<T>)
      descriptor.m_destructor = nullptr;
    else
      descriptor.m_destructor = [](Ptr<byte> p) noexcept
      {
        std::destroy_at(std::bit_cast<Ptr<T>>(p));
      };

    if constexpr (T::info.m_name.empty())
      descriptor.m_name = Typename<T>();
    else
      descriptor.m_name = T::info.m_name;
    return descriptor;
  }

  template <typename TEvent>
  inline constexpr auto desc = CreateDescriptor<TEvent>();
}

namespace Xivi::Event
{
  template <typename TEvent>
  static constexpr auto &info = Details::desc<Raw<TEvent>>;
}