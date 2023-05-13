#ifndef CPP17_H
#define CPP17_H

template <size_t Count, size_t... Rest>
constexpr auto make_sequence_impl()
{
	if constexpr (Count != 0)
	{
		return make_sequence_impl<Count - 1, Rest..., 1UL << Count>();
	}
	else
	{
		return index_sequence<Rest..., 1>{};
	}
}

template <size_t T>
using make_sequence = decltype(make_sequence_impl<T>());

#endif // !CPP17_H
