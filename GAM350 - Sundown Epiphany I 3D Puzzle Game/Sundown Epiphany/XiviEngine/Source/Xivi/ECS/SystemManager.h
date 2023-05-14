/*****************************************************************
*\file         SystemManager.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
namespace Xivi::ECS::System
{
struct Manager final : NonCopyable
{
  struct Events
  {
    Event::Instance<> m_on_start;
    Event::Instance<> m_on_frame_start;
    Event::Instance<> m_on_update;
    Event::Instance<> m_on_frame_end;
    Event::Instance<> m_on_end;
    Event::Instance<> m_on_destroy;
    Event::Instance<> m_on_pause;
    Event::Instance<> m_on_resume;
    Event::Instance<> m_on_sync;
  };

  inline ~Manager() noexcept;

  inline void Run() noexcept;

  template <Concepts::IsSystem TSystem>
  inline TSystem &Register( World::Instance &world ) noexcept;

  template <Concepts::IsSystem TSystem>
  inline TSystem &Get() noexcept;

  inline void OnNewArchetype( Archetype::Instance &archetype ) noexcept;

  using SystemDescAndInstance = std::pair<Ptr<const Descriptor>, UniquePtr<System::Instance>>;
  using Systems = PinnedVector<SystemDescAndInstance>;
  using SystemMap = unordered_map<ID, Ptr<System::Instance>>;

  SystemMap m_systems;
  Systems m_update_systems;
  Systems m_notifier_systems;
  Events m_events;
};
}