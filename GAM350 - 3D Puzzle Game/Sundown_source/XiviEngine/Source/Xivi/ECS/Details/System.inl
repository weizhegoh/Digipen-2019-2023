/*****************************************************************
*\file         System.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::System::Details
{
template <Concepts::IsSystem TSystem>
struct Linker final : TSystem
{
  TSystem::Events m_events;

  Linker( World::Instance &world ) noexcept : TSystem( world )
  {}

  inline void Run() noexcept
  {
    if constexpr ( TSystem::info.type == Type::ID::Update || Xivi::Traits::IsSpecialised< Type::ChildUpdate, Raw<decltype( TSystem::info )> >() )
    {
      XV_PROFILE_ZONESCOPED_N( TSystem::info.m_name );
      Service::Locator::Get<TimerEngine>().StartFrameRate();

      if constexpr ( &TSystem::OnPreUpdate != &Instance::OnPreUpdate ) TSystem::OnPreUpdate();

      if constexpr ( &TSystem::OnUpdate != &Instance::OnUpdate ) TSystem::OnUpdate();
      else
      {
        auto archetypes = TSystem::m_world.Search( info<TSystem>.m_query );
        TSystem::m_world.ForEach( archetypes, *this );
      }

      if constexpr ( &TSystem::OnPostUpdate != &Instance::OnPostUpdate ) TSystem::OnPostUpdate();
      {
        XV_PROFILE_ZONESCOPED_N( "Update Structural Changes" );
        TSystem::m_world.m_archetype_manager.UpdateStructuralChanges();
      }

      if constexpr ( &TSystem::OnPostStructuralChanges != &Instance::OnPostStructuralChanges ) TSystem::OnPostStructuralChanges();

      Service::Locator::Get<TimerEngine>().EndFrameRate( TSystem::info.m_id, TSystem::info.m_name );
    }
  }

  inline void Notify( Entity &entity ) noexcept
  {
    if constexpr ( TSystem::info.is_notifier )
    {
      if ( !info<TSystem>.m_query.Compare( TSystem::m_world.GetArchetype( entity ).GetSignature() ) ) return;

      XV_PROFILE_ZONESCOPED_N( TSystem::info.m_name );
      Service::Locator::Get<TimerEngine>().StartFrameRate();
      if constexpr ( &TSystem::OnNotify != &Instance::OnNotify ) TSystem::OnNotify( entity );
      else
      {
        auto &entry = TSystem::m_world.m_entity_manager.Get( entity );
        auto components = Archetype::Details::GetComponentPointerArray( *entry.m_pool, entry.m_pool_index, Traits::Tuple::null<typename Traits::Function::Type<Linker<TSystem>>::ArgsTuple > );
        Archetype::Details::Invoke( *this, components );
      }
      Service::Locator::Get<TimerEngine>().EndFrameRate( TSystem::info.m_id, TSystem::info.m_name );
    }
  }
};
}

namespace Xivi::ECS::System::Details
{
template <Concepts::IsSystem TSystem>
consteval Descriptor CreateDescriptor() noexcept
{
  Descriptor descriptor;

  using RealSystem = Details::Linker<TSystem>;

  if constexpr ( TSystem::info.m_id.Valid() ) descriptor.m_id = TSystem::info.m_id;
  else descriptor.m_id = ID( Typename<TSystem>() );

  if constexpr ( !TSystem::info.is_notifier ) descriptor.m_notifier_registration = nullptr;
  else
  {
    descriptor.m_notifier_registration = []( Archetype::Instance &archetype, System::Instance &system ) noexcept
    {
      if constexpr ( TSystem::info.type == Type::ID::NotifyCreate )
        archetype.m_events.m_on_entity_created.Register<&RealSystem::Notify>( static_cast<RealSystem &>( system ) );
      else if constexpr ( TSystem::info.type == Type::ID::NotifyDestroy )
        archetype.m_events.m_on_entity_destroyed.Register<&RealSystem::Notify>( static_cast<RealSystem &>( system ) );
      else if constexpr ( TSystem::info.type == Type::ID::NotifyMoveIn )
        archetype.m_events.m_on_entity_moved_in.Register<&RealSystem::Notify>( static_cast<RealSystem &>( system ) );
      else if constexpr ( TSystem::info.type == Type::ID::NotifyMoveOut )
        archetype.m_events.m_on_entity_moved_out.Register<&RealSystem::Notify>( static_cast<RealSystem &>( system ) );
    };
  }

  descriptor.m_destructor = []( System::Instance &system ) noexcept
  {
    std::destroy_at( &static_cast<Details::Linker<TSystem> &>( system ) );
  };

  if constexpr ( TSystem::info.m_name ) descriptor.m_name = Typename<TSystem>();
  else descriptor.m_name = TSystem::info.m_name;

  descriptor.m_type = TSystem::info.type;

  return descriptor;
}
}

namespace Xivi::ECS::System
{
/*
* Constructor / Destructor
*/

Instance::Instance( World::Instance &world ) noexcept : Overrides( world )
{}

/*
* Events API
*/

template <Xivi::Event::Concepts::IsEvent TEvent,
  Concepts::IsSystem TSystem,
  typename... Args>
  requires( ( std::is_same_v<typename TSystem::Events, Overrides::Events> == false ) && !!( Traits::Tuple::Index<TEvent, typename TSystem::Events> +1 ) )
  void Instance::SendEvent( TSystem *system, Args &&...args ) noexcept
{
  tuplet::get<Traits::Tuple::Index<TEvent, typename TSystem::Events>>( static_cast <Ptr< Details::Linker<TSystem> >>( system )->m_events ).Notify( std::forward<Args &&>( args )... );
}

template <Xivi::Event::Concepts::IsEvent TEvent>
TEvent &Instance::GetGlobalEvent() const noexcept
{
  return m_world.GetGlobalEvent<TEvent>();
}

template <Xivi::Event::Concepts::IsEvent TEvent, typename... Args>
void Instance::SendGlobalEvent( Args &&...args ) const noexcept
{
  m_world.SendGlobalEvent<TEvent, Args...>( std::forward<Args &&>( args )... );
}

/*
* Entities API
*/

bool Instance::IsEntityZombie( const Entity &entity ) const noexcept
{
  return m_world.IsEntityZombie( entity );
}

template <Component::Concepts::IsComponent C>
bool Instance::HaveComponent( const Entity &entity ) const noexcept
{
  return m_world.HaveComponent<C>( entity );
}

bool Instance::HaveComponent( const Entity &entity, Ptr<const Component::Descriptor>  descriptor ) const noexcept
{
  return m_world.HaveComponent( entity, descriptor );
}

bool Instance::HaveComponent( const Entity &entity, const Component::Descriptor &descriptor ) const noexcept
{
  return m_world.HaveComponent( entity, descriptor );
}

template <Component::Concepts::IsComponent... Cs>
bool Instance::HaveComponents( const Entity &entity ) const noexcept
{
  return m_world.HaveComponents<Cs...>( entity );
}

bool Instance::HaveComponents( const Entity &entity, DescriptorSpan descriptors ) const noexcept
{
  return m_world.HaveComponents( entity, descriptors );
}

template <Component::Concepts::IsComponent C>
C &Instance::GetComponent( const Entity &entity ) const noexcept
{
  return m_world.GetComponent<C>( entity );
}

Ptr<byte> Instance::GetComponent( const Entity &entity, Ptr<const Component::Descriptor> descriptor ) const noexcept
{
  return m_world.GetComponent( entity, descriptor );
}

Ptr<byte> Instance::GetComponent( const Entity &entity, const Component::Descriptor &descriptor ) const noexcept
{
  return m_world.GetComponent( entity, descriptor );
}

template <Component::Concepts::IsComponent... Cs>
tuplet::tuple<Cs&...> Instance::GetComponents( const Entity &entity ) const noexcept
{
  return m_world.GetComponents<Cs...>( entity );
}

vector<Ptr<byte>> Instance::GetComponents( const Entity &entity, DescriptorSpan descriptors ) const noexcept
{
  return m_world.GetComponents( entity, descriptors );
}

template <ECS::Concepts::SetterCallback Func>
bool Instance::GetEntity( const Entity &entity, Func &&func ) const noexcept
{
  return m_world.GetEntity( entity, std::forward<Func>( func ) );
}

void Instance::DestroyEntity( Entity &entity ) const noexcept
{
  return m_world.DestroyEntity( entity );
}

template <Xivi::ECS::Concepts::SetterCallback Func>
Entity Instance::AddOrRemoveComponents( Entity entity, DescriptorSpan add, DescriptorSpan remove, Func &&func ) const noexcept
{
  return m_world.AddOrRemoveComponents( entity, add, remove, func );
}

template <Xivi::ECS::Concepts::SetterCallback Func>
Entity Instance::AddOrRemoveComponents( Entity entity, const Signature &add, const Signature &remove, Func &&func ) const noexcept
{
  return m_world.AddOrRemoveComponents( entity, add, remove, func );
}

template <IsTypelist AddList, IsTypelist RemoveList, ECS::Concepts::SetterCallback Func>
Entity Instance::AddOrRemoveComponents( Entity entity, Func &&func ) const noexcept
{
  if constexpr ( std::is_same_v<Func, EmptyLambda> )
    return m_world.AddOrRemoveComponents<AddList, RemoveList>( entity );
  else
    return m_world.AddOrRemoveComponents<AddList, RemoveList>( entity, std::forward<Func &&>( func ) );
}

/*
* Singleton Entity API
*/
template <Component::Concepts::SingletonComponent C>
C &Instance::GetOrCreateSingletonComponent() const noexcept
{
  return m_world.GetOrCreateSingletonComponent<C>();
}

/*
* Archetypes API
*/

Archetype::Instance &Instance::GetOrCreateArchetype( DescriptorSpan desc, const std::string &str ) const noexcept
{
  return m_world.GetOrCreateArchetype( desc, str );
}

template <Component::Concepts::IsComponentList List>
Archetype::Instance &Instance::GetOrCreateArchetype( const std::string &str ) const noexcept
{
  return m_world.GetOrCreateArchetype<List>( str );
}

template <Component::Concepts::IsComponent... Cs>
Archetype::Instance &Instance::GetOrCreateArchetype( const std::string &str ) const noexcept
{
  return m_world.GetOrCreateArchetype<Cs...>( str );
}

Archetype::Instance &Instance::GetArchetype( Archetype::ID id ) const noexcept
{
  return m_world.GetArchetype( id );
}

Archetype::Instance &Instance::GetArchetype( const std::string &str ) const noexcept
{
  return m_world.GetArchetype( str );
}

Archetype::Instance &Instance::GetArchetype( const Entity &entity ) const noexcept
{
  return m_world.GetArchetype( entity );
}

/*
* System API
*/

template <Concepts::IsSystem TSystem>
TSystem &Instance::GetSystem() const noexcept
{
  return m_world.GetSystem<TSystem>();
}

PinnedVector<Ptr<Archetype::Instance>> Instance::Search( const Query::Instance &query ) const noexcept
{
  return m_world.Search( query );
}

template <Component::Concepts::SharedComponent C>
const Ptr<SharedFilter> Instance::FindSharedFilter( C &&shared_component, Archetype::ID id ) const noexcept
{
  static_assert( Component::info<C>.m_build_filter );
  XV_CORE_ASSERT_MSG( Component::info<C>.m_global_scoped ? id.m_value == 0 : id.Valid(), "Invalid ID" );

  return FindSharedFilter(
    Component::GenerateSharedKey( id, Component::info<C>, std::bit_cast<byte *>( &shared_component ) ) );
}

const Ptr<SharedFilter> Instance::FindSharedFilter( Component::Type::Shared::Key key ) const noexcept
{
  auto shared_map = m_world.m_archetype_manager.m_shared_component_map;
  auto it = shared_map.find( key );
  if ( it != shared_map.end() )
  {
    auto &details = m_world.m_entity_manager.Get( it->second );
    return &details.m_pool->GetComponent<SharedFilter>( details.m_pool_index );
  }
  return nullptr;
}

template <ECS::Concepts::SystemCallback Func>
bool Instance::ForEach( Func &&func ) const noexcept
{
  return m_world.ForEach<Func>( std::forward<Func &&>( func ) );
}

template <ECS::Concepts::SystemCallback Func>
bool Instance::ForEach( Query::Instance query, Func &&func ) const noexcept
{
  return m_world.ForEach<Func>( query, std::forward<Func &&>( func ) );
}

template <ECS::Concepts::SystemCallback Func>
bool Instance::ForEach( span<const Ptr<Archetype::Instance>> list, Func &&func ) const noexcept
{
  return m_world.ForEach<Func>( list, std::forward<Func &&>( func ) );
}

template <ECS::Concepts::SystemCallback Func>
bool Instance::ForEach( const SharedFilter &shared_filter, const Query::Instance &query, Func &&func ) const noexcept
{
  Query::Iterator<Func> iterator( m_world );

  for ( const auto &entry : shared_filter.m_entries )
  {
    if ( !query.Compare( entry.m_archetype->GetSignature(), entry.m_archetype->GetExclusiveSignature() ) ) continue;

    iterator.UpdateArchetype( *entry.m_archetype );

    for ( auto family : entry.m_families )
    {
      iterator.UpdateFamily( *family );
      iterator.UpdatePool( family->m_pool );
      if constexpr ( Traits::Concepts::FuncReturnType<Func, bool> )
      {
        if ( iterator.ForEach( std::forward<Func &&>( func ) ) ) return true;
      }
      else iterator.ForEach( std::forward<Func &&>( func ) );
    }
  }
  return false;
}
}