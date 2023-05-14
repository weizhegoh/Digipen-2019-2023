#pragma once

#include <type_traits>

/*
* Check specialised template
*/

namespace Xivi::Traits::Details
{
template <template <typename...> typename Base, typename Derived>
struct Specialised : std::false_type
{};

template <template <typename...> typename Base, typename... Args>
struct Specialised<Base, Base<Args...>> : std::true_type
{};
}


namespace Xivi::Traits
{
template <template <typename...> typename Base, typename Derived>
inline constexpr bool IsSpecialised()
{
  return Details::Specialised<Base, Derived>::value;
}
}