/*****************************************************************
*\file         EntityManager.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS
{
struct EntityManager final : NonCopyable
{
  // std::array for now
  // Switch to pinned vector to optimise memory and update the linked-list set-up
  array<Entity::Descriptor, Settings::max_entities> m_entities {};
  i32 m_head { 0 };
  u32 m_count { 0 };
  u32 m_max { 0 };

  inline EntityManager() noexcept;
  inline void Initialise();
  inline Entity Allocate( Archetype::Instance &archetype, Pool::Instance &pool, Pool::Index pool_index ) noexcept;
  inline void Delete( u32 index ) noexcept;
  inline void Delete( u32 index, const Entity &entity ) noexcept;
  inline const Entity::Descriptor &Get( Entity entity ) const noexcept;
  inline  Entity::Descriptor &Get( Entity entity ) noexcept;
  inline void Move( Pool::Index pool_index, const Entity &entity ) noexcept;
};
}