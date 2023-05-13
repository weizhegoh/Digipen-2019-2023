#include "Test4.h"
#include <iostream>

/*
namespace Final
{
	template <typename T>
	struct Node
	{
		T data;
		Node* next;
	};

	template <typename T>
	class List
	{
	protected:
		Node<T>* head;

	public:
		List() :head(nullptr) {}

		void push_front(T value)
		{
			Node<T>* newNode = new Node<T>;

			newNode->data = value;
			newNode->next = head;

			head = newNode;
		}

		void print(Node<T>* list)
		{
			Node<T>* current = list;

			if (list == nullptr)
			{
				std::cout << std::endl;
				return;
			}

			while (current)
			{
				if (current->next)
				{
					std::cout << current->data << ",";
				}
				else
				{
					std::cout << current->data;
				}
				current = current->next;
			}

			std::cout << std::endl;

		}
	};

	////////////////////////////////////////////////////////////

	template <typename T>
	class Test: protected List<T>
	{
	private:
		T a;

	public:
		Test() :a(0) {}					//default constructor

		Test(T A) : a(A)				//parameterized constructor
		{

		}				

		T getValue() const				//getter
		{
			return a;
		}

		void setValue(T A)				//setter
		{
			this->a = A;
		}

		Test(const Test<T> &A)			//copy constructor
		{
			a = A.a;
		}

		Test operator+(const Test& rhs)	// plus operator
		{
			return a + rhs.a;
		}

		Test operator-(const Test& rhs) // minus operator
		{
			return a - rhs.a;
		}

		Test operator*(const Test& rhs) // multiply operator
		{
			return a * rhs.a;
		}

		Test operator/(const Test& rhs) // divide operator
		{
			return a / rhs.a;
		}

		Test operator+=(const Test& rhs) // += operator
		{
			return a += rhs.a;
		}

		Test operator-=(const Test& rhs) // += operator
		{
			return a -= rhs.a;
		}

		Test operator++()				// ++a operator
		{
			++a;
			return *this;
		}

		Test operator++(int)			// a++ operator
		{
			a++;
			return *this;
		}

		Test operator--()				// --a operator
		{
			--a;
			return *this;
		}

		Test operator--(int)			// a-- operator
		{
			a--;
			return *this;
		}

		Test operator=(const Test& rhs) // = operator
		{
			a = rhs.a;
			return *this;
		}

		friend std::ostream& operator <<(std::ostream& os, const Test<T>& rhs)
		{

			if (rhs.a < 0)
			{
				throw "Negative";
			}
			else
			{
				os << rhs.a;	//Overloading for cout
				return os;
			}
		}

		~Test() {}						//Destructor

		//////////////////////////////////////////////////////

		void push(T value)
		{
			List<T>::push_front(value);
		}

		T pop()
		{
			if (isEmpty() == true)
			{
				throw "Error";
			}

			T deleteValue = 0;

			deleteValue = List<T>::head->data;

			Node<T>* deleteNode = nullptr;

			Node<T>* current = List<T>::head;

			deleteNode = current;

			List<T>::head = current->next;

			delete deleteNode;
			return deleteValue;
		}

		bool isEmpty()
		{
			return (List<T>::head == nullptr) ? true : false;
		}
	};
}

int main(void)
{
#if defined(debug) | defined(_debug)
	_crtsetdbgflag(_crtdbg_alloc_mem_df | _crtdbg_leak_check_df);
#endif

	Final::Test<int> A;

	A.setValue(10);
	std::cout << A.getValue() << std::endl;			//10

	Final::Test<int> B(A);							//copy constructor
	std::cout << B.getValue() << std::endl;			//10

	Final::Test<int> C(50);
	std::cout << C.getValue() << std::endl;			//50

	Final::Test<int> D;
	D = C + B;
	std::cout << D.getValue() << std::endl;			//50+10 = 60

	D = C - B;
	std::cout << D.getValue() << std::endl;			//50-10 = 40

	D = C * B;
	std::cout << D.getValue() << std::endl;			//50*10 = 500

	D = C / B;
	std::cout << D.getValue() << std::endl;			//50/10 = 5

	Final::Test<int> E(1);

	E += D;
	std::cout << E.getValue() << std::endl;			//5+1 = 6

	E -= D;
	std::cout << E.getValue() << std::endl;			//6-5 = 1

	E = ++D;
	std::cout << E.getValue() << std::endl;			//++5 = 6

	E = D++;
	std::cout << E.getValue() << std::endl;			//6++ = 7

	E = --D;
	std::cout << E.getValue() << std::endl;			//--7 = 6

	E = D--;
	std::cout << E.getValue() << std::endl;			//6-- = 5

	Final::Test<int> F;
	F = E;
	std::cout << F.getValue() << std::endl;			//5

	Final::Test<int> G(20);
	std::cout << G;									//20

	///////////////////////////////////////////////////////////

	std::cout << "********" << std::endl;

	///////////////////////////////////////////////////////////

	Final::Test<double> H;

	H.setValue(10.1);
	std::cout << H.getValue() << std::endl;			//10.1

	Final::Test<double> I(H);						//copy constructor
	std::cout << I.getValue() << std::endl;			//10.1

	Final::Test<double> J(50.1);
	std::cout << J.getValue() << std::endl;			//50.1

	Final::Test<double> K;
	K = J + I;
	std::cout << K.getValue() << std::endl;			//50.1+10.1 = 60.2

	K = J - I;
	std::cout << K.getValue() << std::endl;			//50.1-10.1 = 40

	K = J * I;
	std::cout << K.getValue() << std::endl;			//50.1*10.1 = 506.01

	K = J / I;
	std::cout << K.getValue() << std::endl;			//50.1/10.1 = 4.9604

	Final::Test<double> L(1.1);

	L += K;
	std::cout << L.getValue() << std::endl;			//4.9604+1.1 = 6.0604

	L -= K;
	std::cout << L.getValue() << std::endl;			//6.0604 - 4.9064 = 1.1

	L =  ++K;
	std::cout << L.getValue() << std::endl;			//++4.9064 = 5.9064

	L = K++;
	std::cout << L.getValue() << std::endl;			//5.9064++ = 6.9064

	L = --K;
	std::cout << L.getValue() << std::endl;			//--6.9064 = 5.9064

	L = K--;
	std::cout << L.getValue() << std::endl;			//5.9064-- = 4.9064

	Final::Test<double> M;
	M = L;
	std::cout << M.getValue() << std::endl;			//4.9064

	Final::Test<double> O(20.1);
	std::cout << O;									//20.1

	///////////////////////////////////////////////////////////

	std::cout << "********" << std::endl;

	///////////////////////////////////////////////////////////

	Final::Test<int> P;
	
	P.push(3);
	P.push(2);
	P.push(1);
	
	try
	{
		std::cout << P.pop() << "->";	//1->
		std::cout << P.pop() << "->";   //1->2->
		std::cout << P.pop() << "->";   //1->2->3->
		std::cout << P.pop() << "->";   //1->2->3->Error
	}
	catch (const char* str)
	{
		std::cout << str << std::endl;
	}

	Final::Test<double> Q;

	Q.push(10.3);
	Q.push(10.2);
	Q.push(10.1);

	try
	{
		std::cout << Q.pop() << "->";  //10.1->
		std::cout << Q.pop() << "->";  //10.1->10.2->
		std::cout << Q.pop() << "->";  //10.1->10.2->10.3->
		std::cout << Q.pop() << "->";  //10.1->10.2->10.4->Error
	}
	catch (const char* str)
	{
		std::cout << str << std::endl;
	}

	Final::Test<int> R(-10);
	Final::Test<double> S(10.1);

	try
	{
		std::cout << R << "->";		//Negative		
		std::cout << S << "->";		//Once throw error msg, all will be skipped
	}
	catch (const char* str)
	{
		std::cout << str << std::endl;
	}
}
*/