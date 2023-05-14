/*****************************************************************
*\file         Pool.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::ECS::Pool
{
static constexpr u32 invalid_index = 0xffffffffu >> 1;

struct Instance final : NonCopyable
{
  /*
  * Constructor / Destructor
  */

  inline Instance() noexcept = default;
  inline ~Instance() noexcept;

  /*
  * Modifiers
  */

  inline void Initialise( DescriptorSpan, Family::Instance & ) noexcept;
  inline i32 Size() const noexcept;
  inline Index Append( u32 count ) noexcept;
  inline void Delete( Index index ) noexcept;
  inline void Clear() noexcept;
  inline Index MoveInFromPool( Instance &old_pool, Index old_index, Index new_index ) noexcept;

  /*
  * Component Accessor
  */

  inline i32 GetComponentIndex( const Component::Descriptor &desc ) const noexcept;

  inline i32 GetComponentIndex( const Component::Descriptor &desc, i32 &sequence ) const noexcept;

  template <Component::Concepts::IsComponent C>
  inline Raw<C> &GetComponent( Index index ) noexcept;

  template <Component::Concepts::IsComponent C>
  inline Raw<C> &GetComponent( Index index, i32 &sequence ) noexcept;

  inline Ptr<byte> GetComponent( Index index, const Component::Descriptor &desc ) noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline tuplet::tuple<Cs&...> GetComponents( Index index ) noexcept;

  inline vector<Ptr<byte>> GetComponents( Index index, const Signature &signature ) noexcept;

  /*
  * Structural Changes
  */

  inline void UpdateStructuralChanges( EntityManager &em ) noexcept;

  using ComponentVector = PinnedVector<Ptr<byte>>;
  ComponentVector m_components {};
  DescriptorSpan m_descriptors {};
  Ptr<Instance> m_pending_changes { nullptr };
  Ptr<Family::Instance> m_family { nullptr };
  i32 m_current {};
  i32 m_size {};
  u32 m_global_delete_index { invalid_index };
  u32 m_move_delete_index { invalid_index };

private:
  inline bool Free( Index index, bool destructor ) noexcept;
};
}