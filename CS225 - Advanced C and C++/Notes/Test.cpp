#include <iostream>
#include <functional> 
#include <algorithm>
#include <numeric>
#include <array>
#include <vector>
#include <deque>
#include <tuple>
#include <cmath>
#include <execution>

namespace ns
{
	template<typename TIterator, typename F>
	void generate_n(TIterator begin, size_t n, F f)
	{
		while (n--)
		{
			*begin++ = f();
		}
	}

	template< typename TIterator, typename F>
	auto for_each(TIterator begin, TIterator end, F f)
	{
		while (begin != end)
		{
			f(*begin++);
		}

		return f;
	}

	template<typename TInIterator, typename TOutIterator, typename F>
	void transform(TInIterator ibegin, TInIterator iend, TOutIterator obegin, F f)
	{
		while(ibegin != iend)
		{
			*obegin++ = f(*ibegin++);
		}
	}

	template<typename TInIterator1,typename TInIterator2,typename TOutIterator,typename F>
	void transform(
		TInIterator1 ibegin1, TInIterator1 iend1,
		TInIterator2 ibegin2,
		TOutIterator obegin,
		F f)
	{
		while (ibegin1 != iend1)
		{
			*obegin++ = f(*ibegin1++, *ibegin2++);
		}
	}

	template<typename TInIterator, typename TValue>
	auto accumulate(TInIterator begin, TInIterator end, TValue value)
	{
		while (begin != end)
		{
			value += *begin++;
		}

		return value;
	}
}

int power(int base, int exp)
{
	return (int)std::pow((double)base, (double)exp);
}



int main()
{
	std::array<int, 10> array;

	int current_num = 0;
	auto get_value = [&current_num]()
	{
		auto result = current_num;
		current_num += 10;
		return result;
	};

	ns::generate_n(std::begin(array), 10, get_value);

	auto print = [](int x) //lamba
	{
		std::cout << x << ", ";
	};

	ns::for_each(std::begin(array), std::end(array), print);
	std::cout << std::endl;

	//------------------

	std::vector<int> v;
	v.resize(std::size(array));

	//1
	ns::transform(
		std::begin(array), std::end(array),
		std::begin(v), [](int x) {return x + 1;});

	ns::for_each(std::begin(v), std::end(v), print);
	std::cout << std::endl;

	//---------------------
	std::deque<int> d;
	d.resize(std::size(v));

	ns::transform(
		std::begin(array), std::end(array),		//input 1
		std::begin(v),							//input 2
		std::begin(d),							//output 
		[](int x, int y)						//f(I1, I2) -> 0
	{
		return y - x;
	}
	);

	ns::for_each(std::begin(d), std::end(d), print);
	std::cout << std::endl;

	//-------------------

	std::cout << "Accumulate: " << ns::accumulate(std::begin(d), std::end(d), 0) << std::endl;

	//--------------------

	std::cout << "Reduce : " << std::reduce(std::execution::par, std::begin(d), std::end(d), 0) << std::endl;

	//-------------------
	
	std::vector<int> v4;
	v4.resize(std::size(v));

	std::function<int(int)> f = std::bind(
		power,
		std::placeholders::_1,
		3
	);

	//int result = f(11);
	//auto fb = f(a,b,c)	//equal to auto fb = std::bind(f, 10, std::placeholder_2, std::placeholder::_1);
			  //fb(x,y)

	ns::transform(
		std::begin(v), std::end(v),
		std::begin(v4),
		f
	);

	ns::for_each(std::begin(v4), std::end(v4), print);
	std::cout << std::endl;

	//------------------

	//std::back_insert_iterator<std::vector<int>> it = std::back_inserter(v5);

	std::vector<int> v5;
	std::transform(
		std::begin(v4), std::end(v4),
		std::back_inserter(v5),
		[](int x) 
	{
		return x % 2;
	}
	);

	ns::for_each(std::begin(v5), std::end(v5), print);
	std::cout << std::endl;

	//---------------
	
	std::vector<int> v6 = { 0, 2, 3, 4 };

	ns::transform(
		std::begin(v4), std::end(v4),
		std::insert_iterator(v6, std::next(std::begin(v6))),
		[](int x)
	{
		return x % 2;
	}
	);

	ns::for_each(std::begin(v6), std::end(v6), print);
	std::cout << std::endl;

	//------------------
	/*
	auto lambda = []()
	{
		return std::make_tuple(2, 8);
	};

	auto [base, exp] = lambda();
	std::cout << power(base, exp) << std::endl;		//256
	*/
    
	auto get_parameters = []()
	{
		return std::make_tuple(2, 8);
	};

	std::cout << std::apply(power, get_parameters()) << std::endl;  //256
}
