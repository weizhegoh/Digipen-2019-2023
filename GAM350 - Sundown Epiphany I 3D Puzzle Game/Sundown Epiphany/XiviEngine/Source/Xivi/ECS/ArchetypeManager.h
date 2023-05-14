/*****************************************************************
*\file         ArchetypeManager.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
namespace Xivi::ECS::Archetype
{
struct Manager final : NonCopyable
{
  struct Events
  {
    Event::Instance<Archetype::Instance &> m_on_new_archetype;
  };

  /*
  * Constructor / Destructor
  */

  Manager() = delete;
  inline Manager( World::Instance &world ) noexcept;

  /*
  * Archetypes
  */

  inline Archetype::Instance &Get( Archetype::ID id ) noexcept;
  inline Archetype::Instance &Get( const std::string &str ) noexcept;
  inline Archetype::Instance &Create( Archetype::ID id, const Signature &signature, const std::string &str = {} ) noexcept;
  inline Archetype::Instance &GetOrCreate( const Signature &signature, const std::string &str = {} ) noexcept;

  /*
  * Structural Changes
  */

  inline void AddToStructuralPendingList( Archetype::Instance &archetype ) noexcept;
  inline void AddToStructuralPendingList( Pool::Instance &pool ) noexcept;
  inline void UpdateStructuralChanges() noexcept;

  /*
  * Entities API
  */

  template <Xivi::ECS::Concepts::SetterCallback Func = EmptyLambda>
  inline Entity AddOrRemoveComponents( Entity entity, const Signature &add, const Signature &remove, Func &&func = EmptyLambda {} ) noexcept;

  using FamilyMap = unordered_map<Pool::Family::ID, Ptr<Pool::Family::Instance>>;
  using ArchetypeMap = unordered_map<Archetype::ID, Ptr<Archetype::Instance>>;
  using SharedComponentMap = unordered_map<Component::Type::Shared::Key, Entity>;
  using SignatureAndExclusive = std::pair<Signature, Signature>;
  using ArchetypeList = PinnedVector<UniquePtr<Archetype::Instance>>;
  using ArchetypeSignatureList = PinnedVector<SignatureAndExclusive>;
  static constexpr auto end_structural_changes = ~static_cast<u64>( 0 );

  World::Instance &m_world;
  Events m_events;
  SharedComponentMap m_shared_component_map;
  ArchetypeMap m_archetype_map;
  FamilyMap m_family_map;
  ArchetypeList m_archetypes;
  ArchetypeSignatureList m_archetypes_bits;
  Ptr<Archetype::Instance> m_archetype_structural_pending { std::bit_cast<Ptr<Instance>>( end_structural_changes ) };
  Ptr<Pool::Instance> m_pool_structural_pending { std::bit_cast<Ptr<Pool::Instance>>( end_structural_changes ) };
};
}