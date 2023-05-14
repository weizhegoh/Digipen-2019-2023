/*****************************************************************
*\file         SystemManager.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::System::Details
{
inline std::string_view Typename( System::Type::ID id )
{
  switch ( id )
  {
    case System::Type::ID::Update:
    return "Update";
    case System::Type::ID::GlobalEvent:
    return "Global Event";
    case  System::Type::ID::SystemEvent:
    return "System Event";
    case  System::Type::ID::NotifyCreate:
    return "Create Notification";
    case  System::Type::ID::NotifyDestroy:
    return "Destroy Notification";
    case  System::Type::ID::NotifyMoveIn:
    return "MoveIn Notification";
    case  System::Type::ID::NotifyMoveOut:
    return "MoveOut Notification";
    default:
    break;
  }
  XV_CORE_ASSERT_MSG( false, "[ComponentDescriptorManager] Invalid Component ID" );
  return "";
}
}

namespace Xivi::ECS::System
{
Manager::~Manager() noexcept
{
  while ( m_update_systems.size() )
  {
    auto ptr = m_update_systems.back().second.release();
    m_update_systems.back().first->m_destructor( *ptr );
    delete std::bit_cast<void *>( ptr );
    m_update_systems.pop_back();
  }

  while ( m_notifier_systems.size() )
  {
    auto ptr = m_notifier_systems.back().second.release();
    m_notifier_systems.back().first->m_destructor( *ptr );
    delete std::bit_cast<void *>( ptr );
    m_notifier_systems.pop_back();
  }
}

void Manager::Run() noexcept
{
  m_events.m_on_frame_start.Notify();
  m_events.m_on_update.Notify();
  m_events.m_on_frame_end.Notify();
}

template <Concepts::IsSystem TSystem>
TSystem &Manager::Register( World::Instance &world ) noexcept
{
  using RealSystem = Details::Linker<TSystem>;
  using Info = Raw<decltype( TSystem::info )>;

  auto &system = *static_cast<Ptr<RealSystem>>( [&]()
  {
    if constexpr ( RealSystem::info.is_notifier )
      return m_notifier_systems.emplace_back( std::pair { &info<TSystem>, MakeUnique<RealSystem>( world ) } ).second.get();
    else
      return m_update_systems.emplace_back( std::pair { &info<TSystem>, MakeUnique<RealSystem>( world ) } ).second.get();
  }( ) );

  m_systems.emplace( std::pair < ID, Ptr<System::Instance>>{info<TSystem>.m_id, static_cast<Ptr<Instance>>( &system )} );

  if constexpr ( &TSystem::OnCreate != &Overrides::OnCreate )
    system.OnCreate();

  if constexpr ( RealSystem::info.type != Type::ID::GlobalEvent )
  {
    Query::Instance query;
    query.Set<TSystem::All, TSystem::Any, TSystem::None>();
    if constexpr ( Traits::Concepts::InvokableFunction<TSystem> )
      query.SetQueryFromFunction<TSystem>();
    info<TSystem>.m_query = query;
  }

  if constexpr ( RealSystem::info.type == Type::ID::Update )
    m_events.m_on_update.Register<&RealSystem::Run>( system );

  if constexpr ( RealSystem::info.type == Type::ID::GlobalEvent )
    world.m_event_manager.Get<Info::EventType>().Register<&TSystem::OnEvent>( world.GetSystem<TSystem>() );

  if constexpr ( RealSystem::info.type == Type::ID::SystemEvent )
  {
    if constexpr ( Xivi::Traits::IsSpecialised<System::Type::ChildUpdate, Info>() )
    {
      tuplet::get<Traits::Tuple::Index<Info::EventType, Traits::Tuple::Raw<Info::SystemType::Events>>>(
        std::bit_cast<Ptr < Details::Linker<Info::SystemType> >>( &Get<Info::SystemType>() )->m_events )
        .Register<&RealSystem::Run>( system );
    }
    else
    {
      tuplet::get<Traits::Tuple::Index<Info::EventType, Traits::Tuple::Raw<Info::SystemType::Events>>>(
        std::bit_cast <Ptr < Details::Linker<Info::SystemType > >>( &Get<Info::SystemType>() )->m_events )
        .Register<&RealSystem::OnEvent>( system );
    }
  }

  if constexpr ( &RealSystem::OnStart != &System::Overrides::OnStart )
    m_events.m_on_start.Register<&RealSystem::OnStart>( system );

  if constexpr ( &RealSystem::OnFrameStart != &System::Overrides::OnFrameStart )
    m_events.m_on_frame_start.Register<&RealSystem::OnFrameStart>( system );

  if constexpr ( &RealSystem::OnFrameEnd != &System::Overrides::OnFrameEnd )
    m_events.m_on_frame_end.Register<&RealSystem::OnFrameEnd>( system );

  if constexpr ( &RealSystem::OnEnd != &System::Overrides::OnEnd )
    m_events.m_on_end.Register<&RealSystem::OnEnd>( system );

  if constexpr ( &RealSystem::OnDestroy != &System::Overrides::OnDestroy )
    m_events.m_on_destroy.Register<&RealSystem::OnDestroy>( system );

  if constexpr ( &RealSystem::OnPause != &System::Overrides::OnPause )
    m_events.m_on_pause.Register<&RealSystem::OnPause>( system );
  else
    m_events.m_on_pause.Register<&System::Overrides::OnPause>( system );

  if constexpr ( &RealSystem::OnResume != &System::Overrides::OnResume )
    m_events.m_on_resume.Register<&RealSystem::OnResume>( system );
  else
    m_events.m_on_resume.Register<&System::Overrides::OnResume>( system );

  return system;
}

template <Concepts::IsSystem TSystem>
TSystem &Manager::Get() noexcept
{
  auto it = m_systems.find( info<TSystem>.m_id );
  XV_CORE_ASSERT_MSG( it != m_systems.end(), "[SystemManager] Could not find system " );
  return *static_cast<TSystem *>( it->second );
}

void Manager::OnNewArchetype( Archetype::Instance &archetype ) noexcept
{
  for ( auto &system : m_notifier_systems )
  {
    auto &entry = *system.second;
    if ( system.first->m_query.Compare( archetype.m_signature ) )
      system.first->m_notifier_registration( archetype, entry );
  }
}
}