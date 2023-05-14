/*****************************************************************
*\file         GUIManager.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include "Xivi/ECS/XVECS.h"
#include "Components/GUIComponents.h"

namespace Xivi::GUI
{
class Manager
{
  struct FuncSort
  {
    bool operator()( const std::string &lhs, const std::string &rhs ) const noexcept
    {
      return lhs < rhs;
    }
  };

  using CanvasContainer = std::vector<Entity>;
  CanvasContainer m_canvas_container;
  using FuncMapping = std::map<std::string, std::function<void( Entity )>, FuncSort>;
  FuncMapping m_function_map;

public:
  Ptr<CanvasContainer> Get() noexcept
  {
    return &m_canvas_container;
  }

  Entity Push( Entity entity ) noexcept
  {
    XV_CORE_ASSERT_MSG( entity.Valid(), "Pushing invalid entity into container" );

    auto world = &Service::Locator::Get<ECS::World::Instance>();
    XV_CORE_ASSERT_MSG( world->HaveComponent<UI>( entity ), "Pushing entity that is not UI" );

    m_canvas_container.push_back( entity );
    return entity;
  }

  Entity Pop() noexcept
  {
    XV_CORE_ASSERT_MSG( !m_canvas_container.empty(), "Popping empty container" );
    Entity tmp = m_canvas_container.back();
    m_canvas_container.pop_back();
    return tmp;
  }

  bool Exist( Entity entity ) noexcept
  {
    for ( auto &ui : m_canvas_container )
      if ( ui == entity )
        return true;
    return false;
  }

  void PopAll() noexcept
  {
    XV_CORE_ASSERT_MSG( !m_canvas_container.empty(), "Popping empty container" );
    while ( !m_canvas_container.empty() )
      m_canvas_container.pop_back();
  }

  Entity GetGUIByName( std::string name ) const noexcept
  {
    auto world = &Service::Locator::Get<ECS::World::Instance>();
    for ( auto &ui : m_canvas_container )
    {
      auto &entry = world->m_entity_manager.Get( ui );
      if ( entry.m_name == name )
        return ui;
    }
    return Entity {};
  }

  Entity Top() const noexcept
  {
    XV_CORE_ASSERT_MSG( !m_canvas_container.empty(), "Getting top from empty container" );
    return m_canvas_container.back();
  }

  Entity operator[]( const CanvasContainer::size_type index ) const noexcept
  {
    XV_CORE_ASSERT_MSG( index != std::numeric_limits<CanvasContainer::size_type>::max() &&
                        index < m_canvas_container.size(),
                        "Invalid index" );

    return m_canvas_container[index];
  }

  Entity &operator[]( const CanvasContainer::size_type index ) noexcept
  {
    XV_CORE_ASSERT_MSG( index != std::numeric_limits<CanvasContainer::size_type>::max() &&
                        index < m_canvas_container.size(),
                        "Invalid index" );

    return m_canvas_container[index];
  }

  Entity At( const CanvasContainer::size_type index ) const noexcept
  {
    XV_CORE_ASSERT_MSG( index != std::numeric_limits<CanvasContainer::size_type>::max() &&
                        index < m_canvas_container.size(),
                        "Invalid index" );

    return m_canvas_container.at( index );
  }

  bool Empty() const noexcept
  {
    return m_canvas_container.empty();
  }

  CanvasContainer::size_type Size() const noexcept
  {
    return m_canvas_container.size();
  }

  void Remove( const Entity entity ) noexcept
  {
    for ( auto it = m_canvas_container.begin(); it != m_canvas_container.end(); ++it )
      if ( *it == entity )
      {
        m_canvas_container.erase( it );
        break;
      }
  }

  void Sort() noexcept
  {
    std::sort( m_canvas_container.begin(), m_canvas_container.end(), []( const Entity &lhs, const Entity &rhs )
    {
      auto world = &Service::Locator::Get<ECS::World::Instance>();
      auto &lhs_pos = world->GetComponent<Position>( lhs );
      auto &rhs_pos = world->GetComponent<Position>( rhs );

      return Math::SIMD::getX( lhs_pos.m_position ) > Math::SIMD::getX( rhs_pos.m_position );
    } );
  }

  bool Find( const Entity &entity )
  {
    for ( auto &ui : m_canvas_container )
      if ( ui == entity )
        return true;
    return false;
  }

  //void ChangeLayer(Entity entity, i32 new_layer) noexcept
  //{
  //	for (CanvasContainer::size_type i = 0 ; i < m_canvas_container.size(); ++i)
  //	{
  //		if (m_canvas_container[i].first == entity)
  //		{
  //			m_canvas_container[i].second = new_layer;
  //			i32 tmp = static_cast<i32>(i);
  //			for (auto j = 1; (tmp - j) >= 0 && m_canvas_container[tmp - j].second > m_canvas_container[tmp].second; ++j, --tmp)
  //				std::swap(m_canvas_container[tmp - j], m_canvas_container[tmp]);
  //
  //			tmp = static_cast<i32>(i);
  //			for (auto j = 1; (tmp + j) < m_canvas_container.size() && m_canvas_container[tmp + j].second < m_canvas_container[tmp].second; ++j, ++tmp)
  //				std::swap(m_canvas_container[tmp + j], m_canvas_container[tmp]);
  //
  //			break;
  //		}
  //	}
  //}

  //void IncreaseLayer(Entity entity) noexcept
  //{
  //	// If it has the biggest layer, do nothing
  //	if (m_canvas_container[m_canvas_container.size() - 1].first == entity)
  //		return;

  //	for (CanvasContainer::size_type i = 0; i < m_canvas_container.size(); i++)
  //		if (m_canvas_container[i].first == entity)
  //		{
  //			m_canvas_container[i].first = m_canvas_container[i + 1].first;
  //			m_canvas_container[i + 1].first = entity;
  //			break;
  //		}
  //}

  //void DecreaseLayer(Entity entity) noexcept
  //{
  //	// If it has the smallest layer, do nothing
  //	if (m_canvas_container[0].first == entity)
  //		return;

  //	for (CanvasContainer::size_type i = 0; i < m_canvas_container.size(); i++)
  //		if (m_canvas_container[i].first == entity)
  //		{
  //			m_canvas_container[i].first = m_canvas_container[i - 1].first;
  //			m_canvas_container[i - 1].first = entity;
  //			break;
  //		}
  //}

  FuncMapping &GetFuncMap() noexcept
  {
    return m_function_map;
  }

  /*std::function<void(Entity)> GetFunc(const char* key) noexcept
  {
    return m_function_map[key];
  }*/

  std::function<void( Entity )> GetFunc( const std::string key ) noexcept
  {
    return m_function_map[key.c_str()];
  }

  void AddFunc( std::string key, std::function<void( Entity )> func ) noexcept
  {
    m_function_map[key] = func;
  }

  void SetAllFuncByKey() noexcept
  {
    auto &world = Service::Locator::Get<ECS::World::Instance>();
    world.ForEach( [&]( Entity &entity, UIHover *uihover, UITrigger *uitrigger )
    {
      if ( uihover )
      {
        const char *key = uihover->m_key_to_func.c_str();
        if ( m_function_map.find( key ) != m_function_map.end() )
          uihover->m_callback = m_function_map[key];

        const char *unhoverkey = uihover->m_key_to_func_unhover.c_str();
        if ( m_function_map.find( unhoverkey ) != m_function_map.end() )
          uihover->m_callback_unhover = m_function_map[unhoverkey];
      }
      if ( uitrigger )
      {
        const char *key = uitrigger->m_key_to_func.c_str();
        if ( m_function_map.find( key ) != m_function_map.end() )
          uitrigger->m_callback = m_function_map[key];
      }
    } );
  }
};
}