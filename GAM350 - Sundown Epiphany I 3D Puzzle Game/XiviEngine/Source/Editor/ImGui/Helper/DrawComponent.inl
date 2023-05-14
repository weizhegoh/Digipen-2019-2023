/*****************************************************************
*\file         DrawComponent.ini
*\author(s)    Chng Yong Quan, Goh Wei Zhe

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#ifdef XV_EDITOR
namespace Xivi::Editor::Draw
{
template <Traits::Concepts::InvokableFunction Func>
void ShowProperties( Ptr<byte> data, const Ptr<const ECS::Component::Descriptor> desc, ImGuiTextFilter &filter, Func func ) noexcept
{
  auto instance = desc->m_reflect( data );
  auto instance_name = instance.get_type().get_name().to_string();
  if ( filter.PassFilter( instance_name.c_str() ) )
  {
    ImGui::PushItemWidth( ImGui::GetWindowWidth() - 30.0f );
    ImGui::BeginGroup();
    if ( ImGui::CollapsingHeader( instance_name.c_str(), ImGuiTreeNodeFlags_FramePadding & ImGuiTreeNodeFlags_AllowItemOverlap ) )
    {
      ImGui::Dummy( { 0.0f, 8.0f } );

      func( instance, instance_name, desc );

      ImGui::Dummy( { 0.0f, 8.0f } );
    }
    ImGui::EndGroup();
    ImGui::PopItemWidth();
  }
}

template <typename DataType>
void RenderAssetsProperties( const std::string &prop_name,
                             rttr::property property,
                             const Ptr<const ECS::Component::Descriptor> desc,
                             Resource::Handler<DataType> &data,
                             const Entity entity,
                             Ptr<RM> resource_manager,
                             Ptr<EntityContext> entity_context,
                             Ptr<ECS::World::Instance> world ) noexcept
{
  const auto &id = data.Key().m_value;

  ImGuiHelper::DropDown( ( "##" + prop_name ).c_str(), data.File().c_str(), [&]
  {
    auto list = resource_manager->List<DataType>();
    std::sort( list.begin(), list.end(), [&]( const std::string &lhs, const std::string &rhs )
    {
      return lhs < rhs;
    } );
    for ( auto i = 0u; i < list.size(); ++i )
    {
      bool is_selected = ( Resource::ID( list[i] ) == id );
      if ( ImGui::Selectable( list[i].c_str(), is_selected ) )
      {
        entity_context->SetComponentSnapshot<DataType>( *world, desc, property, Resource::ID( list[i] ) );
        System::Instance().m_command_history.Commit<UpdateComponentCommand>( *world,
                                                                             entity_context->LastSelectedEntity(), *desc, entity_context->snapshots[0][0] );
      }

      if ( is_selected )
        ImGui::SetItemDefaultFocus();
    }
  } );
}

template <Traits::Concepts::FuncReturnType<bool> Func>
void SnapshotInput( const Ptr<const ECS::Component::Descriptor> desc,
                    const Entity entity,
                    Ptr<EntityContext> entity_context,
                    Ptr<ECS::World::Instance> world,
                    Func func ) noexcept
{
  Entity ent = entity;
  if ( func() )
    ComponentUpdateSystem::OnCall( *world, ent, *desc );

  if ( ImGui::IsItemActivated() )
    entity_context->SetComponentSnapshot( *world, desc );
  else if ( ImGui::IsItemDeactivatedAfterEdit() )
    System::Instance().m_command_history.Commit<UpdateComponentCommand>( *world, ent, *desc, entity_context->snapshots[0][0] );
}

template <ECS::Component::Concepts::IsComponent C, Traits::Concepts::InvokableFunction Func>
void SpecializeDrag3FloatReset( Ptr<byte> data,
                                Entity entity,
                                const Ptr<const ECS::Component::Descriptor> desc,
                                Ptr<EntityContext> entity_context,
                                Ptr<ECS::World::Instance> world,
                                ImGuiTextFilter &filter,
                                Func func ) noexcept
{
  ShowProperties( data, desc, filter, [&]( rttr::instance &instance, std::string instance_name, const Ptr<const ECS::Component::Descriptor> desc )
  {
    for ( auto property : instance.get_type().get_properties() )
    {
      if ( property.get_metadata( "NoEditor" ) )
        continue;

      auto prop_type = property.get_type();
      auto prop_name = property.get_name().to_string();

      ImGui::Text( prop_name.c_str() );
      ImGui::SameLine();

      // For resetting
      ImGui::PushID( prop_name.c_str() );
      if ( ImGui::Button( ICON_FA_RECYCLE, { 20.0f, 20.0f } ) )
        func( prop_name );
      ImGui::PopID();

      prop_name.assign( prop_name + instance_name );

      rttr::variant value = property.get_value( instance );

      if ( prop_type == rttr::type::get<std::reference_wrapper<Vec3f>>() )
      {
        SnapshotInput( desc, entity, entity_context, world, [&]
        {
          if ( ImGui::DragFloat3( ( "##" + prop_name ).c_str(), std::bit_cast<Ptr<f32>>( &value.get_value<std::reference_wrapper<Vec3f>>().get() ), 0.01f ) )
          {
            auto &new_val = value.get_value<std::reference_wrapper<Vec3f>>().get();
            for ( auto &selected_entity : entity_context->selections )
            {
              Entity ent = selected_entity;
              auto &entry = world->m_entity_manager.Get( ent );
              auto pool = entry.m_pool;
              auto pool_index = entry.m_pool_index;
              for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
              {
                if ( desc->m_bit_index != desc2->m_bit_index )
                  continue;
                auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
                rttr::variant setter = property.get_value( selected_instance );

                setter.get_value<std::reference_wrapper<Vec3f>>().get() = new_val;
              }
            }
            return true;
          }
          return false;
        } );
      }
    }
  } );
}

template <ECS::Component::Concepts::IsComponent C>
inline void DrawComponent<C>( Ptr<byte> data,
                              Entity entity,
                              const Ptr<const ECS::Component::Descriptor> desc,
                              const ECS::Signature &signature,
                              Ptr<RM> resource_manager,
                              Ptr<EntityContext> entity_context,
                              Ptr<ECS::World::Instance> world,
                              ImGuiTextFilter &filter,
                              const ECS::Signature can_edit ) noexcept
{
  // Printing property
  auto instance = ECS::Component::info<C>.m_reflect( data );
  auto instance_name = instance.get_type().get_name().to_string();
  if ( filter.PassFilter( instance_name.c_str() ) )
    RenderProperties( entity, instance, desc, signature, resource_manager, entity_context, world );

  // Printing popup when right click
  ImGui::PushID( std::string { desc->m_name }.c_str() );
  PopUpDeleteComponent( world, ECS::Component::DescriptorManager::Instance(), entity, desc->m_id, entity_context, signature, can_edit[desc->m_bit_index] );
  ImGui::PopID();
}

template<>
inline void DrawComponent<Position>( Ptr<byte> data,
                                     Entity entity,
                                     const Ptr<const ECS::Component::Descriptor> desc,
                                     const ECS::Signature &signature,
                                     Ptr<RM> resource_manager,
                                     Ptr<EntityContext> entity_context,
                                     Ptr<ECS::World::Instance> world,
                                     ImGuiTextFilter &filter,
                                     const ECS::Signature can_edit ) noexcept
{
  auto &position = *std::bit_cast<Ptr<Position>>( data );
  auto speed = position.speed;
  float pos[3];
  position.m_position.store3( pos );
  ImGui::PushItemWidth( ImGui::GetWindowWidth() - 30.0f );
  ImGui::BeginGroup();
  if ( ImGui::CollapsingHeader( "Position", ImGuiTreeNodeFlags_FramePadding & ImGuiTreeNodeFlags_AllowItemOverlap ) )
  {
    ImGui::Dummy( { 0.0f, 8.0f } );
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      bool change = false;
      ImGui::Text( "Position" );
      ImGui::SameLine();
      // For resetting
      ImGui::PushID( "Position" );
      if ( ImGui::Button( ICON_FA_RECYCLE, { 20.0f, 20.0f } ) )
        position.m_position.setAll( 0.0f );
      ImGui::PopID();
      if ( ImGui::DragFloat3( "##Position", pos, 0.01f ) )
      {
        for ( auto i = 0; i < 3; ++i )
          position.m_position[i] = pos[i];
        change = true;
      }
      ImGui::Text( "Interpolate Speed" );
      if ( ImGui::DragFloat( "##InterpolateSpeed", &speed, 0.01f ) )
      {
        position.speed = speed;
        change = true;
      }
      return change;
    } );
    ImGui::Dummy( { 0.0f, 8.0f } );
  }
  ImGui::EndGroup();
  ImGui::PopItemWidth();
}

template<>
inline void DrawComponent<Transform>( Ptr<byte> data,
                                      Entity entity,
                                      const Ptr<const ECS::Component::Descriptor> desc,
                                      const ECS::Signature &signature,
                                      Ptr<RM> resource_manager,
                                      Ptr<EntityContext> entity_context,
                                      Ptr<ECS::World::Instance> world,
                                      ImGuiTextFilter &filter,
                                      const ECS::Signature can_edit ) noexcept
{
  // Printing property
  SpecializeDrag3FloatReset<Transform>( data, entity, desc, entity_context, world, filter, [&]( const std::string prop_name ) noexcept
  {
    auto &transform = *std::bit_cast<Ptr<Transform>>( data );
    if ( prop_name == "Axis" )
      transform.axis.setAll( 0.0f );
    else if ( prop_name == "Scale" )
      transform.scale.setAll( 50.0f );
  } );
}

template<>
inline void DrawComponent<Colour>( Ptr<byte> data,
                                   Entity entity,
                                   const Ptr<const ECS::Component::Descriptor> desc,
                                   const ECS::Signature &signature,
                                   Ptr<RM> resource_manager,
                                   Ptr<EntityContext> entity_context,
                                   Ptr<ECS::World::Instance> world,
                                   ImGuiTextFilter &filter,
                                   const ECS::Signature can_edit ) noexcept
{
  ShowProperties( data, desc, filter, [&]( rttr::instance &instance, std::string instance_name, const Ptr<const ECS::Component::Descriptor> desc )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      auto &colour = *std::bit_cast<Ptr<Colour>>( data );
      bool changes = false;
      float tmp[3] { colour.colour[0], colour.colour[1], colour.colour[2] };
      ImGui::PushID( ( instance_name + "Colour" ).c_str() );
      ImGui::Text( "Colour" );
      ImGui::SameLine();
      if ( ImGui::Button( ICON_FA_RECYCLE, { 20.0f, 20.0f } ) )
      {
        colour.colour.setAll( 1.0f );
        colour.colour.store3( tmp ); // Set tmp to be same as vec as colour
        changes = true;
      }
      ImGui::PopID();
      if ( ImGui::ColorEdit3( "##ColorPicker", tmp ) )
      {
        for ( auto i = 0; i < 3; ++i )
          colour.colour[i] = tmp[i];
        changes = true;
      }
      return changes;
    } );
  } );
}

template<>
inline void DrawComponent<LightProperty>( Ptr<byte> data,
                                          Entity entity,
                                          const Ptr<const ECS::Component::Descriptor> desc,
                                          const ECS::Signature &signature,
                                          Ptr<RM> resource_manager,
                                          Ptr<EntityContext> entity_context,
                                          Ptr<ECS::World::Instance> world,
                                          ImGuiTextFilter &filter,
                                          const ECS::Signature can_edit ) noexcept
{
  ShowProperties( data, desc, filter, [&]( rttr::instance &instance, std::string instance_name, const Ptr<const ECS::Component::Descriptor> desc )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      auto &light_property = *std::bit_cast<Ptr<LightProperty>>( data );
      bool changes = false;
      f32 tmp_ambient[3] { light_property.ambient[0], light_property.ambient[1],light_property.ambient[2] };
      f32 tmp_diffuse[3] { light_property.diffuse[0], light_property.diffuse[1],light_property.diffuse[2] };
      f32 tmp_specular[3] { light_property.specular[0], light_property.specular[1],light_property.specular[2] };

      auto render_picker = [&]( std::string name, Vec4f &prop, Ptr<f32> tmp )
      {
        ImGui::PushID( ( instance_name + name ).c_str() );
        ImGui::Text( name.c_str() );
        ImGui::SameLine();
        if ( ImGui::Button( ICON_FA_RECYCLE, { 20.0f, 20.0f } ) )
        {
          //prop.setAll( 1.0f );
          //prop.store3( tmp ); // Set tmp to be same as vec as colour
          prop[0] = tmp[0];
          prop[1] = tmp[1];
          prop[2] = tmp[2];
          changes = true;
        }
        ImGui::PopID();
        if ( ImGui::ColorEdit3( ( std::string( "##" ) + name + "Picker" ).c_str(), tmp ) )
        {
          for ( auto i = 0; i < 3; ++i )
            prop[i] = tmp[i];
          changes = true;
        }
      };
      render_picker( "Ambient", light_property.ambient, tmp_ambient );
      ImGui::Text( "Ambient Radius" );
      ImGui::SameLine();
      if ( ImGui::DragFloat( "##AmbientRadius", &light_property.ambient[3] ) )
        changes = true;

      render_picker( "Diffuse", light_property.diffuse, tmp_diffuse );
      ImGui::Text( "DiffuseRadius" );
      ImGui::SameLine();
      if ( ImGui::DragFloat( "##DiffuseRadius", &light_property.diffuse[3] ) )
        changes = true;

      render_picker( "Specular", light_property.specular, tmp_specular );
      ImGui::Text( "Specular Radius" );
      ImGui::SameLine();
      if ( ImGui::DragFloat( "##SpecularRadius", &light_property.specular[3] ) )
        changes = true;

      ImGui::Text( "Radius" );
      ImGui::SameLine();
      if ( ImGui::DragFloat( "##Radius", &light_property.radius ) )
        changes = true;

      ImGui::Text( "Near Plane" );
      ImGui::SameLine();
      if ( ImGui::DragFloat( "##NearPlane", &light_property.near_plane ) )
        changes = true;

      ImGui::Text( "Far Plane" );
      ImGui::SameLine();
      if ( ImGui::DragFloat( "##FarPlane", &light_property.far_plane ) )
        changes = true;

      ImGui::Text( "Bound" );
      ImGui::SameLine();
      if ( ImGui::DragFloat( "##Bound", &light_property.bound ) )
        changes = true;

      ImGui::Text( "View At" );
      ImGui::SameLine();
      if ( ImGui::DragFloat3( "##ViewAt", &light_property.view_at[0] ) )
        changes = true;

      ImGui::Text( "View Up" );
      ImGui::SameLine();
      if ( ImGui::DragFloat3( "##ViewUp", &light_property.view_up[0] ) )
        changes = true;

      ImGui::Text( "Light Render Shadow" );
      ImGui::SameLine();
      if ( ImGui::Checkbox( "##Render Shadow", &light_property.toRenderShadow ) )
        changes = true;

      return changes;
    } );
  } );
}

template<>
inline void DrawComponent<EntityType>( Ptr<byte> data,
                                       Entity entity,
                                       const Ptr<const ECS::Component::Descriptor> desc,
                                       const ECS::Signature &signature,
                                       Ptr<RM> resource_manager,
                                       Ptr<EntityContext> entity_context,
                                       Ptr<ECS::World::Instance> world,
                                       ImGuiTextFilter &filter,
                                       const ECS::Signature can_edit ) noexcept
{
  ShowProperties( data, desc, filter, [&]( rttr::instance &instance, std::string instance_name, const Ptr<const ECS::Component::Descriptor> desc )
  {
    auto &grid_pos = world->GetComponent<GridPosition>( entity );
    auto &grid = world->GetOrCreateSingletonComponent<GameGrid>();
    auto &type = world->GetComponent<EntityType>( entity );

    for ( auto property : instance.get_type().get_properties() )
    {
      if ( property.get_metadata( "NoEditor" ) )
        continue;

      auto prop_type = property.get_type();
      auto prop_name = property.get_name().to_string();

      auto value = property.get_value( instance );

      ImGui::Text( prop_name.c_str() );

      if ( RenderEnumeration( prop_name, prop_type, value, instance, property, desc, entity, entity_context, world ) )
        if ( grid_pos.value.x >= 0 && grid_pos.value.y >= 0 && grid_pos.value.z >= 0 )
          grid.UpdateCell( grid_pos.value, type.value, entity );
    }
  } );
}

template<>
inline void DrawComponent<GridPosition>( Ptr<byte> data,
                                         Entity entity,
                                         const Ptr<const ECS::Component::Descriptor> desc,
                                         const ECS::Signature &signature,
                                         Ptr<RM> resource_manager,
                                         Ptr<EntityContext> entity_context,
                                         Ptr<ECS::World::Instance> world,
                                         ImGuiTextFilter &filter,
                                         const ECS::Signature can_edit ) noexcept
{
  ShowProperties( data, desc, filter, [&]( rttr::instance &instance, std::string instance_name, const Ptr<const ECS::Component::Descriptor> desc )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      auto &grid_pos = world->GetComponent<GridPosition>( entity );
      auto &grid = world->GetOrCreateSingletonComponent<GameGrid>();
      auto &pos = world->GetComponent<Position>( entity ).m_position;
      auto &type = world->GetComponent<EntityType>( entity );

      for ( auto property : instance.get_type().get_properties() )
      {
        if ( property.get_metadata( "NoEditor" ) )
          continue;

        auto prop_type = property.get_type();
        auto prop_name = property.get_name().to_string();

        auto value = property.get_value( instance );

        ImGui::Text( prop_name.c_str() );

        ImGui::SameLine();

        if ( !world->HaveComponent<Parent>( entity ) )
        {
          ImGui::PushID( prop_name.c_str() );
          if ( ImGui::Button( ICON_FA_RECYCLE, { 20.0f, 20.0f } ) )
          {
            if ( prop_name == "Offset" )
            {
              value.get_value<std::reference_wrapper<Vec3f>>().get().setToZero();
              pos = grid.WorldSpace( grid_pos.value );
              ImGui::PopID();
              return true;
            }

            else if ( prop_name == "Value" )
            {
              if ( grid_pos.value.x != -1 && grid_pos.value.y != -1 && grid_pos.value.z != -1 ) // Null value check
                grid.UpdateCell( grid_pos.value, GameType::Empty );
              grid_pos.value = { 0,0,0 };
              grid.UpdateCell( grid_pos.value, type.value, entity );
              pos = grid.WorldSpace( grid_pos.value );
              ImGui::PopID();
              return true;
            }
          }
          ImGui::PopID();

          if ( prop_type == rttr::type::get<std::reference_wrapper<Vec3i>>() )
          {
            auto tmp = value.get_value<std::reference_wrapper<Vec3i>>().get();
            if ( ImGui::DragInt3( "##GridPosition", std::bit_cast<Ptr<i32>>( &tmp ), 1.0f ) )
            {
              auto &new_val = tmp;
              for ( auto &selected_entity : entity_context->selections )
              {
                Entity ent = selected_entity;
                auto &entry = world->m_entity_manager.Get( ent );
                auto pool = entry.m_pool;
                auto pool_index = entry.m_pool_index;
                for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
                {
                  if ( desc->m_bit_index != desc2->m_bit_index )
                    continue;
                  auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
                  rttr::variant setter = property.get_value( selected_instance );

                  new_val.x = Math::clamp( new_val.x, -1, grid.grid_size.x - 1 );
                  new_val.y = Math::clamp( new_val.y, -1, grid.grid_size.y - 1 );
                  new_val.z = Math::clamp( new_val.z, -1, grid.grid_size.z - 1 );

                  if ( new_val.x != -1 &&
                       new_val.y != -1 &&
                       new_val.z != -1 )
                  {
                    if ( grid.GetCellType( new_val ) == GameType::Empty )
                    {
                      if ( grid_pos.value.x != -1 && grid_pos.value.y != -1 && grid_pos.value.z != -1 ) // Null value check
                        grid.UpdateCell( grid_pos.value, GameType::Empty );
                      grid_pos.value = new_val;
                      grid.UpdateCell( new_val, type.value, entity );
                      pos = grid.WorldSpace( new_val );
                      setter.get_value<std::reference_wrapper<Vec3i>>().get() = new_val;
                    }
                    else
                    {
                      setter.get_value<std::reference_wrapper<Vec3i>>().get() = grid_pos.value;
                      pos = grid.WorldSpace( grid_pos.value );
                    }
                  }
                }
              }
              return true;
            }
          }
          else if ( prop_type == rttr::type::get<std::reference_wrapper<Vec3f>>() )
          {
            if ( ImGui::DragFloat3( ( "##" + prop_name ).c_str(), std::bit_cast<Ptr<f32>>( &value.get_value<std::reference_wrapper<Vec3f>>().get() ), 0.01f ) )
            {
              auto &new_val = value.get_value<std::reference_wrapper<Vec3f>>().get();
              for ( auto &selected_entity : entity_context->selections )
              {
                Entity ent = selected_entity;
                auto &entry = world->m_entity_manager.Get( ent );
                auto pool = entry.m_pool;
                auto pool_index = entry.m_pool_index;
                for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
                {
                  if ( desc->m_bit_index != desc2->m_bit_index )
                    continue;
                  auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
                  rttr::variant setter = property.get_value( selected_instance );

                  setter.get_value<std::reference_wrapper<Vec3f>>().get() = new_val;
                }
              }

              if ( prop_name == "Offset" )
                pos = grid.WorldSpace( grid_pos.value ) + new_val;
              return true;
            }
          }
        }
        else if ( prop_type == rttr::type::get<std::reference_wrapper<Vec3i>>() )
        {
          auto tmp = value.get_value<std::reference_wrapper<Vec3i>>().get();
          std::string str = std::to_string( tmp.x ) + "\t" + std::to_string( tmp.y ) + "\t" + std::to_string( tmp.z );
          ImGui::Text( str.c_str() );
        }
      }
      return false;
    } );
  } );
}

template<>
inline void DrawComponent<Waypoints>( Ptr<byte> data,
                                      Entity entity,
                                      const Ptr<const ECS::Component::Descriptor> desc,
                                      const ECS::Signature &signature,
                                      Ptr<RM> resource_manager,
                                      Ptr<EntityContext> entity_context,
                                      Ptr<ECS::World::Instance> world,
                                      ImGuiTextFilter &filter,
                                      const ECS::Signature can_edit ) noexcept
{
  ShowProperties( data, desc, filter, [&]( rttr::instance &instance, std::string instance_name, const Ptr<const ECS::Component::Descriptor> desc )
  {
    bool change = false;

    auto &vecs = std::bit_cast<Ptr<Waypoints>>( data )->moving_waypoints;
    auto &grid = world->GetOrCreateSingletonComponent<GameGrid>();
    ImGui::Text( "Moving Waypoints" );
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      ImGuiHelper::Table( ( "##vectors_of_vec3i" + std::to_string( entity.m_global_index ) ).c_str(), 2, ImGuiTableFlags_SizingStretchProp, [&]
      {
        int i = 0;
        for ( auto it = vecs.begin(); it != vecs.end(); ++it, ++i )
        {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          if ( ImGui::DragInt3( ( "##waypoints" + std::to_string( i ) ).c_str(), std::bit_cast<Ptr<i32>>( &( *it ) ), 0.01f ) )
          {
            it->x = Math::clamp( it->x, 0, grid.grid_size.x - 1 );
            it->y = Math::clamp( it->y, 0, grid.grid_size.y - 1 );
            it->z = Math::clamp( it->z, 0, grid.grid_size.z - 1 );
            change = true;
          }

          ImGui::TableNextColumn();
          if ( ImGui::Button( ICON_FA_MINUS, { 20.f, 20.f } ) )
          {
            vecs.erase( it );
            change = true;
            break;
          }
        }
      } );

      if ( ImGui::Button( ICON_FA_PLUS, { 20.f, 20.f } ) )
        vecs.push_back( { 0, 0, 0 } );
      ImGui::SameLine();
      ImGui::Text( "Add Waypoint" );
      return change;
    } );

    for ( auto property : instance.get_type().get_properties() )
    {
      if ( property.get_metadata( "NoEditor" ) )
        continue;

      auto prop_type = property.get_type();
      auto prop_name = property.get_name().to_string();

      if ( prop_name == "Moving Waypoints" )
        continue;

      auto value = property.get_value( instance );

      ImGui::Text( prop_name.c_str() );

      if ( prop_type == rttr::type::get<std::reference_wrapper<bool>>() )
      {
        SnapshotInput( desc, entity, entity_context, world, [&]
        {
          if ( ImGui::Checkbox( ( "##" + prop_name ).c_str(), &value.get_value<std::reference_wrapper<bool>>().get() ) )
          {
            auto &new_val = value.get_value<std::reference_wrapper<bool>>().get();
            for ( auto &selected_entity : entity_context->selections )
            {
              Entity ent = selected_entity;
              auto &entry = world->m_entity_manager.Get( ent );
              auto pool = entry.m_pool;
              auto pool_index = entry.m_pool_index;
              for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
              {
                if ( desc->m_bit_index != desc2->m_bit_index )
                  continue;
                auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
                rttr::variant setter = property.get_value( selected_instance );

                setter.get_value<std::reference_wrapper<bool>>().get() = new_val;
              }
            }
            return true;
          }

          return false;
        } );
      }
      else if ( prop_type == rttr::type::get<std::reference_wrapper<i32>>() )
      {
        SnapshotInput( desc, entity, entity_context, world, [&]
        {
          if ( ImGui::InputInt( ( "##" + prop_name ).c_str(), ( &value.get_value<std::reference_wrapper<i32>>().get() ), 1 ) )
          {
            auto &new_val = value.get_value<std::reference_wrapper<i32>>().get();
            for ( auto &selected_entity : entity_context->selections )
            {
              Entity ent = selected_entity;
              auto &entry = world->m_entity_manager.Get( ent );
              auto pool = entry.m_pool;
              auto pool_index = entry.m_pool_index;
              for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
              {
                if ( desc->m_bit_index != desc2->m_bit_index )
                  continue;
                auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
                rttr::variant setter = property.get_value( selected_instance );

                setter.get_value<std::reference_wrapper<i32>>().get() = new_val;
              }
            }
            return true;
          }

          return false;
        } );
      }
    }
  } );
}

inline void AudioTableHeader() noexcept
{
  static std::array<const char *, 4> header
  {
    "Actions",
    "Files",
    "Remove",
    "Play/Stop"
  };
  for ( auto i = 0; i < 4; ++i )
  {
    ImGui::TableNextColumn();
    ImGui::Text( header[i] );
    if ( ImGuiHelper::IsItemHovered() )
    {
      ImVec2 m = ImGui::GetIO().MousePos;
      ImGui::SetNextWindowPos( ImVec2( m.x - 10, m.y ) );
      ImGuiHelper::Tooltip( [&]
      {
        ImGui::Text( header[i] );
      } );
    }
  }
}

inline void AudioColumn1( const std::string &act,
                          const std::string &file,
                          std::string &action,
                          std::string &filename,
                          bool &empty_action,
                          bool &empty_filename,
                          const bool is_selected,
                          const std::string &selected_attached_action,
                          const std::string &selected_val ) noexcept
{
  if ( ImGui::Selectable( selected_val.c_str(), is_selected ) && !selected_val.empty() )
  {
    // Set the action and filename when selected
    action = selected_val;
    filename = file;
    empty_action = false;
    empty_filename = false;
  }
  if ( ImGuiHelper::IsItemHovered() )
  {
    ImVec2 m = ImGui::GetIO().MousePos;
    ImGui::SetNextWindowPos( ImVec2( m.x - 10, m.y ) );
    ImGuiHelper::Tooltip( [&]
    {
      ImGui::Text( selected_val.c_str() );
    } );
  }
}

inline void AudioColumn2( const std::string &file ) noexcept
{
  const char *file_char = file.c_str();
  ImGui::Text( file_char );
  // Display text when hovered
  if ( ImGuiHelper::IsItemHovered() )
  {
    ImVec2 m = ImGui::GetIO().MousePos;
    ImGui::SetNextWindowPos( ImVec2( m.x - 10, m.y ) );
    ImGuiHelper::Tooltip( [&]
    {
      ImGui::Text( file_char );
    } );
  }
}

template<>
inline void DrawComponent<Audio2DComponent>( Ptr<byte> data,
                                             Entity entity,
                                             const Ptr<const ECS::Component::Descriptor> desc,
                                             const ECS::Signature &signature,
                                             Ptr<RM> resource_manager,
                                             Ptr<EntityContext> entity_context,
                                             Ptr<ECS::World::Instance> world,
                                             ImGuiTextFilter &filter,
                                             const ECS::Signature can_edit ) noexcept
{
  ShowProperties( data, desc, filter, [&]( rttr::instance &instance, std::string instance_name, const Ptr<const ECS::Component::Descriptor> desc )
  {
    auto &audio = *std::bit_cast<Ptr<Audio2DComponent>>( data );

    const auto &audio_cont = audio.GetAudio2DCont();

    auto &audio_mgr = Audio::AudioManager::Instance();

    static std::string action;
    static std::string filename;

    static bool empty_action = false;
    static bool empty_filename = false;

    const auto &loop_cont = audio_mgr.GetLoopCont();

    // What audio the component comtain
    std::string selected_attached_action;
    ImGui::Text( "Audio Action Attached" );

    static const int column = 4;

    static std::string action_currently_playing;

    ImGuiHelper::ChildWindow( "##2d_audio_list", { 0.0f, 200.0f }, true, 0, [&]
    {
      ImGuiHelper::Table( "##2d_audio_table", column, ImGuiTableFlags_BordersInner, [&]
      {
        // Header (top row)
        ImGui::TableNextRow();
        AudioTableHeader();

        for ( const auto &[act, file] : audio_cont )
        {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();

          std::string selected_val = act;
          bool is_selected = ( selected_attached_action == selected_val );
          // Column 1 : Actions
          AudioColumn1( act, file, action, filename, empty_action, empty_filename, is_selected, selected_attached_action, selected_val );

          ImGui::TableNextColumn();
          // Column 2 : Files
          AudioColumn2( file );

          ImGui::TableNextColumn();

          // Column 3 : Remove
          ImGui::PushID( act.c_str() );
          if ( ImGui::Button( ICON_FA_MINUS, { 20.0f, 20.0f } ) )
          {
            // Stop and remove when delete button is pressed
            audio.Stop2DLoop( act );
            audio.DeleteAudio2DComp( act );
            action_currently_playing.clear();
          }
          ImGui::PopID();

          ImGui::TableNextColumn();

          // Column 4 : Play/Stop
          static bool is_playing = false;
          static bool prev_is_playing = false;
          audio_mgr.getChlnSFX( file )->isPlaying( &is_playing );

          const auto play_stop_button = is_playing ?
            ICON_FA_STOP :
            ICON_FA_PLAY;

          const bool loop = loop_cont.find( file ) != loop_cont.end();
          if ( action_currently_playing == act && !is_playing && prev_is_playing )
            action_currently_playing.clear();

          if ( ( action_currently_playing.empty() || action_currently_playing == act ) )
          {
            ImGui::PushID( act.c_str() );
            if ( ImGui::Button( play_stop_button, { 20.0f, 20.0f } ) )
            {
              if ( is_playing )
              {
                action_currently_playing.clear();
                audio.Stop2DLoop( act );
                is_playing = false;
              }
              else
              {
                loop ?
                  audio.Play2DLoop( act ) :
                  audio.Play2DSFX( act );
                action_currently_playing = act;
                is_playing = true;
              }
              is_playing = !is_playing;
            }
            ImGui::PopID();
          }

          if ( action_currently_playing == act )
            prev_is_playing = is_playing;

          if ( is_selected )
            ImGui::SetItemDefaultFocus();
        }
      } );
    } );

    ImGui::Text( "" );
    // Action
    ImGui::Text( "Action" );
    ImGui::SameLine();
    ImGui::PushID( "##action_reset" );
    if ( ImGui::Button( ICON_FA_RECYCLE, { 20.0f, 20.0f } ) )
      action.clear();
    ImGui::PopID();

    char buffer[256] {};
    std::copy( action.begin(), action.end(), buffer );
    if ( ImGui::InputText( "##audio2d", buffer, sizeof( buffer ) ) )
      action = std::string( buffer );

    // Print error for action
    ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1.0f, 0.20f, 0.20f, 0.85f ) );
    if ( empty_action )
      ImGui::Text( "Action cannot be empty" );
    else
      ImGui::Text( "" );
    ImGui::PopStyleColor();

    const char *current_item = filename.c_str();

    auto list_of_audio = audio_mgr.GetSFXCont();

    for ( auto &[name, sound] : loop_cont )
      list_of_audio.insert( { name, sound } );

    // List of audio files to select
    ImGui::Text( "Audio Files" );
    ImGuiHelper::DropDown( "##audio_files", current_item, [&]
    {
      for ( const auto &[name, sound] : list_of_audio )
      {
        std::string selected_val = name;
        bool is_selected = ( current_item == selected_val.c_str() );
        if ( ImGui::Selectable( selected_val.c_str(), is_selected ) )
          filename = selected_val;

        if ( is_selected )
          ImGui::SetItemDefaultFocus();
      }
    } );

  // Print error for audio files
    ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1.0f, 0.20f, 0.20f, 0.85f ) );
    if ( empty_filename )
      ImGui::Text( "Audio Files cannot be empty" );
    else
      ImGui::Text( "" );
    ImGui::PopStyleColor();

    auto button = audio.ExistAudio2DComp( action ) ?
      ICON_FA_EDIT :
      ICON_FA_PLUS;

    const char *text = audio.ExistAudio2DComp( action ) ?
      "Edit sound" :
      "Add to Component";

    // Add or edit sound
    if ( ImGui::Button( button, { 20.0f, 20.0f } ) )
    {
      empty_action = false;
      empty_filename = false;

      if ( action.empty() )
        empty_action = true;
      if ( filename.empty() )
        empty_filename = true;

      if ( !empty_action && !empty_filename )
      {
        audio.ExistAudio2DComp( action ) ?
          audio.EditAudio2DComp( action, filename ) :
          audio.AddAudio2DComp( action, filename );
        audio_mgr.StopAllAudio();
        action_currently_playing.clear();
      }
    }
    ImGui::SameLine();
    ImGui::Text( text );
  } );
}

template<>
inline void DrawComponent<Audio3DComponent>( Ptr<byte> data,
                                             Entity entity,
                                             const Ptr<const ECS::Component::Descriptor> desc,
                                             const ECS::Signature &signature,
                                             Ptr<RM> resource_manager,
                                             Ptr<EntityContext> entity_context,
                                             Ptr<ECS::World::Instance> world,
                                             ImGuiTextFilter &filter,
                                             const ECS::Signature can_edit ) noexcept
{
  ShowProperties( data, desc, filter, [&]( rttr::instance &instance, std::string instance_name, const Ptr<const ECS::Component::Descriptor> desc )
  {
    auto &audio = *std::bit_cast<Ptr<Audio3DComponent>>( data );
    const auto &audio_cont = audio.GetAudio3DCont();

    auto &audio_mgr = Audio::AudioManager::Instance();

    static std::string action;
    static std::string filename;

    static bool empty_action = false;
    static bool empty_filename = false;

    // What audio the component comtain
    std::string selected_attached_action;
    ImGui::Text( "Audio Action Attached" );

    static const int column = 4;

    static std::string action_currently_playing;

    ImGuiHelper::ChildWindow( "##3d_audio_list", { 0.0f, 200.0f }, true, 0, [&]
    {
      ImGuiHelper::Table( "##3d_audio_table", column, ImGuiTableFlags_BordersInner, [&]
      {
        // Header (top row)
        ImGui::TableNextRow();
        AudioTableHeader();

        for ( const auto &[act, file] : audio_cont )
        {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();

          std::string selected_val = act;
          bool is_selected = ( selected_attached_action == selected_val );
          // Column 1 : Actions
          AudioColumn1( act, file, action, filename, empty_action, empty_filename, is_selected, selected_attached_action, selected_val );

          ImGui::TableNextColumn();
          // Column 2 : Files
          AudioColumn2( file );

          ImGui::TableNextColumn();

          // Column 3 : Remove
          ImGui::PushID( act.c_str() );
          if ( ImGui::Button( ICON_FA_MINUS, { 20.0f, 20.0f } ) )
          {
            // Stop and remove when delete button is pressed
            audio_mgr.StopAllAudio();
            audio.DeleteAudio3DComp( act );
            action_currently_playing.clear();
          }
          ImGui::PopID();

          ImGui::TableNextColumn();

          // Column 4 : Play/Stop
          static bool is_playing = false;
          static bool prev_is_playing = false;
          audio_mgr.getChln3DSFX( entity )->isPlaying( &is_playing );

          const auto play_stop_button = is_playing ?
            ICON_FA_STOP :
            ICON_FA_PLAY;

          if ( action_currently_playing == act && !is_playing && prev_is_playing )
            action_currently_playing.clear();

          if ( ( action_currently_playing.empty() || action_currently_playing == act ) )
          {
            ImGui::PushID( act.c_str() );
            if ( ImGui::Button( play_stop_button, { 20.0f, 20.0f } ) )
            {
              if ( is_playing )
              {
                action_currently_playing.clear();
                audio_mgr.StopAllAudio();
                is_playing = false;
              }
              else
              {
                audio_mgr.play3DSFX( entity, act );
                action_currently_playing = act;
                is_playing = true;
              }
              is_playing = !is_playing;
            }
            ImGui::PopID();
          }

          if ( action_currently_playing == act )
            prev_is_playing = is_playing;

          if ( is_selected )
            ImGui::SetItemDefaultFocus();
        }
      } );
    } );

    ImGui::Text( "" );
    // Action
    ImGui::Text( "Action" );
    ImGui::SameLine();
    ImGui::PushID( "##action_reset" );
    if ( ImGui::Button( ICON_FA_RECYCLE, { 20.0f, 20.0f } ) )
      action.clear();
    ImGui::PopID();

    char buffer[256] {};
    std::copy( action.begin(), action.end(), buffer );
    if ( ImGui::InputText( "##audio3d", buffer, sizeof( buffer ) ) )
      action = std::string( buffer );

    // Print error for action
    ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1.0f, 0.20f, 0.20f, 0.85f ) );
    if ( empty_action )
      ImGui::Text( "Action cannot be empty" );
    else
      ImGui::Text( "" );
    ImGui::PopStyleColor();

    const char *current_item = filename.c_str();

    auto list_of_audio = audio_mgr.Get3DSFXCont();

    // List of audio files to select
    ImGui::Text( "Audio Files" );
    ImGuiHelper::DropDown( "##3d_audio_files", current_item, [&]
    {
      for ( const auto &[name, sound] : list_of_audio )
      {
        std::string selected_val = name;
        bool is_selected = ( current_item == selected_val.c_str() );
        if ( ImGui::Selectable( selected_val.c_str(), is_selected ) )
          filename = selected_val;

        if ( is_selected )
          ImGui::SetItemDefaultFocus();
      }
    } );

  // Print error for audio files
    ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1.0f, 0.20f, 0.20f, 0.85f ) );
    if ( empty_filename )
      ImGui::Text( "Audio Files cannot be empty" );
    else
      ImGui::Text( "" );
    ImGui::PopStyleColor();

    auto button = audio.ExistAudio3DComp( action ) ?
      ICON_FA_EDIT :
      ICON_FA_PLUS;

    const char *text = audio.ExistAudio3DComp( action ) ?
      "Edit sound" :
      "Add to Component";

    // Add or edit sound
    if ( ImGui::Button( button, { 20.0f, 20.0f } ) )
    {
      empty_action = false;
      empty_filename = false;

      if ( action.empty() )
        empty_action = true;
      if ( filename.empty() )
        empty_filename = true;

      if ( !empty_action && !empty_filename )
      {
        audio.ExistAudio3DComp( action ) ?
          audio.EditAudio3DComp( action, filename ) :
          audio.AddAudio3DComp( action, filename );
        audio_mgr.StopAllAudio();
        action_currently_playing.clear();
      }
    }
    ImGui::SameLine();
    ImGui::Text( text );
  } );
}

template<>
inline void DrawComponent<GUI::UIHover>( Ptr<byte> data,
                                         Entity entity,
                                         const Ptr<const ECS::Component::Descriptor> desc,
                                         const ECS::Signature &signature,
                                         Ptr<RM> resource_manager,
                                         Ptr<EntityContext> entity_context,
                                         Ptr<ECS::World::Instance> world,
                                         ImGuiTextFilter &filter,
                                         const ECS::Signature can_edit ) noexcept
{
  ShowProperties( data, desc, filter, [&]( rttr::instance &instance, std::string instance_name, const Ptr<const ECS::Component::Descriptor> desc )
  {
    auto &canvas_stack = Service::Locator::Get<GUI::Manager>();
    auto &list_of_gui_func = canvas_stack.GetFuncMap();

    auto &ui_hover = *std::bit_cast<Ptr<GUI::UIHover>>( data );

    const char *current_hover_item = ui_hover.m_key_to_func.c_str();
    ImGui::Text( "Hover Key" );
    ImGuiHelper::DropDown( "##current_hover_item", current_hover_item, [&]
    {
      for ( const auto &[key, func] : list_of_gui_func )
      {
        bool is_selected = ( !std::strcmp( current_hover_item, key.c_str() ) );
        if ( ImGui::Selectable( key.c_str(), is_selected ) )
        {
          ui_hover.m_key_to_func = key;
          ui_hover.m_callback = list_of_gui_func[key];
        }

        if ( is_selected )
          ImGui::SetItemDefaultFocus();
      }
    } );

    const char *current_unhover_item = ui_hover.m_key_to_func_unhover.c_str();
    ImGui::Text( "Unhover Key" );
    ImGuiHelper::DropDown( "##current_unhover_item", current_unhover_item, [&]
    {
      for ( const auto &[key, func] : list_of_gui_func )
      {
        bool is_selected = ( !std::strcmp( current_unhover_item, key.c_str() ) );
        if ( ImGui::Selectable( key.c_str(), is_selected ) )
        {
          ui_hover.m_key_to_func_unhover = key;
          ui_hover.m_callback_unhover = list_of_gui_func[key];
        }

        if ( is_selected )
          ImGui::SetItemDefaultFocus();
      }
    } );
  } );
}

template<>
inline void DrawComponent<GUI::UITrigger>( Ptr<byte> data,
                                           Entity entity,
                                           const Ptr<const ECS::Component::Descriptor> desc,
                                           const ECS::Signature &signature,
                                           Ptr<RM> resource_manager,
                                           Ptr<EntityContext> entity_context,
                                           Ptr<ECS::World::Instance> world,
                                           ImGuiTextFilter &filter,
                                           const ECS::Signature can_edit ) noexcept
{
  ShowProperties( data, desc, filter, [&]( rttr::instance &instance, std::string instance_name, const Ptr<const ECS::Component::Descriptor> desc )
  {
    auto &canvas_stack = Service::Locator::Get<GUI::Manager>();
    auto &list_of_gui_func = canvas_stack.GetFuncMap();

    auto &ui_trigger = *std::bit_cast<Ptr<GUI::UITrigger>>( data );
    const char *current_item = ui_trigger.m_key_to_func.c_str();

    ImGui::Text( "Trigger Key" );
    ImGuiHelper::DropDown( "##gui_func_trigger", current_item, [&]
    {
      for ( const auto &[name, func] : list_of_gui_func )
      {
        const char *selected_val = name.c_str();
        bool is_selected = ( current_item == selected_val );
        if ( ImGui::Selectable( selected_val, is_selected ) )
        {
          ui_trigger.m_key_to_func = selected_val;
          ui_trigger.m_callback = list_of_gui_func[selected_val];
        }

        if ( is_selected )
          ImGui::SetItemDefaultFocus();
      }
    } );
  } );
}

template <Traits::Concepts::IsTuple CTuple>
inline void ComponentSection<CTuple>::Draw( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                                            Entity entity,
                                            ECS::Signature signature,
                                            Ptr<RM> resource_manager,
                                            Ptr<EntityContext> entity_context,
                                            Ptr<ECS::World::Instance> world,
                                            ImGuiTextFilter &filter,
                                            const ECS::Signature can_edit ) noexcept
{
  [&] <typename... Cs>( tuplet::tuple<Cs...>* ) noexcept
  {
    ( [&]<typename C>( tuplet::tuple<C>* ) noexcept
    {
      auto &desc = ECS::Component::info<C>;
      ECS::Signature sig;
      sig.set( desc.m_bit_index );
      if ( ( signature & sig ).any() )
        DrawComponent<C>( cached_components[desc.m_id], entity, &desc, signature, resource_manager, entity_context, world, filter, can_edit );
    }( Traits::Tuple::null<tuplet::tuple<Cs>> ), ... );
  }( Traits::Tuple::null<CTuple> );
}

template <Traits::Concepts::InvokableFunction Func>
void AssignHeader( const char *name, Func func ) noexcept
{
  if ( ImGui::CollapsingHeader( name, ImGuiTreeNodeFlags_FramePadding & ImGuiTreeNodeFlags_AllowItemOverlap ) )
  {
    ImGuiHelper::Indent( [&]
    {
      func();
    } );
  }
}

template <Traits::Concepts::IsTuple Cs>
ECS::Signature SetSignature() noexcept
{
  ECS::Signature sig;
  [&] <typename... Cs>( tuplet::tuple<Cs...>* ) noexcept
  {
    ( [&]<typename C>( tuplet::tuple<C>* ) noexcept
    {
      sig.set( ECS::Component::info<C>.m_bit_index );
    }( Traits::Tuple::null<tuplet::tuple<Cs>> ), ... );
  }( Traits::Tuple::null<Cs> );

  return sig;
}
}
#endif