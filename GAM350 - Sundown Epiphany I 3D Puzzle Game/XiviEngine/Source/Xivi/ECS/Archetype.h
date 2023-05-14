/*****************************************************************
*\file         Archetype.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::Archetype
{
inline ID GenerateID( Signature signature ) noexcept;

struct Instance final : NonCopyable
{
  struct Events
  {
    Event::Instance<Entity &> m_on_entity_created;
    Event::Instance<Entity &> m_on_entity_destroyed;
    Event::Instance<Entity &> m_on_entity_moved_in;
    Event::Instance<Entity &> m_on_entity_moved_out;
  };

  /*
  * Constructor / Destructor
  */

  inline Instance( Manager &manager ) noexcept;

  inline ~Instance();

  /*
  * Initialise
  */

  inline void Initialise( ID id, const Signature &signature, const std::string &str = {} ) noexcept;

  template <ECS::Concepts::SetterCallback Func>
  inline void SetConstructor( Func &&func ) noexcept;

  /*
  * ID / Signature
  */

  inline ID GetID() const noexcept;
  inline const Signature &GetSignature() const noexcept;
  inline const Signature &GetExclusiveSignature() const noexcept;

  template <Component::Concepts::IsComponent C>
  inline bool HaveComponent() const noexcept;
  inline bool HaveComponent( const Component::Descriptor &descriptor ) const noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline bool HaveAllComponents() const noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline bool HaveAllComponents( tuplet::tuple<Cs...> * ) const noexcept;
  inline bool HaveAllComponents( DescriptorSpan descriptors ) const noexcept;

  /*
  * Get SharedComponent
  */

  template <Component::Concepts::SharedComponent C>
  inline const C &GetSharedComponent( const Pool::Family::Instance &family ) const noexcept;

  /*
  * Families
  */

  inline Pool::Family::Instance &DefaultFamily() noexcept;
  inline Pool::Family::Instance &GetOrCreateFamily( DescriptorSpan descriptors, DataSpan datas ) noexcept;
  inline Pool::Family::Instance &GetOrCreateFamilyFromSameArchetype( Pool::Family::Instance &family, Signature signature, DataSpan datas, SharedKeySpan keys ) noexcept;
  inline Pool::Family::Instance &GetOrCreateFamilyFromDifferentArchetype( Entity entity ) noexcept;
  inline Pool::Family::Instance &CreateFamily( Pool::Family::ID id, EntitySpan entities, SharedKeySpan keys ) noexcept;

  /*
  * Structural Changes
  */

  inline void AddFamilyToPending( UniquePtr<Pool::Family::Instance> &&family ) noexcept;
  inline void UpdateStructuralChanges() noexcept;

  /*
  * Entities API
  */

  inline Entity CreateEntity( Pool::Family::Instance &family, DescriptorSpan descriptors, DataSpan datas ) noexcept;
  inline Entity CreateEntity( DescriptorSpan descriptors, DataSpan datas ) noexcept;

  template <ECS::Concepts::SetterCallback Func = EmptyLambda>
  inline Entity CreateEntity( Pool::Family::Instance &family, Func &&func = EmptyLambda {} ) noexcept;
  template <ECS::Concepts::SetterCallback Func = EmptyLambda>
  inline Entity CreateEntity( Func &&func = EmptyLambda {} ) noexcept;

  template <ECS::Concepts::SetterCallback Func = EmptyLambda>
  inline Entity CreateEntities( u32 count, Func &&func = EmptyLambda {} ) noexcept;

  inline void DestroyEntity( Entity &entity ) noexcept;
private:

  template <typename Func = EmptyLambda>
  inline void _CreateEntity( Pool::Family::Instance &family, u32 count, Func &&func = EmptyLambda {} ) noexcept;

  template <ECS::Concepts::SetterCallback Func = EmptyLambda>
  requires( false == ECS::Concepts::HasSharedParameters<Func> )
    inline Entity _CreateEntities( i32 count, Func &&func = EmptyLambda {} ) noexcept;

  template <ECS::Concepts::SetterCallback Func = EmptyLambda>
  requires( false == ECS::Concepts::HasSharedParameters<Func> )
    inline Entity _CreateEntities( i32 count, Pool::Family::Instance &family, Func &&func = EmptyLambda {} ) noexcept;

  template <ECS::Concepts::SetterCallback Func = EmptyLambda>
  requires( true == ECS::Concepts::HasSharedParameters<Func> )
    inline Entity _CreateEntities( i32 count, Func &&func = EmptyLambda {} )noexcept;

  template <ECS::Concepts::SetterCallback Func = EmptyLambda>
  inline Entity MoveEntity( Entity &entity, Pool::Family::Instance &family, Func &&func = EmptyLambda {} ) noexcept;

public:

  using SharedArchetypes = PinnedVector<Ptr<Instance>>;
  using ArchetypeDescriptors = vector<Ptr<const Component::Descriptor>>;

  SharedArchetypes m_shared_archetypes;
  Pool::Family::Instance m_default_family;
  Signature m_signature;
  Signature m_exclusive_signature;
  Events m_events;

  ArchetypeDescriptors m_descriptors;
  UniquePtr<Pool::Family::Instance> m_family_pending;
  Ptr<Instance> m_pending_structural_changes;
  ID m_id;
  std::string m_name;
  Manager &m_manager;
  Action< Entity > m_constructor;

  u32 m_data_component_count;
  u32 m_shared_component_count;
  u32 m_tag_component_count;
  u32 m_exclusive_tag_component_count;

  friend struct Manager;
  friend struct Pool::Family::Instance;
  friend struct System::Manager;
  friend struct World::Instance;
};
}