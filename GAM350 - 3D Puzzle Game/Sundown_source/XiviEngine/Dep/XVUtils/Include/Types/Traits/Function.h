#pragma once

#include <Types/Base.h>
#include <tuplet/tuple.hpp>
#include <type_traits>

/*
* Is invokable function
*/

namespace Xivi::Traits::Function::Details
{
  template <typename T>
  struct Callable
  {
    template <auto U>
    struct Check;

    template <typename>
    static std::false_type Test(...);

    template <typename Func>
    static std::true_type Test(Check<&Func::operator()> *);

    static constexpr auto value = std::is_same_v<decltype(Test<T>(nullptr)), std::true_type>;
  };

  template <typename T>
  constexpr auto is_callable = std::conditional_t<std::is_class<T>::value, Callable<T>, std::false_type>::value;
}

namespace Xivi::Traits::Function
{
  template <typename Func>
  constexpr auto& is_callable = Details::is_callable<Raw<Func>>;
}

namespace Xivi::Traits::Concepts
{
  template <typename Func>
  concept InvokableFunction = Function::is_callable<Raw<Func>>;
}

/*
* Function Traits
*/

namespace Xivi::Traits::Function
{

  namespace Details
  {
    template <bool NoExcept, typename Return, typename... Args>
    struct Make
    {
      using Type = Return(Args...) noexcept(NoExcept);
    };

    template <bool NoExcept, typename Return>
    struct Make<NoExcept, Return, void>
    {
      using Type = Return() noexcept(NoExcept);
    };
  }

  template <bool NoExcept, typename Return, typename... Args>
  using MakeType = typename Details::Make<NoExcept, Return, Args...>::Type;

  namespace Details
  {
    template <size_t Index, size_t Max, typename... Args>
    struct Params
    {
      using Type = typename std::tuple_element_t<Index, tuplet::tuple<Args...>>;
    };
    template <>
    struct Params<0, 0>
    {
      using Type = void;
    };
  }

  template <size_t Index, size_t Max, typename... Args>
  using ParamsType = typename Details::Params<Index, Max, Args...>::Type;

  template <class F>
  struct Type;

  template <bool NoExcept, typename Return, typename... Args>
  struct Decomposition
  {
    static constexpr size_t args_count = sizeof...(Args);

    using ReturnType = Return;
    using ClassType = void;
    using Self = Type<ReturnType(Args...) noexcept(NoExcept)>;
    using FuncType = MakeType<NoExcept, ReturnType, Args...>;
    using ArgsTuple = tuplet::tuple<Args...>;

    template <size_t Index>
    using Type = ParamsType<Index, args_count, Args...>;

    template <size_t Index>
    using SafeType = std::conditional_t < Index<args_count, Type<Index>, void>;
  };

  // functions
  template <typename Return, typename... Args>
  struct Type<Return(Args...) noexcept> : Decomposition<true, Return, Args...>
  {
    using ReturnType = Return;
    using ArgsTuple = tuplet::tuple<Args...>;
  };
  template <typename Return, typename... Args>
  struct Type<Return(Args...)> : Decomposition<false, Return, Args...>
  {
    using ReturnType = Return;
    using ArgsTuple = tuplet::tuple<Args...>;
  };

  // function pointer
  template <typename Return, typename... Args>
  struct Type<Return (*)(Args...)> : Type<Return(Args...)>
  {
    using ReturnType = Return;
    using ArgsTuple = tuplet::tuple<Args...>;
  };
  template <typename Return, typename... Args>
  struct Type<Return (*)(Args...) noexcept> : Type<Return(Args...) noexcept>
  {
    using ReturnType = Return;
    using ArgsTuple = tuplet::tuple<Args...>;
  };

  // member function pointer
  template <typename Class, typename Return, typename... Args>
  struct Type<Return (Class::*)(Args...) noexcept> : Type<Return(Args...) noexcept>
  {
    using ClassType = Class;
    using ReturnType = Return;
    using ArgsTuple = tuplet::tuple<Args...>;
  };
  template <typename Class, typename Return, typename... Args>
  struct Type<Return (Class::*)(Args...)> : Type<Return(Args...)>
  {
    using ClassType = Class;
    using ReturnType = Return;
  };

  // const member function pointer
  template <typename Class, typename Return, typename... Args>
  struct Type<Return (Class::*)(Args...) const noexcept> : Type<Return(Args...) noexcept>
  {
    using ClassType = Class;
    using ReturnType = Return;
    using ArgsTuple = tuplet::tuple<Args...>;
  };
  template <typename Class, typename Return, typename... Args>
  struct Type<Return (Class::*)(Args...) const> : Type<Return(Args...)>
  {
    using ClassType = Class;
    using ReturnType = Return;
  };

  // functors
  template <typename Class>
  struct Type : Type<decltype(&Class::operator())>
  {
    using ClassType = Class;
  };
  template <typename Class>
  struct Type<Class &> : Type<Class>
  {
  };
  template <typename Class>
  struct Type<const Class &> : Type<Class>
  {
  };
  template <typename Class>
  struct Type<Class &&> : Type<Class>
  {
  };
  template <typename Class>
  struct Type<const Class &&> : Type<Class>
  {
  };
  template <typename Class>
  struct Type<Class *> : Type<Class>
  {
  };
  template <typename Class>
  struct Type<const Class *> : Type<Class>
  {
  };
}

namespace Xivi::Traits::Concepts
{
template <typename Func, typename Return>
concept FuncReturnType = std::is_same_v<typename Traits::Function::Type<Func>::ReturnType, Return>;
}