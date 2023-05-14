/*****************************************************************
*\file         World.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::ECS::World
{
struct Instance final : NonCopyable
{
  /*
  * Constructor / Destructor
  */

  inline Instance() noexcept;

  inline ~Instance() noexcept;

  /*
  * Runtime API
  */
  inline void Initialise() noexcept;

  inline void Start() noexcept;

  inline void Run() noexcept;

  inline void Pause() noexcept;

  inline void Resume() noexcept;

  inline void Stop() noexcept;

  inline void Clear() noexcept;

  /*
  * Components API
  */

  template <Component::Concepts::IsComponentList List>
  inline void RegisterComponents() noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline void RegisterComponents() noexcept;

  inline void LockComponents() noexcept;

  /*
  * Systems API
  */

  template <System::Concepts::IsSystemList List>
  inline void RegisterSystems() noexcept;

  template <System::Concepts::IsSystem... TSystems>
  inline void RegisterSystems() noexcept;

  template <System::Concepts::IsSystem TSystem>
  inline TSystem &GetSystem() noexcept;

  /*
  * Events API
  */

  template <Xivi::Event::Concepts::IsEventList List>
  inline void RegisterGlobalEvents() noexcept;

  template <Xivi::Event::Concepts::IsEvent... TEvents>
  inline void RegisterGlobalEvents() noexcept;

  template <Xivi::Event::Concepts::IsEvent TEvent>
  inline TEvent &GetGlobalEvent() noexcept;

  template <Xivi::Event::Concepts::IsEvent TEvent, typename... Args>
  inline void SendGlobalEvent( Args &&...args ) noexcept;

  /*
  * Entities API
  */
  inline bool IsEntityZombie( const Entity &entity ) const noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline bool HaveComponents( const Entity &entity ) const noexcept;
  inline bool HaveComponents( const Entity &entity, DescriptorSpan descriptors ) const noexcept;

  template <Component::Concepts::IsComponent C>
  inline bool HaveComponent( const Entity &entity ) const noexcept;
  inline bool HaveComponent( const Entity &entity, Ptr<const Component::Descriptor> descriptor ) const noexcept;
  inline bool HaveComponent( const Entity &entity, const Component::Descriptor &descriptor ) const noexcept;

  template <Component::Concepts::IsComponent C>
  inline C &GetComponent( const Entity &entity ) noexcept;
  inline Ptr<byte> GetComponent( const Entity &entity, Ptr<const Component::Descriptor> descriptor )noexcept;
  inline Ptr<byte> GetComponent( const Entity &entity, const Component::Descriptor &descriptor )noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline tuplet::tuple<Cs&...> GetComponents( const Entity &entity ) noexcept;
  inline vector<Ptr<byte>> GetComponents( const Entity &entity, DescriptorSpan descriptors ) noexcept;

  template <ECS::Concepts::SetterCallback Func> // TODO: Disallow SharedComponent
  inline bool GetEntity( const Entity &entity, Func &&func ) noexcept;

  inline void DestroyEntity( Entity &entity ) noexcept;

  template <Xivi::ECS::Concepts::SetterCallback Func = EmptyLambda>
  inline Entity AddOrRemoveComponents( Entity entity, DescriptorSpan add, DescriptorSpan remove = {}, Func &&func = EmptyLambda {} ) noexcept;

  template <Xivi::ECS::Concepts::SetterCallback Func = EmptyLambda>
  inline Entity AddOrRemoveComponents( Entity entity, const Signature &add, const Signature &remove = {}, Func &&func = EmptyLambda {} ) noexcept;

  template <IsTypelist AddList, IsTypelist RemoveList = Typelist<>, Xivi::ECS::Concepts::SetterCallback Func = EmptyLambda>
  inline Entity AddOrRemoveComponents( Entity entity, Func &&func = EmptyLambda {} ) noexcept;

  /*
  * Singleton Entity API
  */
  template <Component::Concepts::SingletonComponent C>
  inline C &GetOrCreateSingletonComponent() noexcept;

  /*
  * Archetypes API
  */

  inline Archetype::Instance &GetOrCreateArchetype( DescriptorSpan desc, const std::string &str = {} ) noexcept;

  template <Component::Concepts::IsComponentList List>
  inline Archetype::Instance &GetOrCreateArchetype( const std::string &str = {} ) noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline Archetype::Instance &GetOrCreateArchetype( const std::string &str = {} ) noexcept;

  inline Archetype::Instance &GetArchetype( Archetype::ID id ) noexcept;

  inline Archetype::Instance &GetArchetype( const std::string &str ) noexcept;

  inline Archetype::Instance &GetArchetype( const Entity &entity ) const noexcept;

  /*
  * Systems API
  */

  inline PinnedVector<Ptr<Archetype::Instance>> Search( const Query::Instance &query ) const noexcept;

  template <ECS::Concepts::SystemCallback Func>
  inline bool ForEach( Func &&func ) noexcept;

  template <ECS::Concepts::SystemCallback Func>
  inline bool ForEach( Query::Instance query, Func &&func ) noexcept;

  template <ECS::Concepts::SystemCallback Func>
  inline bool ForEach( span<const Ptr<Archetype::Instance>> archetypes, Func &&func ) noexcept;

  /*
  * Serialisation / Deserialisation API
  */

  inline void SerialiseWorld( const std::string &filename );

  inline void DeserialiseWorld( const std::string &filename );

  System::Manager m_system_manager;
  Event::Manager m_event_manager;
  EntityManager m_entity_manager;
  Archetype::Manager m_archetype_manager;
  unordered_map<Component::ID, UniquePtr<byte[]>> m_singleton_map;
  bool m_running = false;
};
}