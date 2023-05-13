#include "MemoryAlloc.h"

#include <iostream>
#include <climits>
#include <sstream>
#include <string>
#include <exception>

// Define an alias using typedef to prove there is no templates involved.
#include "bitset_tep.h"
typedef cs225::bitset_tep tep;

#ifndef USE_STL_BITSET

	#include "allocator.h"
	#include "bitset.h"
	namespace ns
	{
		template <size_t N>
		using bitset = cs225::bitset<N, cs225::allocator>;
	}

#else

	#include <bitset>
	namespace ns
	{
		template <size_t N>
		using bitset = std::bitset<N>;
	}

#endif

namespace
{
#include "Test.h"

	void test1_impl(tep bitset, size_t index)
	{
		bitset.set(index, true);
		if (!bitset.test(index))
		{
			throw std::runtime_error{"Bit has not been set."};
		}
		bitset.set(index, false);
		bitset.set(index + 1, true);
		if (bitset.test(index))
		{
			throw std::runtime_error{"Bit has not been cleared."};
		}
	}

	void test1()
	{
		constexpr size_t INDEX = 0;
		using bs = ns::bitset<16>;
		test1_impl(tep::create<bs>(), INDEX);
	}

	void test2_impl(tep bitset, size_t index)
	{
		bitset.set(index, true);
		if (!bitset.test(index))
		{
			throw std::runtime_error{"Bit has not been set."};
		}
		bitset.set(index, false);
		bitset.set(index - 1, true);
		if (bitset.test(index))
		{
			throw std::runtime_error{"Bit has not been cleared."};
		}
	}

	void test2()
	{
		constexpr size_t INDEX = 15;
		using bs = ns::bitset<INDEX + 1>;
		test2_impl(tep::create<bs>(), INDEX);
	}

	void test3_impl(tep bitset, size_t index)
	{
		bitset.set(index, true);
		if (!bitset.test(index))
		{
			throw std::runtime_error{"Bit has not been set."};
		}
		bitset.reset(index);
		bitset.set(index + 1, true);
		if (bitset.test(index))
		{
			throw std::runtime_error{"Bit has not been cleared."};
		}
	}

	void test3()
	{
		constexpr size_t INDEX = 0;
		using bs = ns::bitset<16>;
		test3_impl(tep::create<bs>(), INDEX);
	}
	
	void test4_impl(tep bitset, size_t index)
	{
		bitset.set(index, true);
		if (!bitset.test(index))
		{
			throw std::runtime_error{"Bit should have been set."};
		}
		bitset.flip(index);
		bitset.set(index - 1, true);
		if (bitset.test(index))
		{
			throw std::runtime_error{"Bit should have been cleared."};
		}
	}

	void test4()
	{
		constexpr size_t INDEX = 15;
		using bs = ns::bitset<INDEX + 1>;
		test4_impl(tep::create<bs>(), INDEX);
	}

	void test5_impl(tep bitset, size_t index)
	{
		bitset.reset(index);
		if (bitset.test(index))
		{
			throw std::runtime_error{"Bit should have been cleared."};
		}
		bitset.flip(index);
		bitset.set(index - 1, false);
		if (!bitset.test(index))
		{
			throw std::runtime_error{"Bit should have been set."};
		}
	}

	void test5()
	{
		constexpr size_t INDEX = 15;
		using bs = ns::bitset<INDEX + 1>;
		test5_impl(tep::create<bs>(), INDEX);
	}

	void test6_impl(tep bitset)
	{
		try
		{
			(void)bitset.test(bitset.size());
			throw std::runtime_error
			{
				"Test of a bit outside of range should have resulted in "
				"an exception."
			};
		}
		catch (const std::out_of_range&)
		{
			// All good; swallow the exception and continue as normal.
		}
	}

	void test6()
	{
		using bs = ns::bitset<CHAR_BIT>;
		test6_impl(tep::create<bs>());
	}
	
	void test7_impl(tep bitset, size_t index)
	{
		bitset.set(index, true);
		if (!bitset.test(index))
		{
			throw std::runtime_error{"Bit has not been set."};
		}
		bitset.set(index, false);
		if (bitset[index])
		{
			throw std::runtime_error{"Bit has not been cleared."};
		}
	}

	void test7()
	{
		constexpr size_t INDEX = 15;
		using bs = ns::bitset<INDEX + 1>;
		test7_impl(tep::create<bs>(), INDEX);
	}

	void test8_impl(tep bitset)
	{
		bitset.set(1);
		bitset.set(2);

		const std::string s = bitset.to_string();
		if (s != "00110")
		{
			std::cout << s << std::endl;
			throw std::runtime_error{"Converstion to string did not work."};
		}
	}

	void test8()
	{
		using bs = ns::bitset<5>;
		test8_impl(tep::create<bs>());
	}
	
	void test9_impl(tep bitset)
	{
		bitset.set(1);
		bitset.set(2);

		const std::string s = bitset.to_string('_');
		if (s != "__11_")
		{
			std::cout << s << std::endl;
			throw std::runtime_error{"Converstion to string did not work."};
		}
	}

	void test9()
	{
		using bs = ns::bitset<5>;
		test9_impl(tep::create<bs>());
	}
	
	void test10_impl(tep bitset)
	{
		bitset.set(1);
		bitset.set(2);

		const std::string s = bitset.to_string('F', 'T');
		if (s != "FFTTF")
		{
			std::cout << s << std::endl;
			throw std::runtime_error{"Converstion to string did not work."};
		}
	}

	void test10()
	{
		using bs = ns::bitset<5>;
		test10_impl(tep::create<bs>());
	}

	void test11_impl(tep bitset)
	{
		bitset.set(1);
		bitset.set(2);
		if (bitset.count() != 2)
		{
			throw std::runtime_error{"Counting set bits did not work."};
		}
	}

	void test11()
	{
		using bs = ns::bitset<5>;
		test11_impl(tep::create<bs>());
	}
	
	void test12_impl(tep bitset)
	{
		bitset.reset(1);
		bitset.set(2);
		if (bitset.count() != 1)
		{
			throw std::runtime_error{"Counting set bits did not work."};
		}
	}
	
	void test12()
	{
		using bs = ns::bitset<50>;
		test12_impl(tep::create<bs>());
	}
}

int main()
{
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	const Test tests[] = {
		test1,
		test2,
		test3,
		test4,
		test5,
		test6,
		test7,
		test8,
		test9,
		test10,
		test11,
		test12
	};

	std::cout << __FILE__ << std::endl;
	std::size_t test_index = 0;
	for (std::size_t i = 0; i < sizeof(tests) / sizeof(*tests); ++i)
	{
		run(test_index, tests[i]);
	}
}
