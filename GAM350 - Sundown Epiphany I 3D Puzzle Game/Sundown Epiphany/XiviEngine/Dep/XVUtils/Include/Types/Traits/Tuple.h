#pragma once
#include "Specialised.h"
#include <tuplet/tuple.hpp>

namespace Xivi::Traits::Concepts
{
template <typename TupleType>
concept IsTuple = Traits::IsSpecialised<tuplet::tuple, TupleType>();
}

  /*
* Tuple Sort
*/

// From: https://codereview.stackexchange.com/questions/131194/selection-sorting-a-type-list-compile-time

namespace Xivi::Traits::Tuple::Details
{
  // swap types at index i and index j in the template argument tuple
template <size_t i, size_t j, typename TupleType>
class ElementSwap
{
  template <typename Indices>
  struct Implementation;

  template <size_t... Indices>
  struct Implementation<std::index_sequence<Indices...>>
  {
    using Type = tuplet::tuple<
      std::tuple_element_t<Indices != i && Indices != j ? Indices : Indices == i ? j
      : i,
      TupleType>...>;
  };

public:
  using Type = typename Implementation<
    std::make_index_sequence<std::tuple_size<TupleType>::value>>::Type;
};

template <template <typename, typename> typename Comparator, typename TupleType>
class SelectionSort
{
  // selection sort's "loop"
  template <size_t i, size_t j, size_t TupleSize, typename LoopTuple>
  struct Implementation
  {
    // this is done until we have compared every element in the type list
    // true: swap(i, j)
    // false: do nothing
    using Loop = std::conditional_t<
      Comparator<
      std::tuple_element_t<j, LoopTuple>, std::tuple_element_t<i, LoopTuple>>::value,
      typename ElementSwap<i, j, LoopTuple>::Type,
      LoopTuple>;

  // recurse until j == TupleSize using the modified tuple
    using Type = typename Implementation<i, j + 1, TupleSize, Loop>::Type;
  };

  template <size_t i, size_t TupleSize, typename LoopTuple>
  struct Implementation<i, TupleSize, TupleSize, LoopTuple>
  {
    // once j == TupleSize, we increment i and start j at i + 1 and recurse
    using Type = typename Implementation<i + 1, i + 2, TupleSize, LoopTuple>::Type;
  };

  template <size_t j, size_t TupleSize, class LoopTuple>
  struct Implementation<TupleSize, j, TupleSize, LoopTuple>
  {
    // once i == TupleSize, we know that every element has been compared
    using Type = LoopTuple;
  };

public:
  using Type = typename Implementation<0, 1, std::tuple_size<TupleType>::value, TupleType>::Type;
};

/*
* Tuple Index
*/

template <typename T, typename Ts>
struct Index;

template <typename T, typename... Ts>
struct Index<T, tuplet::tuple<T, Ts...>>
{
  static constexpr size_t value = 0;
};

template <typename T, typename U, typename... Ts>
struct Index<T, tuplet::tuple<U, Ts...>>
{
  static constexpr size_t value = 1 + Index<T, tuplet::tuple<Ts...>>::value;
};
}



namespace Xivi::Traits::Tuple
{

template <Concepts::IsTuple TupleType>
constexpr auto null = static_cast<Ptr<TupleType>>( nullptr );

template <typename... Args>
constexpr auto make_null = static_cast<Ptr<tuplet::tuple<Args...>>>( nullptr );

template <Concepts::IsTuple... Tuples>
using CatType = decltype( tuplet::tuple_cat( std::declval<Tuples>()... ) );

template <Concepts::IsTuple TupleType>
using Raw = std::invoke_result_t<decltype( []<typename... Ts>( tuplet::tuple<Ts...>*  )->tuplet::tuple<Raw<Ts>...>
{} ), TupleType* > ;


template <typename T, Concepts::IsTuple TupleType>
static constexpr auto Index = Details::Index<T, TupleType>::value;

template <template <typename, typename> typename Comparator, Concepts::IsTuple TupleType>
using SortType = typename Details::SelectionSort<Comparator, TupleType>::Type;
}