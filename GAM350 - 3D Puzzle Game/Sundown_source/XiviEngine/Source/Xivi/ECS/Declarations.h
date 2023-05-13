/*****************************************************************
*\file         Declarations.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::Component
{
struct ComponentID
{};

using ID = GUID<struct ComponentID>;
struct Descriptor;
struct DescriptorManager;
namespace Type
{
struct Tag;
struct Data;
struct Shared;
}

namespace Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Component::ID>( "Component GUID" )
  .property( "Value", &ID::m_value );
}
}
}

namespace Xivi::ECS::World
{
struct Instance;
}

namespace Xivi::ECS::System
{
using ID = GUID<struct SystemID>;
struct Instance;
struct Manager;

namespace Concepts
{
template <typename TSystem>
concept IsSystem = std::derived_from<TSystem, Instance>;
}

namespace Details
{
template <Concepts::IsSystem TSystem>
struct Linker;
}
}

namespace Xivi::ECS::Archetype
{
using ID = GUID<struct ArchetypeID>;
struct Instance;
struct Manager;
}

namespace Xivi::ECS::Pool
{
using ID = GUID<struct PoolID>;
struct Instance;
struct Index
{
  i32 m_value { 0 };
  auto operator<=>( const Index & ) const = default;
};
}

namespace Xivi::ECS::Pool::Family
{
using ID = GUID<struct FamilyID>;
struct Instance;
}

namespace Xivi::ECS::Event
{
  /*using ID = GUID<struct EventID>;
  template <typename... Args>
  struct Instance;
  struct Overrides;*/
using ID = Xivi::Event::ID;
using Global = Xivi::Event::Type::Global;
using Overrides = Xivi::Event::Overrides;
template <typename... Args>
using Instance = Xivi::Event::Instance<Args...>;
using Descriptor = Xivi::Event::Descriptor;
using Manager = Xivi::Event::Manager;
}

namespace Xivi::ECS::Query
{
struct Instance;
}

namespace Xivi::ECS
{
struct Entity;
struct ReferenceCount;
struct SharedFilter;

using Signature = Xivi::CBitset<Settings::max_components>;
using DescriptorSpan = span<const Ptr<const Component::Descriptor>>;
using DataSpan = span<Ptr<byte>>;
using ConstDataSpan = span<const Ptr<const byte>>;
using EntitySpan = span<Entity>;

inline i32 GetComponentIndexFromSignature( const Signature &signature, i32 index ) noexcept
{
  const i32 y = index % 64;
  i32 x = index / 64;
  i32 count = std::popcount( signature.data()[x] & ( ( 1ull << y ) - 1 ) );
  while ( x )
    count += std::popcount( signature.data()[--x] );
  return count;
}
}