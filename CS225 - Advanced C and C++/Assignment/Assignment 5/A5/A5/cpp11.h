#ifndef CPP11_H_
#define CPP11_H_

template <size_t Counter, size_t... Rest>
struct make_sequence_impl
{
	using type = typename make_sequence_impl<Counter - 1, Counter - 1, Rest...>::type;
};

template<size_t...Rest>
struct make_sequence_impl<0, Rest...>
{
	using type = index_sequence<Rest...>;
};

template<size_t N>
using make_sequence = typename make_sequence_impl<N>::type;


#endif // !CPP11_H_H
