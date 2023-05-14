/*****************************************************************
*\file         Family.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::ECS::Pool::Family
{
inline ID GenerateID( const Archetype::ID id, DescriptorSpan descriptors, ConstDataSpan datas ) noexcept;

struct Instance final : NonCopyable
{
  struct SharedDetails
  {
    Entity m_entity;
    Component::Type::Shared::Key m_key;
  };

  using SharedDetailsArray = array<SharedDetails, Settings::max_shared_components_per_entity>;

  inline void Initialise( ID id,
                          Archetype::Instance &archetype,
                          DescriptorSpan data_descriptors,
                          DescriptorSpan shared_descriptors,
                          SharedKeySpan shared_keys,
                          EntitySpan shared_entities ) noexcept;

  template <Traits::Concepts::InvokableFunction Func>
  void Append( u32 count, Archetype::Manager &archetype_manager, Func &&func ) noexcept;

  inline void MoveIn( World::Instance &m_world, Instance &src_family, Pool::Instance &src_pool, Pool::Index src_index ) noexcept;

  Pool::Instance m_pool;
  SharedDetailsArray m_shared_details;
  DescriptorSpan m_shared_descriptors;
  UniquePtr<Family::Instance> m_next;
  Ptr<Archetype::Instance> m_archetype;
  Ptr<Family::Instance> m_prev;
  ID m_id;
};
}