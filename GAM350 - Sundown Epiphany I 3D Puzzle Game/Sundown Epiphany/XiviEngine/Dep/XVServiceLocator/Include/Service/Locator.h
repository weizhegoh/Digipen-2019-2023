#pragma once

#include <Utils/Enumerate.h>
#include <Utils/Assert.h>
#include <unordered_map>
#include "Descriptor.h"

namespace Xivi::Service
{

  struct Locator
  {
    inline static std::vector<Ptr<byte>> services;
    inline static std::vector<Ptr<const Service::Descriptor>> descriptors;

    template <typename TService, typename... Args>
    static TService &Register(Args &&...args)
    {
      for (const auto &ptr : descriptors)
        if (ptr->m_id == Service::info<TService>.m_id)
          XV_CORE_ASSERT_MSG(false, "[Service] Service already registered");
    descriptors.push_back( &Service::info<TService> );
    return *std::bit_cast<Ptr<TService>>( services.emplace_back( std::bit_cast<Ptr<byte>>( new TService( std::forward<Args>( args )... ) ) ) );
    }

    template <typename TService>
    static TService &Register()
    {
      for (const auto &ptr : descriptors)
        if (ptr->m_id == Service::info<TService>.m_id)
          XV_CORE_ASSERT_MSG(false, "[Service] Service already registered");
    descriptors.push_back( &Service::info<TService> );
    return *std::bit_cast<Ptr<TService>>( services.emplace_back( std::bit_cast<Ptr<byte>>( new TService() ) ) );
    }

    template <typename TService>
    static TService &Get()
    {
      Ptr<TService> result{nullptr};
      for (const auto &[i, ptr] : Enumerate(descriptors))
        if (ptr->m_id == Service::info<TService>.m_id)
          result = std::bit_cast<Ptr<TService>>(services[i]);
      XV_CORE_ASSERT_MSG(result != nullptr, "[Service] Service is not registered");
      return *result;
    }

    static void Shutdown()
    {
      for (const auto &[i, ptr] : REnumerate(descriptors))
      {
        if (ptr->m_destructor)
          ptr->m_destructor(services[i]);
        delete services[i];
      }
    }
  };

}