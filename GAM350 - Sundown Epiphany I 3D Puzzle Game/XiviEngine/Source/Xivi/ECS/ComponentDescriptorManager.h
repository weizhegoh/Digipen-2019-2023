/*****************************************************************
*\file         ComponentDescriptorManager.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::ECS::Component
{
struct DescriptorManager final : Singleton<DescriptorManager>
{
  using Descriptors = vector<Ptr<const Descriptor>>;
  using DescriptorMap = unordered_map<Component::ID, Ptr<const Descriptor> >;

  template <Concepts::IsComponent C>
  inline void Register() noexcept;

  inline void Lock() noexcept;
  inline bool IsLocked() const noexcept;
  inline Descriptors DescriptorVector( const Signature &signature ) const noexcept;

  inline u32 CountDataComponent( const Signature &signature ) const noexcept;
  inline u32 CountSharedComponent( const Signature &signature ) const noexcept;
  inline u32 CountTagComponent( const Signature &signature ) const noexcept;
  inline u32 CountExclusiveTagomponent( const Signature &signature ) const noexcept;
  inline Ptr<const Descriptor> Get( Component::ID id ) const noexcept;

  Descriptors m_descriptor_array {};
  DescriptorMap m_descriptor_map;
  Signature m_data_bits {};
  Signature m_shared_bits {};
  Signature m_tag_bits {};
  Signature m_exclusive_tag_bits {};
  u32 m_types_count { 0 };
  bool m_lock { false };

private:
  friend class Singleton<DescriptorManager>;
  DescriptorManager() = default;
  ~DescriptorManager() = default;
};
}