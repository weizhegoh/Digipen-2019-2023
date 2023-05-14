/*****************************************************************
*\file         QueryIterator.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::Query
{
enum class Mode
{
  DataOnly,
  DataAndShared
};

namespace Details
{
template <Traits::Concepts::InvokableFunction Func, Mode TMode = ECS::Concepts::HasSharedParameters<Func> ? Mode::DataAndShared : Mode::DataOnly>
struct DataPack;

template <Traits::Concepts::InvokableFunction Func>
struct DataPack<Func, Mode::DataOnly>
{
  static constexpr auto mode = Mode::DataOnly;

  using FuncTuple = typename Traits::Function::Type<Func>::ArgsTuple;
  using DataTupleUnfiltered = FuncTuple;

  template <typename C>
  using UniversalC = Ptr<Raw<C>>;

  using DataTuple =
    std::invoke_result_t< decltype( []<typename... Cs>( tuplet::tuple<Cs...> * )
                                    -> Traits::Tuple::CatType<tuplet::tuple<UniversalC<Cs>>...>
  {} ), decltype( Traits::Tuple::null<DataTupleUnfiltered> ) > ;

  Ptr<Archetype::Instance> m_archetype;
  Ptr<Pool::Instance> m_pool;
  DataTuple m_data_tuple;
};

template <Traits::Concepts::InvokableFunction Func>
struct DataPack<Func, Mode::DataAndShared>
{
  static constexpr auto mode = Mode::DataAndShared;

  using FuncTuple = typename Traits::Function::Type<Func>::ArgsTuple;

  using DataTupleUnfiltered =
    std::invoke_result_t<decltype( []<typename... Cs>( tuplet::tuple<Cs...> * )
                                   -> Traits::Tuple::CatType<std::conditional_t<Component::info<Cs>.m_type == Component::Type::ID::Data, tuplet::tuple<Cs>, tuplet::tuple<>>...>
  {} ),
    decltype( Traits::Tuple::null<FuncTuple> ) > ;

  using SharedTupleUnfiltered =
    std::invoke_result_t<decltype( []<typename...Cs>( tuplet::tuple<Cs...> * )
                                   -> Traits::Tuple::CatType< std::conditional_t< Component::info<Cs>.m_type == Component::Type::ID::Shared,
                                   tuplet::tuple<Cs>, tuplet::tuple<>
                                   >...>
  {} ), decltype( Traits::Tuple::null<FuncTuple> ) > ;

  using SharedTupleChangeableUnfiltered =
    std::invoke_result_t<decltype( []<typename... Cs>( tuplet::tuple<Cs...> * )
                                   -> Traits::Tuple::CatType<std::conditional_t<std::is_const_v<Cs>, tuplet::tuple<>, tuplet::tuple<Cs>>...>
  {} ),
    decltype( Traits::Tuple::null<SharedTupleUnfiltered> ) > ;

  static constexpr auto shared_count = tuple_size_v<SharedTupleUnfiltered>;
  static constexpr auto nonconst_shared_count = tuple_size_v<SharedTupleChangeableUnfiltered>;
  static constexpr auto const_shared_count = shared_count - nonconst_shared_count;
  static constexpr auto data_count = tuple_size_v<FuncTuple> -shared_count;
  static_assert( shared_count );

  template <typename C>
  using UniversalC = std::conditional_t<
    Component::info<C>.m_type == Component::Type::ID::Data,
    Ptr<Raw<C>>,
    std::conditional_t<
    std::is_reference_v<C>,
    std::conditional_t<
    std::is_const_v<C>,
    Ptr<std::remove_reference_t<C>>,
    std::remove_reference_t<C>>, std::conditional_t<
    std::is_const_v<C>, C, std::remove_pointer_t<C>>>>;

  using DataTuple =
    std::invoke_result_t<decltype( []<typename... Cs>( tuplet::tuple<Cs... > * )
                                   -> Traits::Tuple::CatType<tuplet::tuple<UniversalC<Cs>>...>
  {} ), decltype( Traits::Tuple::null<FuncTuple> ) > ;

  using SharedTuple =
    std::invoke_result_t<decltype( []<typename... Cs>( tuplet::tuple<Cs... > * )
                                   ->Traits::Tuple::CatType<std::conditional_t<Component::info<Cs>.m_type == Component::Type::ID::Shared, tuple<UniversalC<Cs>>, tuplet::tuple<>>...>
  {} ), decltype( Traits::Tuple::null<FuncTuple> ) > ;

  using SharedTuplePointers =
    std::invoke_result_t< decltype( []<typename... Cs>( tuplet::tuple<Cs...> * )
                                    ->Traits::Tuple::CatType< std::conditional_t< Component::info<Cs>.m_type == Component::Type::ID::Shared && ( false == std::is_pointer_v< UniversalC<Cs> > ),
                                    tuplet::tuple< Raw<Cs> * >, tuple<>>...>
  {} ), decltype( Traits::Tuple::null<FuncTuple> ) > ;

  static_assert( nonconst_shared_count == tuple_size_v<SharedTuplePointers> );

  inline DataPack() noexcept;

  Ptr<World::Instance> m_world;
  Ptr<Archetype::Instance> m_archetype;
  Ptr<Pool::Family::Instance> m_family;
  Ptr<Pool::Instance> m_pool;
  const Signature m_signature;
  Signature m_shared_signature;
  array<u8, shared_count> m_map_index;
  array<Component::Type::Shared::Key, shared_count> m_cached_keys;
  SharedTuplePointers m_cached_shared_pointers;
  SharedTuple m_shared_tuple;
  DataTuple m_data_tuple;
  u64 m_key_checker;
};
}

template <Traits::Concepts::InvokableFunction Func, Mode TMode = Mode::DataOnly>
struct Iterator : Details::DataPack<Func, TMode>
{
  using Pack = Details::DataPack<Func, TMode>;
  using ReturnType = typename Traits::Function::Type<Func>::ReturnType;
  static constexpr auto mode = Pack::mode;

  inline Iterator( World::Instance &world ) noexcept;
  inline void UpdateArchetype( Archetype::Instance &archetype ) noexcept;
  inline void UpdateFamily( Pool::Family::Instance &family ) noexcept;
  inline void UpdatePool( Pool::Instance &pool ) noexcept;
  inline ReturnType ForEach( Func &&func ) noexcept;
};
}