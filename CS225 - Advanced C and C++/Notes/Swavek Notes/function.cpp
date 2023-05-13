/******************************************************************************
 * This example demonstrates our implementation of std::function that is
 * a type-erased wrapper for any callable object.
 *****************************************************************************/

#include <iostream>
#include <functional>

///////////////////////////////////////////////////////////////////////////////

namespace my
{
	// Our base template will never be used, as we want to allow for a familiar
	// syntax function<TResult(Args...)> that resembles a function pointer.
	template <typename TResult, typename... Args>
	class function;

	// Only this specialization will be instantiated.
	template <typename TResult, typename... Args>
	class function<TResult(Args...)>
	{
		// TEP concept; take note that we are not using any universal
		// references as the references should be explicitly indicated in the
		// type of an instantiated function.
		struct IConcept
		{
			virtual TResult operator()(Args... args) = 0;
			virtual ~IConcept() = default;
		};

		// F is our callable object passed to the constructor by-value. This is
		// important, because for lambda expressions with capture we will get
		// a copy of a lambda, not the actual instance we passed in.
		template <typename F>
		struct Model : IConcept
		{
			F _instance;
		public:
			Model(F f) : _instance{f}
			{
			}

			virtual TResult operator()(Args... args) override
			{
				// Call the actual callable object forwarding parameters as
				// per their types declared for our function type.
				return _instance(std::forward<Args>(args)...);
			}
		};

		// This should be a smart pointer, but in the lecture we used a raw
		// pointer to not give out the answer for the same implementation
		// required in the assignment...
		IConcept* _concept;

	public:
		template <typename F>
		function(F f) : _concept{new Model<F>(f)}
		{
		}

		TResult operator()(Args... args)
		{
			// Call the model callable object forwarding parameters as
			// per their types declared for our function type.
			return _concept->operator()(std::forward<Args>(args)...);
		}

		// If we use a smart pointer there is no need for any of
		// the declarations below, because we can follow the rule of 0;
		// with a raw pointer we need to think about the copy/move semantics.

		function(const function&) = delete;
		function(function&&) = delete;
		void operator=(const function&) = delete;
		void operator=(function&&) = delete;

		~function()
		{
			delete _concept;
		}
	};
}

/******************************************************************************
 * Declarations and definitions used by examples in the main() function.
 *****************************************************************************/

// Function example

int sum1 = 0;
void add(int x)
{
	sum1 += x;
}

// Function pointer example

int sum2 = 0;
void add2(int x)
{
	sum2 += x;
}

// Member function example

class Adder
{
	int _sum;
public:
	Adder() : _sum{0} {}

	void add(int x)
	{
		_sum += x;
	}

	int sum() const
	{
		return _sum;
	}
};

// Member function pointer example

class Adder2
{
	int _sum;
public:
	Adder2() : _sum{0} {}

	void add(int x)
	{
		_sum += x;
	}

	int sum() const
	{
		return _sum;
	}
};

// std::mem_fn (it is really just some functor) example

class Adder3
{
	int _sum;
public:
	Adder3() : _sum{0} {}

	void add(int x)
	{
		_sum += x;
	}

	int sum() const
	{
		return _sum;
	}
};

// Functor (a class with an operator() overloaded) example

class Adder4
{
	int _sum;
public:
	Adder4() : _sum{0} {}

	void operator()(int x)
	{
		_sum += x;
	}

	int sum() const
	{
		return _sum;
	}
};

// Lambda with capture example

class Adder5
{
	int& _sum;
public:
	Adder5(int& sum) : _sum{sum} {}

	void operator()(int x)
	{
		_sum += x;
	}
};

// Lambda without capture example

int sum7 = 0;

// Lambda with deduced parameters example

int sum8 = 0;

///////////////////////////////////////////////////////////////////////////////

int main()
{
	std::cout << "\nFunction" << std::endl;
	{
		my::function<void(int)> myadd = add;

		std::cout << "\t" << sum1 << std::endl;
		add(1);
		add(2);
		myadd(3);
		std::cout << "\t" << sum1 << std::endl;
	}

	std::cout << "\nFunction pointer" << std::endl;
	{
		using PFunc = void(*)(int);
		PFunc f_ptr = add2;
		my::function<void(int)> myadd = f_ptr;

		std::cout << "\t" << sum2 << std::endl;
		f_ptr(10);
		f_ptr(20);
		myadd(30);
		std::cout << "\t" << sum2 << std::endl;
	}

	std::cout << "\nMember function" << std::endl;
	{
		Adder adder;
		// Using my::function is not directly possible due to `this`.
		// See below: std::mem_fn

		std::cout << "\t" << adder.sum() << std::endl;
		(adder.add)(100);
		(adder.add)(200);
		std::cout << "\t" << adder.sum() << std::endl;
	}

	std::cout << "\nMember function pointer" << std::endl;
	{
		using PFunc = void(Adder2::*)(int);
		PFunc f_ptr = &Adder2::add;
		Adder2 adder2;
		// Using my::function is not directly possible due to `this`.
		// See below: std::mem_fn

		std::cout << "\t" << adder2.sum() << std::endl;
		(adder2.*f_ptr)(1000);
		(adder2.*f_ptr)(2000);
		std::cout << "\t" << adder2.sum() << std::endl;
	}

	std::cout << "\nstd::mem_fn (it is really just some functor)" << std::endl;
	{
		my::function<void(Adder3&, int)> myadd = std::mem_fn(&Adder3::add);
		Adder3 adder3;

		std::cout << "\t" << adder3.sum() << std::endl;
		myadd(adder3, 10000);
		myadd(adder3, 20000);
		std::cout << "\t" << adder3.sum() << std::endl;
	}

	std::cout << "\nFunctor (a class with an operator() overloaded)" << std::endl;
	{
		Adder4 adder4;
		my::function<void(int)> myadd = adder4;

		std::cout << "\t" << adder4.sum() << std::endl;
		adder4(100000);
		adder4(200000);
		myadd(300000);	// Does not work; adds to a copy within my::function
		std::cout << "\t" << adder4.sum() << std::endl;
	}

	std::cout << "\nLambda with capture";
	// Under-the-hood it is a functor with
	// - data members
	//   * stored by value for variables captured by value
	//   * stored by reference for variables captured by reference
	// - constructor initializing these members
	// - operator() containing the body of lambda
	std::cout << "\n- Similar functor" << std::endl;
	{
		int sum5 = 0;
		my::function<void(int)> myadd = Adder5{sum5};

		std::cout << "\t" << sum5 << std::endl;
		myadd(4);
		myadd(5);
		std::cout << "\t" << sum5 << std::endl;
	}
	std::cout << "- As a lambda" << std::endl;
	{
		int sum5 = 0;
		auto lambda = [&sum5](int x) { sum5 += x; };
		my::function<void(int)> myadd = lambda;

		std::cout << "\t" << sum5 << std::endl;
		myadd(40);
		myadd(50);
		std::cout << "\t" << sum5 << std::endl;
	}
	std::cout << "- As a lambda with capture with deduced parameters" << std::endl;
	{
		int sum6 = 0;
		auto lambda = [&sum6](auto x)
		{
			sum6 += x;
		};
		std::function<void(int)> myadd = lambda;

		std::cout << "\t" << sum6 << std::endl;
		myadd(static_cast<short>(6));
		myadd(7);
		std::cout << "\t" << sum6 << std::endl;
	}

	std::cout << "\nLambda without capture";
	// Under-the-hood it is a function/a function template with a set
	// of input parameters "(int x)"/parameters of deduced types "(auto x)",
	// with a body "{}", and a return type (deduced or trailing).
	std::cout << "\n- As a lambda" << std::endl;
	{
		using PFunc = void(*)(int);
		PFunc lambda = [](int x)
		{
			sum7 += x;
		};
		std::function<void(int)> myadd = lambda;

		std::cout << "\t" << sum7 << std::endl;
		myadd(400);
		myadd(500);
		std::cout << "\t" << sum7 << std::endl;
	}
	std::cout << "- As a lambda with deduced parameters" << std::endl;
	{
		using PFunc = void(*)(int);
		PFunc lambda = [](auto x)
		{
			sum8 += x;
		};
		std::function<void(int)> myadd = lambda;

		std::cout << "\t" << sum8 << std::endl;
		myadd(60);
		myadd(70);
		std::cout << "\t" << sum8 << std::endl;
	}

	std::cout << "\nstd::function (a TEP wrapper, a functor)" << std::endl;
	{
		int sum9 = 0;
		auto lambda = [&sum9](int x)
		{
			sum9 += x;
		};
		std::function<void(int)> myadd = lambda;

		std::cout << "\t" << sum9 << std::endl;
		myadd(600);
		myadd(700);
		std::cout << "\t" << sum9 << std::endl;
	}

	std::cout << "\nmy::function (a TEP wrapper, a functor)" << std::endl;
	{
		int sum10 = 0;
		auto lambda = [&sum10](int x)
		{
			sum10 += x;
		};
		my::function<void(int)> myadd = lambda;

		std::cout << "\t" << sum10 << std::endl;
		myadd(6000);
		myadd(7000);
		std::cout << "\t" << sum10 << std::endl;
	}
}
