/*****************************************************************
*\file         QueryIterator.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::Query::Details
{
template <Traits::Concepts::InvokableFunction Func>
DataPack<Func, Mode::DataAndShared>::DataPack() noexcept
  : m_signature { [&] <typename... Cs>( tuplet::tuple<Cs...>* )  constexpr noexcept
{
  Signature signature;
  ( ( [&]<typename C>( tuplet::tuple<C>* ) constexpr noexcept
  {
    if constexpr ( !std::is_const_v<C> )
      signature.set( Component::info<Raw<C>>.m_bit_index );
  }( Traits::Tuple::null<Cs> ) ), ... );
  return signature;
}( Traits::Tuple::null<SharedTupleUnfiltered> )
}
{}
}

namespace Xivi::ECS::Query
{
template <Traits::Concepts::InvokableFunction Func, Mode TMode>
Iterator<Func, TMode>::Iterator( World::Instance &world ) noexcept
{
  if constexpr ( mode == Mode::DataAndShared )
  {
    Pack::m_world = &world;

    [&] <typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
    {
      ( ( [&]<typename C>( tuplet::tuple<C>* ) constexpr noexcept
      {
        Pack::m_cached_shared_pointers[Traits::Tuple::Index<C, Pack::SharedTupleUnfiltered>].m_value = 0;
      }( Traits::Tuple::null<Cs> ) ), ... );
    }( Traits::Tuple::null<Pack::SharedTupleUnfiltered> );
  }
}

template <Traits::Concepts::InvokableFunction Func, Mode TMode>
void Iterator<Func, TMode>::UpdateArchetype( Archetype::Instance &archetype ) noexcept
{
  Pack::m_archetype = &archetype;

  if constexpr ( mode == Mode::DataAndShared )
  {
    Pack::m_shared_signature = archetype.GetSignature() & Component::DescriptorManager::Instance().m_shared_bits;

    [&] <typename... Cs>( tuplet::tuple<Cs...> * ) constexpr noexcept
    {
      ( ( [&]<typename C>( tuplet::tuple<C>* )constexpr noexcept
      {
        if constexpr ( std::is_pointer_v<C> )
        {
          Pack::m_map_index[Traits::Tuple::Index<C, Pack::SharedTupleUnfiltered>] =
            ( Pack::m_shared_signature[Component::info<C>.m_bit_index] ) ?
            GetComponentIndexFromSignature( Pack::m_shared_signature, Component::info<C>.m_bit_index )
            : -1;
        }
        else
        {
          XV_CORE_ASSERT_MSG( Pack::m_shared_signature[Component::info<C>.m_bit_index], "[QueryIterator] Shared component does not exist in this pack" );
          Pack::m_map_index[Traits::Tuple::Index<C, Pack::SharedTupleUnfiltered>] =
            GetComponentIndexFromSignature( Pack::m_shared_signature, Component::info<C>.m_bit_index );
        }
      } ( Traits::Tuple::null<Cs> ) ), ... );
    }( Traits::Tuple::null<Pack::SharedTupleUnfiltered> );
  }
}

template <Traits::Concepts::InvokableFunction Func, Mode TMode>
void Iterator<Func, TMode>::UpdateFamily( Pool::Family::Instance &family ) noexcept
{
  if constexpr ( mode == Mode::DataAndShared )
  {
    Pack::m_family = &family;

    [&] <typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
    {
      ( ( [&]<typename C>( tuplet::tuple<C>* ) constexpr noexcept
      {
        const auto index = Traits::Tuple::Index<C, Pack::SharedTupleUnfiltered>;
        const auto mapped_index = Pack::m_map_index[index];
        const auto &shared_details = family.m_shared_details[mapped_index];

        if ( Pack::m_cached_keys[index] != shared_details.m_key )
        {
          using U_C = Pack::template UniversalC<C>;
          using P_C = Raw<C> *;

          auto &entry = Pack::m_world->m_entity_manager.Get( shared_details.m_entity );
          const auto component_index = entry.m_pool->GetComponentIndex( *family.m_shared_descriptors[mapped_index] );

          if constexpr ( std::is_pointer_v<C> )
          {
            if ( component_index != -1 )
            {
              if constexpr ( std::is_pointer_v<U_C> )
                tuplet::get<U_C>( Pack::m_shared_tuple ) = &std::bit_cast<U_C>( entry.m_pool->m_components[component_index] )[entry.m_pool_index.m_value];
              else
              {
                tuplet::get<P_C>( Pack::m_cached_shared_pointers ) = &std::bit_cast<P_C>( entry.m_pool->m_components[component_index] )[entry.m_pool_index.m_value];
                tuplet::get<U_C>( Pack::m_shared_tuple ) = *tuplet::get<P_C>( Pack::m_cached_shared_pointers );
              }
            }
          }
          else
          {
            XV_CORE_ASSERT_MSG( component_index != -1, "[QueryIterator] Invalid component index" );
            if constexpr ( std::is_pointer_v<U_C> )
              tuplet::get<U_C>( Pack::m_shared_tuple ) = &std::bit_cast<U_C>( entry.m_pool->m_components[component_index] )[entry.m_pool_index.m_value];
            else
            {
              tuplet::get<P_C>( Pack::m_cached_shared_pointers ) = &std::bit_cast<P_C>( entry.m_pool->m_components[component_index] )[entry.m_pool_index.m_value];
              tuplet::get<U_C>( Pack::m_shared_tuple ) = *tuplet::get<P_C>( Pack::m_cached_shared_pointers );
            }
          }
          Pack::m_cached_keys[index] = family.m_shared_details[mapped_index];
        }
      }( Traits::Tuple::null<tuplet::tuple<Cs>> ) ), ... );
    }( Traits::Tuple::null<Pack::SharedTupleUnfiltered> );
  }
}

template <Traits::Concepts::InvokableFunction Func, Mode TMode>
void Iterator<Func, TMode>::UpdatePool( Pool::Instance &pool ) noexcept
{
  Pack::m_pool = &pool;

  [&] <typename... Cs>( tuplet::tuple<Cs...> * ) constexpr noexcept
  {
    ( ( [&]<typename C>( tuplet::tuple<C>* )
    {
      using U_C = Pack::template UniversalC<C>;
      if constexpr ( std::is_pointer_v<C> )
      {
        if ( Pack::m_archetype->GetSignature()[Component::info<U_C>.m_bit_index] )
        {
          const auto index = GetComponentIndexFromSignature( Pack::m_archetype->GetSignature(), Component::info<U_C>.m_bit_index );
          tuplet::get<Traits::Tuple::Index<U_C, decltype( Pack::m_data_tuple )>>( Pack::m_data_tuple ) = std::bit_cast<U_C>( pool.m_components[index] );
        }
        else
          tuplet::get<Traits::Tuple::Index<U_C, decltype( Pack::m_data_tuple )>>( Pack::m_data_tuple ) = nullptr;
      }
      else
      {
        XV_CORE_ASSERT_MSG( Pack::m_archetype->GetSignature()[Component::info<U_C>.m_bit_index], "[QueryIterator] Component does not exist in this pack" );
        const auto index = GetComponentIndexFromSignature( Pack::m_archetype->GetSignature(), Component::info<U_C>.m_bit_index );
        tuplet::get<Traits::Tuple::Index<U_C, decltype( Pack::m_data_tuple )>>( Pack::m_data_tuple ) = std::bit_cast<U_C>( pool.m_components[index] );
      }
    }( Traits::Tuple::null<tuplet::tuple<Cs>> ) ), ... );
  }( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );
}

template <Traits::Concepts::InvokableFunction Func, Mode TMode>
Iterator<Func, TMode>::ReturnType  Iterator<Func, TMode>::ForEach( Func &&func ) noexcept
{
  XV_CORE_ASSERT_MSG( Pack::m_pool->Size(), "[QueryIterator] Pool is empty" );

  if constexpr ( mode == Mode::DataAndShared )
  {
    bool break_loop = false;

    auto param = [&]<typename C>( tuplet::tuple<C>* ) constexpr noexcept -> C
    {
      using U_C = Pack::template UniversalC<C>;
      if constexpr ( Component::info<C>.m_type == Component::Type::ID::Shared )
      {
        if constexpr ( std::is_pointer_v<C> )
        {
          if constexpr ( std::is_pointer_v<U_C> ) return tuplet::get<U_C>( Pack::m_shared_tuple );
          else return&tuplet::get<U_C>( Pack::m_shared_tuple );
        }
        else
        {
          if constexpr ( std::is_pointer_v<U_C> ) return *tuplet::get<U_C>( Pack::m_shared_tuple );
          else return tuplet::get<U_C>( Pack::m_shared_tuple );
        }
      }
      else
      {
        auto &mptr = tuplet::get<U_C>( Pack::m_data_tuple );
        if constexpr ( std::is_pointer_v<C> )
          if ( !mptr ) return std::bit_cast<C>( nullptr );

        auto ptr = mptr;
        ++mptr;

        if constexpr ( std::is_pointer_v<C> ) return std::bit_cast<C>( ptr );
        else return std::bit_cast<C>( *ptr );
      }
    };

    for ( i32 i = 0; i < Pack::m_pool->Size(); ++i )
    {
      if constexpr ( std::is_same_v<bool, ReturnType> )
      {
        break_loop = [&]<typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
        {
          return func( param( Traits::Tuple::null<tuplet::tuple<Cs>> )... );
        }( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );
      }
      else
      {
        [&] <typename... Cs>( tuplet::tuple<Cs...> * ) constexpr noexcept
        {
          func( param( Traits::Tuple::null<tuplet::tuple<Cs>> )... );
        }( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );
      }

      [&]<typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
      {
        array<Component::Type::Shared::Key, Pack::nonconst_shared_count> updated_keys;
        u64 updated_key_id {};
        i32 new_entities_count { 0 };

        ( ( [&]<typename C>( tuplet::tuple<C>* ) constexpr noexcept
        {
          if constexpr ( !std::is_const_v<C> )
          {
            const auto index = Traits::Tuple::Index<C, Pack::SharedTupleUnfiltered>;
            if constexpr ( std::is_pointer_v<C> )
              if ( Pack::m_map_index[index] == -1 )
                return;
            updated_keys[new_entities_count] = Component::GenerateSharedKey( Pack::m_archetype->ID(), Component::info<C>, std::bit_cast<const byte *>( &tuplet::get < Pack::template UniversalC<C> >( Pack::m_shared_tuple ) ) );
            updated_key_id += updated_keys[new_entities_count++].m_value;
          }
        }( Traits::Tuple::null<tuplet::tuple<Cs>> ) ), ... );

        if ( updated_key_id != Pack::m_key_checker )
        {
          array<byte *, Pack::nonconst_shared_count> pointers_to_shared;
          int count = 0;
          ( ( [&]<typename C>( tuplet::tuple<C>* ) constexpr noexcept
          {
            if constexpr ( !std::is_const_v<C> )
            {
              const auto index = Traits::Tuple::Index<C, Pack::SharedTupleUnfiltered>;
              if constexpr ( std::is_pointer_v<C> )
                if ( Pack::m_map_index[index] == -1 )
                  return;
              pointers_to_shared[count++] = std::bit_cast<byte *>( &tuplet::get<Pack::template UniversalC<C>>( Pack::m_shared_tuple ) );
            }
          }( Traits::Tuple::null<tuplet::tuple<Cs>> ) ), ... );

          XV_CORE_ASSERT_MSG( count == new_entities_count, "[QueryIterator] Entities count does not match" );
          XV_CORE_ASSERT_MSG( count == Pack::m_signature.count(), "[QueryIterator] Signature count does not match" );

          Pack::m_archetype->GetOrCreateFamilyFromSameArchetype( *Pack::m_family, Pack::m_signature, pointers_to_shared, updated_keys ).MoveIn( *Pack::m_world, *Pack::m_family, *Pack::m_pool, { i } );

          ( ( [&]<typename C>( tuplet::tuple<C>* ) constexpr noexcept
          {
            if constexpr ( !std::is_const_v<C> )
            {
              const auto index = Traits::Tuple::Index<C, Pack::SharedTupleUnfiltered>;
              if constexpr ( std::is_pointer_v<C> )
                if ( Pack::m_map::index[index] == -1 )
                  return;
              tuplet::get< Pack::template UniversalC<C> >( Pack::m_universal_tuple ) = *tuplet::get<Raw<C> *>( Pack::m_cached_shared_pointers );
            }
          }( Traits::Tuple::null<tuplet::tuple<Cs>> ) ), ... );
        }
      }( Traits::Tuple::null<Pack::SharedTupleUnfiltered> );

      if constexpr ( std::is_same_v<bool, ReturnType> )
        return break_loop;
    }
  }
  else if constexpr ( mode == Mode::DataOnly )
  {
    i32 i = Pack::m_pool->Size();
    auto param = [&]<typename C>( tuplet::tuple<C> * )constexpr noexcept -> C
    {
      auto &mptr = tuplet::get<Traits::Tuple::Index<Pack::template UniversalC<C>, decltype( Pack::m_data_tuple )>>( Pack::m_data_tuple );
      if constexpr ( std::is_pointer_v<C> )
        if ( !mptr ) return std::bit_cast<C>( nullptr );

      auto ptr = mptr;
      ++mptr;

      if constexpr ( std::is_pointer_v<C> ) return std::bit_cast<C>( ptr );
      else return reinterpret_cast<C>( *ptr );
    };

    do
    {
      if constexpr ( std::is_same_v<bool, ReturnType> )
      {
        if ( [&]<typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
        {
          return func( param( Traits::Tuple::null<tuplet::tuple<Cs>> )... );
        }( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> ) )
          return true;
      }
      else
      {
        [&] <typename... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
        {
          func( param( Traits::Tuple::null<tuplet::tuple<Cs>> )... );
        }( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );
      }
    } while ( --i );
    if constexpr ( std::is_same_v<bool, ReturnType> )
      return false;
  }
}
}