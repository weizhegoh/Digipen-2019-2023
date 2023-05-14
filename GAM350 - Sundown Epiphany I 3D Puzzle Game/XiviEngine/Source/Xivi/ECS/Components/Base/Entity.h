/*****************************************************************
*\file         Entity.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#pragma warning (disable: 4201)

namespace Xivi::ECS
{
struct Entity final
{
  union Validation final
  {
    u32 m_value { 2147483649u };
    struct
    {
      u32 m_generation : 31, m_zombie : 1;
    };
    bool operator==( const Validation &validation ) const noexcept
    {
      return m_value == validation.m_value;
    }
  };
  static_assert( sizeof( Validation ) == sizeof( u32 ) );

  struct Descriptor final
  {
    std::string m_name {};
    Ptr<Archetype::Instance> m_archetype { nullptr };
    Ptr<Pool::Instance> m_pool { nullptr };
    Pool::Index m_pool_index {};
    Validation m_validation { 2147483649u };

    Descriptor()
    {
      m_validation.m_value = 2147483649u;
    }

    u32 Generation() const noexcept
    {
      return m_validation.m_generation;
    }

    bool Zombie() const noexcept
    {
      return m_validation.m_zombie;
    }

    void Zombie( bool b ) noexcept
    {
      m_validation.m_zombie = b;
    }
  };

  static constexpr u64 invalid_entity = ~static_cast<u64>( 0 );
  static constexpr auto info = Component::Type::Data {
    .m_name = "Entity" };

  // Data
  u32 m_global_index { ~static_cast<u32>( 0 ) };
  Validation m_validation { ~static_cast<u32>( 0 ) };

  Entity() noexcept = default;

  Entity( u32 index, Validation validation ) noexcept
    : m_global_index( index ), m_validation( validation )
  {}

  u32 Index() const noexcept
  {
    return m_global_index;
  }

  void Index( u32 value ) noexcept
  {
    m_global_index = value;
  }

  u32 Generation() const noexcept
  {
    return m_validation.m_generation;
  }

  void Generation( u32 value ) noexcept
  {
    m_validation.m_generation = value;
  }

  bool Zombie() const noexcept
  {
    return m_validation.m_zombie;
  }

  void Zombie( bool b ) noexcept
  {
    m_validation.m_zombie = b;
  }

  bool Valid() const noexcept
  {
    return std::bit_cast<u64>( *this ) != invalid_entity;
  }

  bool operator==( const Entity &entity ) const noexcept
  {
    return std::bit_cast<u64>( *this ) == std::bit_cast<u64>( entity );
  }
};
static_assert( sizeof( Entity ) == sizeof( u64 ) );
}

namespace Xivi
{
using Entity = ECS::Entity;
}

namespace Xivi::ECS::Components::Entity::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::Entity::Validation>( "Validation" )
  .property( "Value", &Xivi::Entity::Validation::m_value );

  rttr::registration::class_<Xivi::Entity::Descriptor>( "Entity Descriptor" )
  .property( "Name", &Xivi::Entity::Descriptor::m_name )
  .property( "Validation", &Xivi::Entity::Descriptor::m_validation );

  rttr::registration::class_<Xivi::Entity>( "Entity" )
  ( rttr::metadata( "GUID", Component::info<ECS::Entity>.m_id ) )
  .property( "Global Index", &Xivi::Entity::m_global_index )
  .property( "Validation", &Xivi::Entity::m_validation );
}
}

namespace std
{
template<>
struct hash<Xivi::ECS::Entity>
{
  std::size_t operator()( const Xivi::ECS::Entity &e ) const
  {
    return Xivi::Hash::XXH3_64( e );
  }
};
}