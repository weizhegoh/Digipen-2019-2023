/*****************************************************************
*\file         System.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::System::Type
{
enum class ID : u8
{
  Update,
  NotifyCreate,
  NotifyDestroy,
  NotifyMoveIn,
  NotifyMoveOut,
  GlobalEvent,
  SystemEvent
};

struct Update
{
  static constexpr auto type = ID::Update;
  static constexpr auto is_notifier = false;
  System::ID m_id;
  const char *m_name;
};

template <System::Concepts::IsSystem TSystem, Xivi::Event::Concepts::IsEvent TEvent>
struct ChildUpdate
{
  using SystemType = TSystem;
  using EventType = TEvent;
  static constexpr auto type = ID::SystemEvent;
  static constexpr auto is_notifier = false;
  System::ID m_id;
  const char *m_name;
};

template <System::Concepts::IsSystem TSystem, Xivi::Event::Concepts::IsEvent TEvent>
struct SystemEvent
{
  using SystemType = TSystem;
  using EventType = TEvent;
  static constexpr auto type = ID::SystemEvent;
  static constexpr auto is_notifier = false;
  System::ID m_id;
  const char *m_name;
};

struct NotifyCreate
{
  static constexpr auto type = ID::NotifyCreate;
  static constexpr auto is_notifier = true;
  System::ID m_id;
  const char *m_name;
};

struct NotifyDestroy
{
  static constexpr auto type = ID::NotifyDestroy;
  static constexpr auto is_notifier = true;
  System::ID m_id;
  const char *m_name;
};

struct NotifyMoveIn
{
  static constexpr auto type = ID::NotifyMoveIn;
  static constexpr auto is_notifier = true;
  System::ID m_id;
  const char *m_name;
};

struct NotifyMoveOut
{
  static constexpr auto type = ID::NotifyMoveOut;
  static constexpr auto is_notifier = true;
  System::ID m_id;
  const char *m_name;
};

template <Xivi::Event::Concepts::IsEvent TEvent>
struct GlobalEvent
{
  using EventType = TEvent;
  static constexpr auto type = ID::GlobalEvent;
  static constexpr auto is_notifier = false;
  System::ID m_id;
  const char *m_name;
};
}

namespace Xivi::ECS::System
{
struct Descriptor
{
  using NotifierRegistration = FuncPtr<void( Archetype::Instance &, System::Instance & ) noexcept>;
  using Destructor = FuncPtr<void( System::Instance & ) noexcept>;

  ID m_id;
  mutable Query::Instance m_query;
  NotifierRegistration m_notifier_registration;
  Destructor m_destructor;
  Type::ID m_type;
  std::string_view m_name;
};

namespace Details
{
template <Concepts::IsSystem TSystem>
consteval Descriptor CreateDescriptor() noexcept;

template <typename TSystem>
inline constexpr auto desc = CreateDescriptor<TSystem>();
}

template <typename TSystem>
inline constexpr auto &info = Details::desc<Raw<TSystem>>;

struct Overrides
{
  static constexpr auto info = Type::Update {};
  using All = Query::All<>;
  using Any = Query::Any<>;
  using None = Query::None<>;

  using Events = tuplet::tuple<>;

  inline Overrides( World::Instance &world ) noexcept : m_world( world )
  {}

  void OnCreate() noexcept
  {}
  void OnStart() noexcept
  {}
  void OnFrameStart() noexcept
  {}
  void OnPreUpdate() noexcept
  {}
  void OnUpdate() noexcept
  {}
  void OnPostUpdate() noexcept
  {}
  void OnPostStructuralChanges() noexcept
  {}
  void OnFrameEnd() noexcept
  {}
  void OnEnd() noexcept
  {}
  void OnDestroy() noexcept
  {}
  void OnPause() noexcept
  {
    m_paused = true;
  }
  void OnResume() noexcept
  {
    m_paused = false;
  }
  void OnEvent( ... ) noexcept
  {}
  void OnNotify( Entity & ) noexcept
  {}

  World::Instance &m_world;
  bool m_paused { true };
};

struct Instance : Overrides, NonCopyable
{
  /*
  * Constructor / Destructor
  */

  inline Instance( World::Instance &world ) noexcept;

  /*
  * Events API
  */

  template <Xivi::Event::Concepts::IsEvent TEvent,
    Concepts::IsSystem TSystem,
    typename... Args>
    requires( ( std::is_same_v<typename TSystem::Events, Overrides::Events> == false ) && !!( Traits::Tuple::Index<TEvent, typename TSystem::Events> +1 ) )
    inline static void SendEvent( TSystem *system, Args &&...args ) noexcept;

  template <Xivi::Event::Concepts::IsEvent TEvent>
  inline TEvent &GetGlobalEvent() const noexcept;

  template <Xivi::Event::Concepts::IsEvent TEvent, typename... Args>
  inline void SendGlobalEvent( Args &&...args ) const noexcept;

  /*
  * Entities API
  */
  inline bool IsEntityZombie( const Entity &entity ) const noexcept;

  template <Component::Concepts::IsComponent C>
  inline bool HaveComponent( const Entity &entity ) const noexcept;
  inline bool HaveComponent( const Entity &entity, Ptr<const Component::Descriptor> descriptors ) const noexcept;
  inline bool HaveComponent( const Entity &entity, const Component::Descriptor &descriptor ) const noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline bool HaveComponents( const Entity &entity ) const noexcept;
  inline bool HaveComponents( const Entity &entity, DescriptorSpan descriptors ) const noexcept;

  template <Component::Concepts::IsComponent C>
  inline C &GetComponent( const Entity &entity ) const noexcept;
  inline Ptr<byte> GetComponent( const Entity &entity, Ptr<const Component::Descriptor> descriptor ) const noexcept;
  inline Ptr<byte> GetComponent( const Entity &entity, const Component::Descriptor &descriptor ) const noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline tuplet::tuple<Cs&...> GetComponents( const Entity &entity ) const noexcept;
  inline vector<Ptr<byte>> GetComponents( const Entity &entity, DescriptorSpan descriptors ) const noexcept;

  template <ECS::Concepts::SetterCallback Func = EmptyLambda>
  inline bool GetEntity( const Entity &entity, Func &&func = EmptyLambda {} ) const noexcept;
  inline void DestroyEntity( Entity &entity ) const noexcept;

  template <Xivi::ECS::Concepts::SetterCallback Func = EmptyLambda>
  inline Entity AddOrRemoveComponents( Entity entity, DescriptorSpan add, DescriptorSpan remove = {}, Func &&func = EmptyLambda {} ) const noexcept;

  template <Xivi::ECS::Concepts::SetterCallback Func = EmptyLambda>
  inline Entity AddOrRemoveComponents( Entity entity, const Signature &add, const Signature &remove = {}, Func &&func = EmptyLambda {} ) const noexcept;

  template <IsTypelist AddList, IsTypelist RemoveList = Typelist<>, ECS::Concepts::SetterCallback Func = EmptyLambda>
  inline Entity AddOrRemoveComponents( Entity entity, Func &&func = EmptyLambda {} ) const noexcept;

  /*
  * Singleton Entity API
  */
  template <Component::Concepts::SingletonComponent C>
  inline C &GetOrCreateSingletonComponent() const noexcept;

  /*
  * Archetypes API
  */

  inline Archetype::Instance &GetOrCreateArchetype( DescriptorSpan desc, const std::string &str = {} ) const noexcept;

  template <Component::Concepts::IsComponentList List>
  inline Archetype::Instance &GetOrCreateArchetype( const std::string &str = {} ) const noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline Archetype::Instance &GetOrCreateArchetype( const std::string &str = {} ) const noexcept;

  inline Archetype::Instance &GetArchetype( Archetype::ID id ) const noexcept;
  inline Archetype::Instance &GetArchetype( const std::string &str ) const noexcept;
  inline Archetype::Instance &GetArchetype( const Entity &entity ) const noexcept;

  /*
  * System API
  */

  template <Concepts::IsSystem TSystem>
  inline TSystem &GetSystem() const noexcept;

  inline PinnedVector<Ptr<Archetype::Instance>> Search( const Query::Instance &query ) const noexcept;

  template <Component::Concepts::SharedComponent C>
  inline const Ptr<SharedFilter> FindSharedFilter( C &&shared_component, Archetype::ID id = Archetype::ID {} ) const noexcept;

  inline const Ptr<SharedFilter> FindSharedFilter( Component::Type::Shared::Key key ) const noexcept;

  template <ECS::Concepts::SystemCallback Func>
  inline bool ForEach( Func &&func ) const noexcept;

  template <ECS::Concepts::SystemCallback Func>
  inline bool ForEach( Query::Instance query, Func &&func ) const noexcept;

  template <ECS::Concepts::SystemCallback Func>
  inline bool ForEach( span<const Ptr<Archetype::Instance>> list, Func &&func ) const noexcept;

  template <ECS::Concepts::SystemCallback Func>
  inline bool ForEach( const SharedFilter &shared_filter, const Query::Instance &query, Func &&func ) const noexcept;

  template <Concepts::IsSystem TSystem>
  friend struct Details::Linker;
};

namespace Concepts
{
template <typename List>
concept IsSystemList = IsTypelist<List> &&
[]<IsSystem... TSystems>( Typelist<TSystems...> ) constexpr noexcept
{
  return true;
}( List() );
}
}