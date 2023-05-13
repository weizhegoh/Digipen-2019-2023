#include <iostream>
#include <tuple>

namespace deep
{
	template <typename...>
	struct leaf {};

	template <typename TFirst, typename... TRest>
	struct leaf<TFirst, TRest...> :
		leaf<TRest...>
	{
		TFirst _value;

		template <typename TValue, typename... TValues>
		explicit leaf(TValue&& value, TValues&&... values) :
			leaf<TRest...>(std::forward<TValues>(values)...),
			_value{std::forward<TValue>(value)}
		{
		}

		leaf(const leaf& lhs) :
			leaf<TRest...>(static_cast<const leaf<TRest...>&>(lhs)),
			_value{lhs._value}
		{
		}

		leaf() :
			_value{}
		{
		}
	};

	template <typename... Ts>
	struct tuple_impl :
		leaf<Ts...>
	{
		template <typename... TValues>
		explicit tuple_impl(TValues&&... values) :
			leaf<Ts...>(
				std::forward<TValues>(values)...
				)
		{
		}

		tuple_impl(const tuple_impl& lhs) :
			leaf<Ts...>(static_cast<const leaf<Ts...>&>(lhs))
		{
		}
	};

	template <typename... Ts>
	using tuple = tuple_impl<Ts...>;

	template <typename... Ts>
	auto make_tuple(Ts... args)
	{
		return tuple<Ts...>(args...);
	}

	template <size_t N, typename TFirst, typename... TRest>
	auto remove_N_type()
	{
		if constexpr (N == 0)
		{
			return leaf<TFirst, TRest...>{};
		}
		else
		{
			return remove_N_type<N - 1, TRest...>();
		}
	}

	template <size_t N, typename... Ts>
	using remove_N_type_t = decltype(remove_N_type<N, Ts...>());

	template <size_t N, typename... Ts>
	auto get(tuple<Ts...> t)
	{
		using leaf = remove_N_type_t<N, Ts...>;
		return static_cast<leaf&>(t)._value;
	}
}

namespace flat
{
	template <size_t N, typename T>
	struct leaf
	{
		T _value;

		template <typename TValue>
		explicit leaf(TValue&& value) :
			_value{std::forward<TValue>(value)}
		{
		}

		leaf(const leaf& lhs) :
			_value{lhs._value}
		{
		}

		leaf() :
			_value{}
		{
		}
	};

	template <size_t... Ns>
	struct index_sequence {};

	template <size_t Counter, size_t... Ns>
	auto index_sequence_imp()
	{
		if constexpr (Counter == 0)
		{
			return index_sequence<Ns...>{};
		}
		else
		{
			return index_sequence_imp<
				Counter - 1,
				Counter - 1, Ns...
			>();
		}
	}

	template <size_t N>
	using make_sequence = decltype(index_sequence_imp<N>());

	template <typename Seq, typename... Ts>
	struct tuple_impl {};

	template <size_t... Ns, typename... Ts>
	struct tuple_impl< index_sequence<Ns...>, Ts...> :
		leaf<Ns, Ts>...
	{
		template <typename... TValues>
		explicit tuple_impl(TValues&&... values) :
			leaf<Ns, Ts>(std::forward<TValues>(values))...
		{
		}

		tuple_impl(const tuple_impl& lhs) :
			leaf<Ns, Ts>(static_cast<const leaf<Ns, Ts>&>(lhs))...
		{
		}

		tuple_impl() :
			leaf<Ns, Ts>()...
		{
		}
	};

	template <typename... Ts>
	struct tuple :
		tuple_impl<
		make_sequence< sizeof...(Ts) >,
		Ts...
		>
	{
		using base = tuple_impl<
			make_sequence< sizeof...(Ts) >,
			Ts...
		>;

		template <typename... TValues>
		explicit tuple(TValues&&... values) :
			base{std::forward<TValues>(values)...}
		{
		}

		tuple(const tuple& lhs) :
			base(static_cast<const base&>(lhs))
		{
		}

		tuple() :
			base()
		{
		}
	};

	template <
		size_t Counter,
		size_t N,
		typename TFirst, typename... TRest>
		auto get_Nth_type()
	{
		if constexpr (Counter == 0)
		{
			return leaf<N, TFirst>{};
		}
		else
		{
			return get_Nth_type<Counter - 1, N, TRest...>();
		}
	}

	template <size_t N, typename... Ts>
	using get_Nth_type_t = decltype(get_Nth_type<N, N, Ts...>());

	template <size_t N, typename... Ts>
	auto get(tuple<Ts...> t)
	{
		using leaf = get_Nth_type_t<N, Ts...>;
		return static_cast<leaf&>(t)._value;
	}

	template <typename... Ts>
	auto make_tuple(Ts... args)
	{
		return tuple<Ts...>(args...);
	}
}

// namespace ns = deep;
// namespace ns = flat;
namespace ns = std;

int main()
{
	auto t = ns::make_tuple(10, 20, 30.0);
	std::cout
		<< sizeof(t) << "\n"
		<< ns::get<1>(t)
		<< std::endl;
}
