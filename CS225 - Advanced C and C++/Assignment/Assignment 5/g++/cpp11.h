#ifndef CPP11_H_
#define CPP11_H_

template <size_t Count, size_t... Rest>
struct make_sequence_impl
{
	using type = typename make_sequence_impl<Count-1,Rest...,1UL<<Count >::type;
};

template <size_t... Rest>
struct make_sequence_impl<0, Rest...>
{
	using type = index_sequence<Rest..., 1>;
};

template <size_t T>
using make_sequence = typename make_sequence_impl<T>::type;

#endif // !CPP11_H_H
