#pragma once

#include <Types/GUID.h>
#include <Utils/Typename.h>

namespace Xivi::Service
{

  using ID = GUID<struct ServiceID>;

  struct Descriptor
  {
    using Destructor = FuncPtr<void(Ptr<byte>) noexcept>;

    Destructor m_destructor{nullptr};
    ID m_id{};
    std::string_view m_name{};
  };

  namespace Details
  {

    template <typename TService>
    consteval Descriptor CreateDescriptor() noexcept
    {
      Descriptor result_desc;

      // ID
      result_desc.m_id = ID(__FUNCSIG__);

      // Name
      result_desc.m_name = Typename<TService>();

      // Destructor
      if constexpr (std::is_trivially_destructible_v<TService>)
        result_desc.m_destructor = nullptr;
      else
        result_desc.m_destructor = [](Ptr<byte> p) noexcept
        {
          std::destroy_at(std::bit_cast<Ptr<TService>>(p));
        };
      return result_desc;
    }

    template <typename TService>
    inline constexpr auto desc = CreateDescriptor<TService>();
  }

  template <typename TService>
  inline constexpr auto &info = Details::desc<Raw<TService>>;
}