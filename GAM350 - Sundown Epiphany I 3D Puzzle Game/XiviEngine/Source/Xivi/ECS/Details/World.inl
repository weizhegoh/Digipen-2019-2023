/*****************************************************************
*\file         World.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::World
{
/*
* Constructor / Destructor
*/

Instance::Instance() noexcept : m_archetype_manager( *this )
{
  XV_CORE_INFO( "[World] Creating..." );
  XV_CORE_TRACE( "[World] Initialising descriptor manager..." );
  m_archetype_manager.m_events.m_on_new_archetype.Register<&System::Manager::OnNewArchetype>( m_system_manager );
  XV_CORE_TRACE( "[World] Created" );
}

Instance::~Instance() noexcept
{
  XV_CORE_INFO( "[World] Destroying..." );
  XV_CORE_TRACE( "[World] Destroying systems..." );
  m_system_manager.m_events.m_on_destroy.Notify();
  auto &c_desc_mgr = Component::DescriptorManager::Instance();
  for ( auto &[key, value] : m_singleton_map )
  {
    auto desc = c_desc_mgr.Get( key );
    if ( desc->m_destructor )
      desc->m_destructor( value.get() );
  }
  XV_CORE_TRACE( "[World] Destroyed" );
}

/*
* Runtime API
*/

void Instance::Initialise() noexcept
{
  RegisterComponents<XVECS_CList>();
}

void Instance::Start() noexcept
{
  if ( !m_running )
  {
    m_running = true;
    m_archetype_manager.UpdateStructuralChanges();
    m_system_manager.m_events.m_on_start.Notify();
  }
}

void Instance::Run() noexcept
{
  m_system_manager.Run();
}

void Instance::Stop() noexcept
{
  if ( m_running )
  {
    m_running = false;
    m_system_manager.m_events.m_on_end.Notify();
  }
}

void Instance::Pause() noexcept
{
  m_system_manager.m_events.m_on_pause.Notify();
}

void Instance::Resume() noexcept
{
  m_system_manager.m_events.m_on_resume.Notify();
}

void Instance::Clear() noexcept
{
  for ( auto &archetype : m_archetype_manager.m_archetypes )
  {
    auto family = &archetype->m_default_family;
    family->m_pool.Clear();
    family->m_next.reset();
  }
  m_archetype_manager.m_shared_component_map.clear();
  m_entity_manager.Initialise();
  auto &c_desc_mgr = Component::DescriptorManager::Instance();
  for ( auto &[key, value] : m_singleton_map )
  {
    auto desc = c_desc_mgr.Get( key );
    if ( desc->m_destructor )
      desc->m_destructor( value.get() );
  }
  m_singleton_map.clear();
}

/*
* Components API
*/

template <Component::Concepts::IsComponentList List>
void Instance::RegisterComponents() noexcept
{
  [&] <typename... Cs>( CList<Cs...> )
  {
    RegisterComponents<Cs...>();
  }
  ( List() );
}

template <Component::Concepts::IsComponent... Cs>
void Instance::RegisterComponents() noexcept
{
  if ( !Component::DescriptorManager::Instance().IsLocked() )
    ( ( Component::DescriptorManager::Instance().Register<Cs>() ), ... );
}

void Instance::LockComponents() noexcept
{
  Component::DescriptorManager::Instance().Lock();
}

/*
* Systems API
*/

template <System::Concepts::IsSystemList List>
void Instance::RegisterSystems() noexcept
{
  [&] <typename... TSystems>( SList<TSystems...> )
  {
    RegisterSystems<TSystems...>();
  }
  ( List() );
}

template <System::Concepts::IsSystem... TSystems>
void Instance::RegisterSystems() noexcept
{
  if ( !Component::DescriptorManager::Instance().IsLocked() )
    LockComponents();
  ( ( m_system_manager.Register<TSystems>( *this ) ), ... );
}

template <System::Concepts::IsSystem TSystem>
TSystem &Instance::GetSystem() noexcept
{
  return m_system_manager.Get<TSystem>();
}

/*
* Events API
*/

template <Xivi::Event::Concepts::IsEventList List>
void Instance::RegisterGlobalEvents() noexcept
{
  [&] <typename... TEvents>( EList<TEvents...> )
  {
    RegisterGlobalEvents<TEvents...>();
  }
  ( List() );
}

template <Xivi::Event::Concepts::IsEvent... TEvents>
void Instance::RegisterGlobalEvents() noexcept
{
  if ( !Component::DescriptorManager::Instance().IsLocked() )
    LockComponents();
  ( ( m_event_manager.Register<TEvents>() ), ... );
}

template <Xivi::Event::Concepts::IsEvent TEvent>
TEvent &Instance::GetGlobalEvent() noexcept
{
  return m_event_manager.Get<TEvent>();
}

template <Xivi::Event::Concepts::IsEvent TEvent, typename... Args>
void Instance::SendGlobalEvent( Args &&...args ) noexcept
{
  m_event_manager.Get<TEvent>().Notify( std::forward<Args &&>( args )... );
}

/*
* Entities API
*/
bool Instance::IsEntityZombie( const Entity &entity ) const noexcept
{
  return m_entity_manager.m_entities[entity.Index()].Zombie();
}

template <Component::Concepts::IsComponent... Cs>
bool Instance::HaveComponents( const Entity &entity ) const noexcept
{
  auto &archetype = GetArchetype( entity );
  return archetype.HaveAllComponents<Cs...>();
}

bool Instance::HaveComponents( const Entity &entity, DescriptorSpan descriptors ) const noexcept
{
  auto &archetype = GetArchetype( entity );
  return archetype.HaveAllComponents( descriptors );
}

template <Component::Concepts::IsComponent C>
bool Instance::HaveComponent( const Entity &entity ) const noexcept
{
  auto &archetype = GetArchetype( entity );
  return archetype.HaveComponent<C>();
}

bool Instance::HaveComponent( const Entity &entity, Ptr<const Component::Descriptor> descriptor ) const noexcept
{
  auto &archetype = GetArchetype( entity );
  return archetype.HaveComponent( *descriptor );
}

bool Instance::HaveComponent( const Entity &entity, const Component::Descriptor &descriptor ) const noexcept
{
  auto &archetype = GetArchetype( entity );
  return archetype.HaveComponent( descriptor );
}

template <Component::Concepts::IsComponent C>
C &Instance::GetComponent( const Entity &entity ) noexcept
{
  auto &entry = m_entity_manager.Get( entity );
  XV_CORE_ASSERT_MSG( entry.m_archetype->HaveComponent<C>(),
                      "[World] Entity does not have {} component", Component::info<C>.m_name );
  auto &pool = entry.m_pool;
  auto &index = entry.m_pool_index;
  return pool->GetComponent<C>( index );
}

Ptr<byte> Instance::GetComponent( const Entity &entity, Ptr<const Component::Descriptor> descriptor ) noexcept
{
  auto &entry = m_entity_manager.Get( entity );
  XV_CORE_ASSERT_MSG( entry.m_archetype->HaveComponent( *descriptor ), "[World] Entity does not have {} component", descriptor->m_name );
  auto &pool = entry.m_pool;
  auto &index = entry.m_pool_index;
  return pool->GetComponent( index, *descriptor );
}

Ptr<byte> Instance::GetComponent( const Entity &entity, const Component::Descriptor &descriptor ) noexcept
{
  auto &entry = m_entity_manager.Get( entity );
  XV_CORE_ASSERT_MSG( entry.m_archetype->HaveComponent( descriptor ), "[World] Entity does not have {} component", descriptor.m_name );
  auto &pool = entry.m_pool;
  auto &index = entry.m_pool_index;
  return pool->GetComponent( index, descriptor );
}

template <Component::Concepts::IsComponent... Cs>
tuplet::tuple<Cs&...> Instance::GetComponents( const Entity &entity ) noexcept
{
  auto &entry = m_entity_manager.Get( entity );
  XV_CORE_ASSERT_MSG( entry.m_archetype->HaveAllComponents<Cs...>(), "[World] Entity does not contain all components" );
  auto &pool = entry.m_pool;
  auto &index = entry.m_pool_index;

  return pool->GetComponents<Cs...>( index );
}

vector<Ptr<byte>> Instance::GetComponents( const Entity &entity, DescriptorSpan descriptors ) noexcept
{
  auto &entry = m_entity_manager.Get( entity );
  XV_CORE_ASSERT_MSG( entry.m_archetype->HaveAllComponents( descriptors ), "[World] Entity does not contain all components" );
  auto &pool = entry.m_pool;
  auto &index = entry.m_pool_index;

  Signature signature;
  for ( const auto &desc : descriptors )
    signature.set( desc->m_bit_index );

  return pool->GetComponents( index, signature );
}

template <ECS::Concepts::SetterCallback Func>
bool Instance::GetEntity( const Entity &entity, Func &&func ) noexcept
{
  if ( entity.Zombie() || !entity.Valid() )
    return false;

  auto &entry = m_entity_manager.m_entities[entity.Index()];
  if ( !entry.Zombie() && entry.m_validation == entity.m_validation )
  {
    if constexpr ( !std::is_same_v<Func, EmptyLambda> )
      [&] <Component::Concepts::IsComponent... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
    {
      func( entry.m_pool->GetComponent<Cs>( entry.m_pool_index )... );
    }( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );
    return true;
  }
  return false;
}

void Instance::DestroyEntity( Entity &entity ) noexcept
{
  XV_CORE_ASSERT_MSG( !entity.Zombie(), "[World] Entity is zombie" );
  auto &entry = m_entity_manager.Get( entity );
  XV_CORE_ASSERT_MSG( entry.m_validation == entity.m_validation, "[World] Entity validation does not match" );
  entry.m_archetype->DestroyEntity( entity );
}

template <Xivi::ECS::Concepts::SetterCallback Func>
Entity Instance::AddOrRemoveComponents( Entity entity, DescriptorSpan add, DescriptorSpan remove, Func &&func ) noexcept
{
  Signature add_signature;
  Signature remove_signature;

  for ( const auto &desc : add )
    add_signature.set( desc->m_bit_index );
  for ( const auto &desc : remove )
    remove_signature.set( desc->m_bit_index );

  if constexpr ( std::is_same_v<Func, EmptyLambda> )
    return m_archetype_manager.AddOrRemoveComponents( entity, add_signature, remove_signature );
  else
    return m_archetype_manager.AddOrRemoveComponents( entity, add_signature, remove_signature, std::forward<Func &&>( func ) );
}

template <Xivi::ECS::Concepts::SetterCallback Func>
Entity Instance::AddOrRemoveComponents( Entity entity, const Signature &add, const Signature &remove, Func &&func ) noexcept
{
  if constexpr ( std::is_same_v<Func, EmptyLambda> )
    return m_archetype_manager.AddOrRemoveComponents( entity, add, remove );
  else
    return m_archetype_manager.AddOrRemoveComponents( entity, add, remove, std::forward<Func &&>( func ) );
}

template <IsTypelist AddList, IsTypelist RemoveList, Xivi::ECS::Concepts::SetterCallback Func>
Entity Instance::AddOrRemoveComponents( Entity entity, Func &&func ) noexcept
{
  Signature add_signature;
  Signature remove_signature;
  if constexpr ( !std::is_same_v<Typelist<>, AddList> )
    [&] <Component::Concepts::IsComponent... Cs>( Typelist<Cs...> )
  {
    ( add_signature.set( Component::info<Cs>.m_bit_index ), ... );
  }
  ( AddList() );

  if constexpr ( !std::is_same_v<Typelist<>, RemoveList> )
    [&] <Component::Concepts::IsComponent... Cs>( Typelist<Cs...> )
  {
    ( remove_signature.set( Component::info<Cs>.m_bit_index ), ... );
  }
  ( RemoveList() );

  if constexpr ( std::is_same_v<Func, EmptyLambda> )
    return m_archetype_manager.AddOrRemoveComponents( entity, add_signature, remove_signature );
  else
    return m_archetype_manager.AddOrRemoveComponents( entity, add_signature, remove_signature, std::forward<Func &&>( func ) );
}

  /*
  * Singleton Entity API
  */

template <Component::Concepts::SingletonComponent C>
C &Instance::GetOrCreateSingletonComponent() noexcept
{
  if ( m_singleton_map.find( Component::info<C>.m_id ) == m_singleton_map.end() )
  {
    m_singleton_map[Component::info<C>.m_id] = MakeUniqueOverwrite<byte[]>( Component::info<C>.m_size );
    if constexpr ( Component::info<C>.m_constructor )
      Component::info<C>.m_constructor( m_singleton_map[Component::info<C>.m_id].get() );
  }
  return *std::bit_cast<Ptr<C>>( m_singleton_map[Component::info<C>.m_id].get() );
}

/*
* Archetypes API
*/

Archetype::Instance &Instance::GetOrCreateArchetype( DescriptorSpan descriptors, const std::string &str ) noexcept
{
  Signature signature {};
  for ( const auto &desc : descriptors )
    signature.set( desc->m_bit_index );

  signature.set( Component::info<Entity>.m_bit_index );

  return m_archetype_manager.GetOrCreate( signature, str );
}

template <Component::Concepts::IsComponentList List>
Archetype::Instance &Instance::GetOrCreateArchetype( const std::string &str ) noexcept
{
  return[&]<typename... Cs>( Typelist<Cs...> ) -> Archetype::Instance &
  {
    return GetOrCreateArchetype<Cs...>( str );
  }( List() );
}

template <Component::Concepts::IsComponent... Cs>
Archetype::Instance &Instance::GetOrCreateArchetype( const std::string &str ) noexcept
{
  Signature signature {};
  ( signature.set( Component::info<Cs>.m_bit_index ), ... );
  signature.set( Component::info<Entity>.m_bit_index );
  return m_archetype_manager.GetOrCreate( signature, str );
}

Archetype::Instance &Instance::GetArchetype( Archetype::ID id ) noexcept
{
  return m_archetype_manager.Get( id );
}

Archetype::Instance &Instance::GetArchetype( const std::string &str ) noexcept
{
  return m_archetype_manager.Get( str );
}

Archetype::Instance &Instance::GetArchetype( const Entity &entity ) const noexcept
{
  XV_CORE_ASSERT_MSG( !entity.Zombie(), "[World] Entity is zombie" );
  auto &entry = m_entity_manager.Get( entity );
  XV_CORE_ASSERT_MSG( entry.m_validation == entity.m_validation, "[World] Entity validation does not match" );
  return *entry.m_archetype;
}

/*
* Systems API
*/

PinnedVector<Ptr<Archetype::Instance>> Instance::Search( const Query::Instance &query ) const noexcept
{
  PinnedVector<Ptr<Archetype::Instance>> result;
  for ( const auto &[i, pair_signature] : Enumerate( m_archetype_manager.m_archetypes_bits ) )
  {
    const auto &[signature, exclusive] = pair_signature;
    if ( query.Compare( signature, exclusive ) )
      result.push_back( m_archetype_manager.m_archetypes[i].get() );
  }
  return result;
}

template <ECS::Concepts::SystemCallback Func>
bool Instance::ForEach( Func &&func ) noexcept
{
  Query::Instance query;
  query.SetQueryFromFunction<Func>();
  return ForEach( Search( query ), std::forward<Func &&>( func ) );
}

template <ECS::Concepts::SystemCallback Func>
bool Instance::ForEach( Query::Instance query, Func &&func ) noexcept
{
  if constexpr ( Traits::Concepts::InvokableFunction<Func> )
    query.SetQueryFromFunction<Func>();
  return ForEach( Search( query ), std::forward<Func &&>( func ) );
}

template <ECS::Concepts::SystemCallback Func>
bool Instance::ForEach( std::span<const Ptr<Archetype::Instance>> archetypes, Func &&func ) noexcept
{
  Query::Iterator<Func> iterator( *this );

  for ( const auto &archetype : archetypes )
  {
    iterator.UpdateArchetype( *archetype );
    for ( auto family = &archetype->DefaultFamily(); family; family = family->m_next.get() )
    {
      iterator.UpdateFamily( *family );
      if ( !family->m_pool.Size() )
        continue;
      iterator.UpdatePool( family->m_pool );
      if constexpr ( Traits::Concepts::FuncReturnType<Func, bool> )
      {
        if ( iterator.ForEach( std::forward<Func &&>( func ) ) )
          return true;
      }
      else
        iterator.ForEach( std::forward<Func &&>( func ) );
    }
  }
  return false;
}

/*
* Serialisation / Deserialisation API
*/

void Instance::SerialiseWorld( const std::string &filename )
{
  XV_CORE_INFO( "[World] Serialising into {}...", filename );

  m_archetype_manager.UpdateStructuralChanges();

  auto &c_desc_mgr = Component::DescriptorManager::Instance();

  JsonFile file;
  file.StartWriter( filename ).StartObject();

  XV_CORE_TRACE( "[World] Serialising component descriptors..." );
  file.WriteKey( "ComponentDescriptors" ).StartArray();
  for ( auto i = 0u; i < c_desc_mgr.m_types_count; ++i )
  {
    auto desc_ = c_desc_mgr.m_descriptor_array[i];
    file.StartObject();
    file.WriteKey( "ID" ).WriteU64( desc_->m_id.m_value );
    file.WriteKey( "Name" ).WriteString( std::string( desc_->m_name ) );
    file.WriteKey( "Type" ).WriteString( desc_->m_type == ECS::Component::Type::ID::Data ? "Data" :
                                         desc_->m_type == ECS::Component::Type::ID::Shared ? "Shared" :
                                         desc_->m_type == ECS::Component::Type::ID::Tag ? "Tag" :
                                         desc_->m_type == ECS::Component::Type::ID::Exclusive ? "Exclusive" : "Singleton" );
    file.EndObject();
  }
  file.EndArray();

  file.WriteKey( "EntitiesCount" ).WriteU64( m_entity_manager.m_count );
  file.WriteKey( "EntitiesBoundaries" ).WriteU64( m_entity_manager.m_max );

  XV_CORE_TRACE( "[World] Serialising entities descriptor..." );
  file.WriteKey( "EntitiesDescriptor" ).StartArray();
  for ( const auto &[i, desc_] : Enumerate( m_entity_manager.m_entities ) )
  {
    if ( i > m_entity_manager.m_max )
      break;
    file.StartObject().WriteObject( desc_ ).EndObject();
  }
  file.EndArray(); // EntityDescriptor End

  // Singleton Component
  if ( m_singleton_map.size() )
  {
    XV_CORE_TRACE( "[World] Serialising singleton components..." );
    file.WriteKey( "Singletons" ).StartObject();
    file.WriteKey( "Total" ).WriteU64( m_singleton_map.size() );
    file.WriteKey( "List" ).StartArray();
    for ( auto &[key, value] : m_singleton_map )
    {
      file.StartObject().WriteKey( "ID" ).WriteU64( key.m_value );
      auto desc_ = c_desc_mgr.Get( key );
      file.WriteKey( "Name" ).WriteString( std::string( desc_->m_name ) );
      file.WriteKey( "Type" ).WriteString( "Singleton" );
      auto obj = desc_->m_reflect( value.get() );
      file.WriteKey( "Data" ).StartObject().WriteObject( obj ).EndObject();
      file.EndObject();
    }
    file.EndArray().EndObject();
  }

  // Archetypes
  XV_CORE_TRACE( "[World] Serialising archetypes..." );
  file.WriteKey( "Archetypes" ).StartObject();
  file.WriteKey( "Total" ).WriteU64( m_archetype_manager.m_archetypes.size() );
  file.WriteKey( "List" ).StartArray();
  for ( const auto &[i, archetype] : Enumerate( m_archetype_manager.m_archetypes ) )
  {
    auto family_count = 0u;
    for ( auto ptr_f = &archetype->DefaultFamily(); ptr_f; ptr_f = ptr_f->m_next.get() )
      ++family_count;

    std::string archetype_name = archetype->m_name;
    XV_CORE_TRACE( "[World] Serialising archetype's details..." );
    file.StartObject();
    // Archetype Name
    file.WriteKey( "Name" ).WriteString( archetype_name );
    // Archetype Details
    file.WriteKey( "Details" ).StartObject();
    file.WriteKey( "ID" ).WriteU64( archetype->m_id.m_value );
    file.WriteKey( "TotalFamily" ).WriteU64( family_count );
    file.WriteKey( "TotalDataComponents" ).WriteU64( archetype->m_data_component_count );
    file.WriteKey( "TotalSharedComponents" ).WriteU64( archetype->m_shared_component_count );
    file.WriteKey( "TotalTagComponents" ).WriteU64( archetype->m_tag_component_count );
    file.EndObject();

    XV_CORE_TRACE( "[World] Serialising archetype's component descriptors..." );
    file.WriteKey( "ComponentDescriptors" ).StartArray();
    for ( const auto &desc_ : archetype->m_descriptors )
    {
      file.StartObject();
      file.WriteKey( "ID" ).WriteU64( desc_->m_id.m_value );
      file.WriteKey( "Name" ).WriteString( std::string( desc_->m_name ) );
      file.WriteKey( "Type" ).WriteString( desc_->m_type == ECS::Component::Type::ID::Data ? "Data" :
                                           desc_->m_type == ECS::Component::Type::ID::Shared ? "Shared" :
                                           desc_->m_type == ECS::Component::Type::ID::Tag ? "Tag" : "Exclusive" );
      file.EndObject();
    }
    file.EndArray(); // Descriptors End

    XV_CORE_TRACE( "[World] Serialising archetype's families..." );
    file.WriteKey( "Families" ).StartArray();
    auto fam = &archetype->DefaultFamily();
    for ( auto k = 0u; k != family_count; ++k, fam = fam->m_next.get() )
    {
      XV_CORE_TRACE( "[World] Serialising family's details..." );
      file.StartObject();
      file.WriteKey( "Details" ).StartObject();
      file.WriteKey( "ID" ).WriteU64( fam->m_id.m_value );
      file.WriteKey( "TotalEntities" ).WriteI32( fam->m_pool.m_size );
      file.EndObject();

      XV_CORE_TRACE( "[World] Serialising family's shared components details..." );
      if ( archetype->m_shared_component_count )
      {
        file.WriteKey( "SharedDetails" ).StartArray();
        for ( auto j = 0u; j < archetype->m_shared_component_count; ++j )
        {
          file.StartObject();
          file.WriteKey( "Entity" ).WriteU64( std::bit_cast<u64>( fam->m_shared_details[j].m_entity ) );
          file.WriteKey( "SharedKey" ).WriteU64( fam->m_shared_details[j].m_key.m_value );
          file.EndObject();
        }
        file.EndArray(); // Shared Details End
      }

      XV_CORE_TRACE( "[World] Serialising family's pool..." );
      auto pool = &fam->m_pool;
      file.WriteKey( "Pool" ).StartArray();
      for ( auto count = 0, last = pool->m_size; count != last; ++count )
      {
        const auto &entity = pool->GetComponent<Entity>( { count } );
        std::string key = m_entity_manager.Get( entity ).m_name;
        file.StartObject();
        file.WriteKey( "EntityName" ).WriteString( key );
        file.WriteKey( "Components" ).StartArray(); // Entity Details
        for ( const auto &[j, desc_] : Enumerate( pool->m_descriptors ) )
        {
          auto obj = desc_->m_reflect( &pool->m_components[j][desc_->m_size * count] );
          file.StartObject();
          file.WriteKey( obj.get_type().get_name().to_string() ).StartObject().WriteObject( obj ).EndObject();
          file.EndObject();
        }
        file.EndArray().EndObject();; // Entity End
      }
      file.EndArray().EndObject(); // Pool End
    }
    file.EndArray().EndObject(); // Family End
  }
  file.EndArray().EndObject(); // Archetype End
  file.EndObject().EndWriter();
  XV_CORE_INFO( "[World] Serialised" );
}

void Instance::DeserialiseWorld( const std::string &filename )
{
  XV_CORE_INFO( "[World] Deserialising from {}...", filename );
  Clear();
  JsonFile file;
  file.StartReader( filename );

  auto m_max_itr = file.FindMember( "EntitiesBoundaries" );
  m_entity_manager.m_max = m_max_itr->value.GetUint();

  XV_CORE_TRACE( "[World] Deserialising entities descriptor..." );
  auto m_itr = file.FindMember( "EntitiesDescriptor" );
  auto count = 0u;
  for ( auto v_itr = m_itr->value.Begin(); v_itr != m_itr->value.End(); ++v_itr )
    Deserialise::ReadRecursive( m_entity_manager.m_entities[count++], *v_itr );

  if ( file.HasMember( "Singletons" ) )
  {
    XV_CORE_TRACE( "[World] Deserialising singletons..." );
    auto singleton_head = file.FindMember( "Singletons" );
    auto singleton_itr = singleton_head->value.FindMember( "List" );
    for ( auto it = singleton_itr->value.Begin(); it != singleton_itr->value.End(); ++it )
    {
      auto id = it->FindMember( "ID" )->value.GetUint64();
      auto &c_desc_mgr = Component::DescriptorManager::Instance();
      auto desc = c_desc_mgr.Get( id );
      m_singleton_map[id] = MakeUniqueOverwrite<byte[]>( desc->m_size );
      if ( desc->m_constructor )
        desc->m_constructor( m_singleton_map[id].get() );
      auto obj = desc->m_reflect( m_singleton_map[id].get() );
      auto value_itr = it->FindMember( "Data" );
      Deserialise::ReadRecursive( obj, value_itr->value );
    }
  }

  XV_CORE_TRACE( "[World] Deserialising archetypes..." );
  auto arch_head = file.FindMember( "Archetypes" );
  // u32 archetype_count = arch_head->value.FindMember( "Total" )->value.Get<u32>();

  auto arch_itr = arch_head->value.FindMember( "List" );

  for ( auto it = arch_itr->value.Begin(); it != arch_itr->value.End(); ++it )
  {
    auto arch_name = it->FindMember( "Name" )->value.GetString();
    // For Handling Archetype Naming Later
    XV_CORE_TRACE( "[World] Archetype name - {}...", arch_name );

    /*auto arch_details =  it->FindMember( "Details" );
    auto arch_details_id = arch_details->value.FindMember( "ID" )->value.GetUint64();
    XV_CORE_INFO( "Archetype ID - {}", arch_details_id);*/

    XV_CORE_TRACE( "[World] Deserialising archetype's component descriptors..." );
    std::vector<Ptr<const Component::Descriptor>> descriptor_vec;
    auto descriptors_itr = it->FindMember( "ComponentDescriptors" );
    for ( auto desc_it = descriptors_itr->value.Begin(); desc_it != descriptors_itr->value.End(); ++desc_it )
    {
      auto id = desc_it->FindMember( "ID" )->value.GetUint64();
      auto desc = Component::DescriptorManager::Instance().Get( { id } );
      if ( !desc )
        XV_CORE_ASSERT_MSG( false, "[World] Incorrect serialisation error" );
      descriptor_vec.push_back( desc );
    }

    XV_CORE_TRACE( "[World] Creating archetype..." );
    auto &archetype = GetOrCreateArchetype( { descriptor_vec }, arch_name );

    std::array<Entity, Settings::max_shared_components_per_entity> shared_entities {};
    std::array<Component::Type::Shared::Key, Settings::max_shared_components_per_entity> shared_keys {};

    XV_CORE_TRACE( "[World] Deserialising archetype's families..." );
    auto families_itr = it->FindMember( "Families" );
    for ( auto fam_itr = families_itr->value.Begin(); fam_itr != families_itr->value.End(); ++fam_itr )
    {
      auto &fam_details_itr = fam_itr->FindMember( "Details" )->value;
      Pool::Family::ID fam_id = fam_details_itr.FindMember( "ID" )->value.GetUint64();
      auto entities_count = fam_details_itr.FindMember( "TotalEntities" )->value.GetUint64();

      if ( !fam_id.Valid() )
        continue;

       // Skip Shared First
       /*    auto shared_details_itr = fam_itr->FindMember( "SharedDetails" );
       if (shared_details_itr->value.IsNull() )
       for ( auto shared_itr = shared_details_itr->value.Begin(); shared_itr != shared_details_itr->value.End(); ++shared_itr )
       {
       }*/

      XV_CORE_TRACE( "[World] Creating archetype's family ID - {}...", fam_id.m_value );
      auto &fam = archetype.CreateFamily( fam_id,
                                          { shared_entities.data(), archetype.m_shared_component_count },
                                          { shared_keys.data(),archetype.m_shared_component_count } );

     // Double Check multi insert / duplication
      XV_CORE_TRACE( "[World] Mapping family's shared components..." );
      if ( archetype.m_shared_component_count )
        for ( auto i = 0u; i < archetype.m_shared_component_count; ++i )
        {
          auto &detail = fam.m_shared_details[i];
          m_archetype_manager.m_shared_component_map.emplace( std::pair { detail.m_key, detail.m_entity } );
        }

      XV_CORE_TRACE( "[World] Deserialising family's pool..." );
      auto &pool = fam.m_pool;
      if ( entities_count > 0 )
      {
        XV_CORE_TRACE( "[World] Appending {} entities...", entities_count );
        pool.Append( static_cast<u32>( entities_count ) );

        XV_CORE_TRACE( "[World] Deserialising pool's entity's component...", entities_count );
        auto pool_itr = fam_itr->FindMember( "Pool" );
        auto curr_ent = 0;
        for ( auto entity_itr = pool_itr->value.Begin(); entity_itr != pool_itr->value.End(); ++entity_itr, ++curr_ent )
        {
          std::string entity_name = entity_itr->FindMember( "EntityName" )->value.GetString(); // For reading
          auto components_itr = entity_itr->FindMember( "Components" );
          auto curr_com = 0;
          for ( auto com_itr = components_itr->value.Begin(); com_itr != components_itr->value.End(); ++com_itr, ++curr_com )
            for ( auto value_itr = com_itr->MemberBegin(); value_itr != com_itr->MemberEnd(); ++value_itr )
            {
              auto desc = pool.m_descriptors[curr_com];
              auto instance = desc->m_reflect( &pool.m_components[curr_com][desc->m_size * curr_ent] );
              Deserialise::ReadRecursive( instance, value_itr->value );
            }
        }
        for ( u32 i = 0; i < entities_count; ++i )
        {
          auto &entity = *std::bit_cast<Entity *>( &pool.m_components[0][i * pool.m_descriptors[0]->m_size] );
          auto &entry = m_entity_manager.m_entities[entity.Index()];

          XV_CORE_ASSERT_MSG( entry.m_validation == entity.m_validation, "[World] Entity descriptor and entity mismatch" );

          entry.m_archetype = &archetype;
          entry.m_pool = &pool;
          entry.m_pool_index = Pool::Index { static_cast<i32>( i ) };
          ++m_entity_manager.m_count;
        }
        m_archetype_manager.AddToStructuralPendingList( pool );
      }
    }
  }
  file.EndReader();

  XV_CORE_TRACE( "[World] Linking entities descriptors..." );
  m_entity_manager.m_head = -1;
  auto entity_span = std::span { m_entity_manager.m_entities.data(), Settings::max_entities };
  for ( auto it = entity_span.rbegin(); it != entity_span.rend(); ++it )
  {
    auto &entry = *it;
    if ( entry.m_pool == nullptr )
    {
      entry.m_pool_index.m_value = m_entity_manager.m_head;
      m_entity_manager.m_head = static_cast<i32>( static_cast<u64>( &entry - m_entity_manager.m_entities.data() ) );
    }
  }

  m_archetype_manager.UpdateStructuralChanges();
  XV_CORE_INFO( "[World] Deserialised" );
}
}