#ifndef CPP17_H
#define CPP17_H

template <size_t Counter, size_t... Rest>
constexpr auto make_sequence_impl()
{
	if constexpr (Counter != 0)
	{
		return make_sequence_impl<
			Counter - 1,
			Counter - 1, Rest...
		>();
	}
	else
	{
		return index_sequence<Rest...>{};
	}
}

template <size_t N>
using make_sequence =
decltype(make_sequence_impl<N>());

#endif // !CPP17_H
