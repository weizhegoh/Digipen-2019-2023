#pragma once

#include <Utils/Assert.h>
#include <Containers/PinnedVector.h>
#include <STL/unordered_map.h>
#include "Descriptor.h"

namespace Xivi::Event
{
  struct Manager final : NonCopyable
  {
    template <Concepts::IsEvent TEvent>
    inline void Register() noexcept
    {
      XV_CORE_ASSERT_MSG(m_events_map.find(info<TEvent>.m_id) == m_events_map.end(), "[EventManager] Event already registered");
      m_events.push_back({MakeUnique<TEvent>(), &info<TEvent>});
      m_events_map.emplace(info<TEvent>.m_id, m_events.back().first.get());
    }

    template <Concepts::IsEvent TEvent>
    inline TEvent &Get() noexcept
    {
      auto it = m_events_map.find(info<TEvent>.m_id);
      XV_CORE_ASSERT_MSG(it != m_events_map.end(), "[EventManager] Event cannot be found");
      return *std::bit_cast<TEvent *>(it->second);
    }

    ~Manager()
    {
      for (auto& [ptr, desc] : m_events)
        if ( desc->m_destructor )
          desc->m_destructor(std::bit_cast<Ptr<byte>>(ptr.get()));
    }

    unordered_map<ID, Ptr<Overrides>> m_events_map;
    PinnedVector<std::pair<UniquePtr<Overrides>, Ptr<const Event::Descriptor>>> m_events;
  };
}