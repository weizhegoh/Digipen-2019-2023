Quiz 1: 
=======
#include <iostream>
using namespace std;

struct ImoveableObject{};
struct Vehicle: ImoveableObject
{
    int velocity;
    Vehicle(int v):velocity(v){}
    Vehicle():velocity(10){}
};

class PlayerCharacter : virtual public Vehicle
{
public:
    PlayerCharacter():Vehicle(10){}
    virtual ~PlayerCharacter() = default;
};

class BonusCharacter : virtual private Vehicle
{
public:
    BonusCharacter():Vehicle(20){}
    virtual ~BonusCharacter() = default;
};

class GameCharacter : public BonusCharacter, public PlayerCharacter
{
public: 
    Vehicle _vehicle;
    ImoveableObject _powerup;
    GameCharacter() : Vehicle(40), BonusCharacter(){}
};
 
int main()
{
    GameCharacter gc; // who is responsible in instantiating the virtual base class? mosst derived which is the gameCharacter
    std::cout << gc.PlayerCharacter::Vehicle::velocity << std::endl;                    //40
    std::cout << "size of ImoveableObject: " << sizeof(ImoveableObject) << std::endl;   //1
    std::cout << "size of Vechicle: " << sizeof(Vehicle) << std::endl;                  //4
    std::cout << "size of PlayerCharacter: " << sizeof(PlayerCharacter) << std::endl;   //16
    std::cout << "size of BonusCharacter: " << sizeof(BonusCharacter) << std::endl;     //16
    std::cout << "size of GameCharacter: " << sizeof(GameCharacter) << std::endl;       //32
}

Imoveable: 1
Vehicle: 4 int = 4
PlayerCharacter: 8vptr + 4 int + 4 padding = 16
BonusCharacter: 8vptr + 4 int + 4 padding = 16
GameCharacter: 8 + 8 + 8 + 8 = 32

BonusCharacter - 8vptr = 8 
PlayerCharacter - 8vptr = 8
GameCharacter - 4 int + 1 imoveable + 3 padding = 8 
Vechicle - 4 int + 4 padding = 8 

--------------------------------------------------------------------------------
Quiz 2: 
=======
#include <iostream>

struct IObject
{
    unsigned short int cost() const { return 0; }
};

struct PowerUp : IObject
{
    PowerUp(unsigned short int v) : cost {v} {}
    PowerUp() : cost { 0 }{}
    virtual ~PowerUp() = default;
protected:
    unsigned short int cost;
};
struct HealthBonus :virtual PowerUp
{
public:
    HealthBonus() : PowerUp(1) {}
};
class ManaBonus : virtual PowerUp
{
public:
    ManaBonus() : PowerUp(2) {}
    unsigned short int cost() const
    {
        return PowerUp::cost;
    }
};
struct SpecialItem : HealthBonus, ManaBonus
{
    SpecialItem() : _owner{ new PowerUp } {}
    ~SpecialItem() { delete _owner; }
private:
    IObject* _owner;
    
};
int main()
{
    IObject* item = new SpecialItem;
    std::cout << "item->cost() = " <<item->cost() << std::endl;             //0
    
    std::cout << "IObject     = "  << sizeof (IObject) << std::endl;        //1
    std::cout << "PowerUp     = "  << sizeof (PowerUp) << std::endl;        //16
    std::cout << "HealthBonus = "  << sizeof (HealthBonus) << std::endl;    //24
    std::cout << "ManaBonus   = "  << sizeof (ManaBonus) << std::endl;      //24
    std::cout << "SpecialItem = "  << sizeof (SpecialItem) << std::endl;    //40
    
    delete item;
}

IObject: 1
PowerUp: 8vptr + 2 int + 6 padding = 16 
HealthBonus: 16 from powerup + 8vptr = 24 
ManaBonus: 16 from power + 8vptr = 24 
SpecialItem: 8 + 8 + 8 + 16 = 40

HealthBonus - 8vptr = 8 
ManaBonus - 8vptr = 8 
SpecialItem - 8 *_owner = 8 
PowerUp - 8vptr + 2 int + 6 padding = 16  
--------------------------------------------------------------------------------
Self Practice: 
==============

#include <iostream>

class A
{
public:
    int a;
    virtual ~A();
};

class B: virtual public A
{
public: 
    char c;
    virtual ~B();
};

class C: virtual public A
{
    float f;
};

class D: public B, public C 
{
public:
  int i;
  double d;
};

int main()
{
    std::cout << "A: " << sizeof(A) << std::endl;   //16
    std::cout << "B: " << sizeof(B) << std::endl;   //32
    std::cout << "C: " << sizeof(C) << std::endl;   //32
    std::cout << "D: " << sizeof(D) << std::endl;   //56
}

A: 8vptr + 4 int + 4 padd = 16 
B: 16 from A + 8vptr + 1 char + 7 padd = 32
C: 16 from A + 8vptr + 4 float + 4 padd = 32
D: 16 + 16 + 8 + 16 = 56

B - 8vptr + 1 char + 7 padd = 16
C - 8vptr + 4 float + 4 padd
                    + 4 int = 16
                    
D - 4 int + 4 padd + 8 double
                    8 double = 8 
A - 8vptr + 4int + 4 padd = 16 

--------------------------------------------------------------------------------

Chapter 1: 
----------
struct A{}  //0


Functional Programming
    - declarative programming paradigm

Imperative programming 
    - Unstructered programming (e.g assembly language)
        - Flow of execution can jump to various locations (goto)
        
    - Structured programming (e.g OS scripts)
        - Flow of execution is controlled by structured statements
          (selection, iteration, structured and unstructured jumps)
        
    - Procedural programming (e.g C)
        - Program is a collection of procedures (in C terminology: functions)
        - Flow of execution is controlled by structured statements 
    
    - Object-oriented programming(e.g. C++)
        - Program is a collection of procedures(C++:functions) and method
          (C++: member functions) that work on objects of class types 
          
    - Generic programming (e.g C++)
        - State of the program is split into separate objects 
        - Types use static polymorphism for better reuse (templates)
        
    - Template meta-programming(e.g. C++):
        - Using generic programming techniques to generate temporary source 
          code resulting in new data types, constants and compile-time behaviours

--------------------------------------------------------------------------------
Chapter 7 - SOLID:
==================
SRP - Single Responsibility principle: 

- A class should have only one reason to change. 
    - If a class has more than one responsibility, changes to one responsibility may impair the class's ability to meet others
    - e.g. changes to A causes rectange to changes for some reason, that change may force us to rebuild, retest, redeploy B. 

OCP - Open Close Principle: (Inheritance or Polymosphism)

- A class should be open for extension, close for modification. 
    - Close for modification is not allowing anyone to change the source code that is working by using keywords like const
    and final to protect its state from modification. 
    - Open for extension is to ablity to add new members or functionality as long as it does not contradict the original code design

LSP - Liscov substitution principle 

- Code that is valid for an object of a base type, should be valid for an object of the derived type. 
    - base class or interface class establishes a contract that lets user make assumption about any derived class 
    - A square is not a rectange. 
    
ISP - Interface segregation principle 

- Code should rely on the smallest possible interface related to the given function 
    - ISP informs us to break do-all interfaces into smaller interface segregating unrelated functionality 
    - avoid fat, do all interfaces and replace with role-specific interface. 

    - Class that have fat interface are classes whose interface are not cohesive. 

DIP - Dependendency inversion principle 

- High level source code should not limit low-level code. Both should depend on abstracts, not specific types. 

================================================================================

Chapter 8: Templates
--------------------

- non-template function match function first. 
- Then, compiler consider base templates 
- Last, compiler consider specialization of selected base. 

Chapter 9: Move semantic
------------------------

Locator values: 
--------------
- located in memory (have address)
- generally used on the left or right or assignment operator 

    - const value , even though they can be put only on the right side of the assignment 
    - name of variable, function, data member 
    - *ptr 
    - string literal ("ABC")
    - L value reference from a function int& f();
    - L value reference variable or parameter, int& x = i;
    - R value reference variable or a parameter 
    - A result of an operator that returns lvalue; int* p =&i;  *p = 5;
    - Any reference that has a name is an lvalue 
    
R-values: 
---------
- Not all lvalues can be used on the right side of the assignment operator 
- getting an Rvalue requires another function call, creating a copy of immutable object, or explicit reuse. 

    Pure r-values: 
        -literals values (immutable and copied), except string literals.  
        -Non reference result from a function, int f(); 
        - *this
        - Result of built in arithmetic, logical, comparison, address-of and some other operators. 
        
    
    Expiring value or explicit r -value:
        - R-value reference result from a function, int&& f();
        - R-value expression without a name 
        - Cast expression result in r-value, i.e. static_cast<int&&>(v); 
        - Data member of an object, or element of an array of object where the object is r-value itself
        
Lvalue reference: 
------------------
- An lvalue reference can bind to a lvalue
- Constant lvalue reference can bind to a lvalue or rvalue

Rvalue reference:
------------------
- An Rvalue reference can bind to a rvalue (pr-value or x-value)
- Constant rvalue reference can bind to const rvalue. They are often used to overload function for rvalue, 
but still support only the copy semantics. 

Copy semantic vs move semantic
------------------------------
- Function can be overloaded based only lvalue or rvalue reference nature of function argument

-Possibilites:
    - Lvalue maybe be used elsewhere, they must be copied. 
    - Lvalue or rvalue that are const must be copied 
    - Rvalues are destructible so that their value can be swapped.  
    
    
vector(const vector<T>& v) : _data{new T[ v._capacity]}, _size{v._size }, _capacity{v._capacity}
{
    std::copy(_data, _data + size, v._data);
}

vector<T>& operator=( const vector<T>& v)
{
    if (this != &v)
    {
        T* temp = new T[ v._capacity];
        std::copy( v._data , v._data + v.size , temp);
        delete[] _data;
        _data = temp;
        _size = v._size
        _capacity = v._capacity
    }

    return *this;
}
--------------------------------------------------------------------------------
Lecture 10: Forward referencing
================================

T         T&         T&&
--------------------------
int      int&        int&& 
int&     int&        int& 
int&&    int&        int&& 

<T>     Ref         T&&
-----------------------
int     &      ->   int& 
int     &&     ->   int&& 

int&    &      ->   int&
int&    &&     ->   int&&

int&&   &      ->   int&
int&&   &&     ->   int&&   

T&&     T
-------------
int&    int& 
int&&   int


std::swap 
---------
template<typename T> 
void swap(T& a, T& b)
{
    T temp{std::move(a)};
    a = std::move(b);
    b = std::move(temp);
}

std::move 
---------
int&& move(int& t)
{
    return static_cast<int&&>(t);
}

int&& move(int&& t)
{
    return static_cast<int&&>(t);
}

std::forward 
------------
template<typename T> 
std::forward(remove_reference_t<T>& t)
{
    return static_cast<T&&>(t);
}

std::remove_reference 
---------------------
template <typename T> struct remove_reference { using type = T; }; 
template <typename T> struct remove_reference<T&> { using type = T; };
template <typename T> struct remove_reference<T&&> { using type = T; };

template <typename T> 
using remove_reference_t = typename remove_reference<T>::type; 

std::move 
---------
template <typename T> 
remove_reference_t<T> && move(T&& t)
{
    return static_cast<remove_reference_t<T>&&>(t);
}
================================================================================
Midterm paper 
-------------

Qn 1:
-----
1a. In an object, data members of virtual base classes reside in memory at lower address than direct member of the class of the object. //False

1b. Non-virtual inheritance from multiple base classes that share the same non-virtual base class leads to the derived class having duplicated inherited members. //True 

1c. A template std::move() from the standard template library always returns object that are r-values, and has no side effects. //True

1d. An r-value reference object that is given a name can be bound to an r-value object, but itself behaves as an lvalue object. //True

1e. Following the OCP(the "O" in S.O.L.I.D.), we shouuld strive to create code open for modification, but closed from extension. //False 

1f. The C++ programming language supports virtual destructors, virtual member functions and virtual non-member functions, but not virtual constructors //False 

1g. Use of move semantics lets us, among other things, develop code that requires less memory. //True 

1h. It is possible to provide a definition to a pure virtual member function in a scope of a class where it has been original declared. //True 

1I. A proper implemented factory design pattern includes the create() function that return an instance of a class that contains a virtual table pointer //True 

1J. Calls to overload functions, includes calls to overload operators are resolved in run-time and therefore these are examples of dynamic dispatch. //False 

Qn 2:
------
template<typename T>
class my_basic_string
{
    size_t _length;
    T* _text;

public:
    // All required declarations are here 
};

using my_string = my_basic_string<char>;
using my_wstring = my_basic_string<wchar_t>;

//Define copy constructor outside of class definition 
template<typename T>
my_basic_string<T>::my_basic_string{ const my_basic_string& rhs }:_length{ rhs.length }, _text{ new T[rhs.length + 1] }
{
    std::copy(rhs._text, rhs.text + rhs.length + 1, _text);
}

//Define move constructor outside of the class definition 
template<typename T>
my_basic_string<T>::my_basic_string(my_basic_string&& rhs) :_length{ rhs._length }, _text{ rhs._text }
{
    rhs._length = 0;
    rhs._text = new T[1];
    rhs._text[0] = 0;
}

//Define the copy assignment operator outside of the class definition, make sure it causes no memory leaks without using try/catch. 
template <typename T>
my_basic_string<T>& my_basic_string<T>::operator=(const my_basic_string<T>& rhs)
{
    if (this != &rhs)
    {
        T* temp = new T[rhs._length + 1];

        std::copy(rhs._text, rhs._text + rhs._length + 1, temp);

        delete[] _text;
        _text = temp;
        _lenth = rhs.length;
    }

    return *this;
}

//Define move assignment operator outside of class definition
template <typename T>
my_basic_string<T>& my_basic_string<T>::operator=(my_basic_string&& rhs)
{
    if (this != &rhs)
    {
        std::swap(_length, rhs._length);
        std::swap(_text, rhs.text);
    }  

    return *this;
}

//Define the destructor outside of the class definition
template<typename T>
my_basic_string<T>::~my_basic_string()
{
    delete[] _text;
}

Qn 3:
-----
#include <iostream> 

struct B
{
	virtual void f1() { std::cout << "B1" << std::endl; }
	virtual void f2() { std::cout << "B2" << std::endl; }
};

struct D1 : public B
{
	void f1(double) { std::cout << "D11" << std::endl; }
	virtual void f2() { std::cout << "D12" << std::endl; }
	virtual void f3() { std::cout << "D13" << std::endl; }
};

struct D2 : public B
{
	virtual void f1(double) { std::cout << "D21" << std::endl; }
	void f2() { std::cout << "D22" << std::endl; }
	virtual void f3() { std::cout << "D23" << std::endl; }
};

int main()
{
	B b;
	D1 d1;
	D2 d2;

	B* bp_b = &b;
	B* bp_d1 = &d1;
	B* bp_d2 = &d2;
	D1* d1p_d1 = &d1;
	D2* d2p_d2 = &d2;

	bp_b->f1();			//B1
	bp_d1->f1();		//B1
	bp_d1->f2();		//D12
	bp_d2->f1();		//B1
	bp_d2->f2();		//D22
	d1p_d1->f1(3.0f);	//D11
	d1p_d1->f1(1.0f);	//D11
	d1p_d1->f2();		//D12
	d2p_d2->f1(2.0f);	//D21
	d2p_d2->f3();		//D23
}

Qn 4:
-----
#include <iostream> 

struct A
{
	A() { std::cout << "A" << std::endl; }
	virtual ~A() { std::cout << "~A" << std::endl; }
};

struct B : virtual A
{
	B() { std::cout << "B" << std::endl; }
	virtual ~B() { std::cout << "~B" << std::endl; }
};

struct C : virtual A
{
	C() { std::cout << "C" << std::endl; }
	virtual ~C() { std::cout << "~C" << std::endl; }
};

struct E : C
{
	C c;
	E() { std::cout << "E" << std::endl; }
	virtual ~E() { std::cout << "~E" << std::endl; }
};

int main()
{
	E e;		//A C A C E ~E ~C ~A ~C ~A
	(void)e;	
}

Qn 5:
-----
#include <iostream>
#include <string>

//#include "Logger.h"

//5a)
struct Logger
{
    virtual void log(const std::string&) = 0;
    virtual ~Logger() = default;
};

struct NullLogger : Logger
{
    void log(const std::string&) {}
};

class FormattedLogger : public Logger
{
    const std::string _prefix;
    const std::string _suffix;

public: 
    FormattedLogger(const std::string& prefix, const std::string& suffix) : _prefix{ prefix }, _suffix{ suffix }{}

    void log(const std::string& message) { std::cout << _prefix << message << _suffix << std::endl; }
};

void execute(Logger&& logger)
{
    logger.log("Before action.");
    std::cout << "Action!" << std::endl;
    logger.log("After action.");
}

int main()
{
    const std::string prefix{ '[' }, suffix{ ']' };
    execute(NullLogger{});
    std::cout << "\n";
    execute(FormattedLogger{ prefix, suffix });
}

5b. Name the SOLID design principle that you followed in devising this implementation, and explain how you applied it. //DIP

5c. sizeof(Logger) = 8 bytes. 

5d. class FormmatedLogger : public Logger{
    
5e. void execute (Logger&& logger){

================================================================================
Type erasure pattern
--------------------

#include <iostream>

/*******************************************************************************
 * Sword and Bow have the same implicit interface, but they do not inherit from
 * any common base class. These can easily fit into the same type-erased model.
 ******************************************************************************/

class Sword
{
public:
	Sword()
	{
		std::cout << "Sword()\n";
	}

	Sword(const Sword&)
	{
		std::cout << "Sword(copy)\n";
	}

	Sword(Sword&&)
	{
		std::cout << "Sword(move)\n";
	}

	~Sword()
	{
		std::cout << "~Sword()\n";
	}

	void attack()
	{
		std::cout << "\tAttacked with a sword!\n";
	}
};

class Bow
{
	int _arrows;
public:
	Bow(int arrows) : _arrows{ arrows }
	{
		std::cout << "Bow()\n";
	}

	Bow(const Bow& rhs) : _arrows{ rhs._arrows }
	{
		std::cout << "Bow(copy)\n";
	}

	Bow(Bow&& rhs) : _arrows{ rhs._arrows }
	{
		rhs._arrows = 0;
		std::cout << "Bow(move)\n";
	}

	~Bow()
	{
		std::cout << "~Bow()\n";
	}

	void attack()
	{
		std::cout << "\tAttacked with a bow; " << --_arrows << " arrows left.\n";
	}
};

/*******************************************************************************
 * Staff does not share the implicit interface, as its performAttack() function
 * name differs; it needs to be adapted with an Adaptor Design Pattern wrapper.
 ******************************************************************************/

class Staff
{
public:
	Staff()
	{
		std::cout << "Staff()\n";
	}

	Staff(const Staff&)
	{
		std::cout << "Staff(copy)\n";
	}

	Staff(Staff&&)
	{
		std::cout << "Staff(move)\n";
	}

	~Staff()
	{
		std::cout << "~Staff()\n";
	}

	// The name of this function does not match the implicit interface
	// (duck-typing) required by a Weapon::Model<T>::attack().
	void performAttack()
	{
		std::cout << "\tAttacked with a staff!\n";
	}
};

class StaffAdaptor
{
	Staff _instance;
public:
	void attack()
	{
		_instance.performAttack();
	}
};

/*******************************************************************************
 * Weapon is a type-erased class (it is not a template!). It hides an explicit
 * interface class IConcept, and a class template Model<T> for any type T that
 * we intend to erase if it matches the implicit interface.
 ******************************************************************************/

class Weapon
{
	struct IConcept
	{
		virtual void attack() = 0;
		virtual ~IConcept() = default;
	};

	IConcept* _concept;

	template <typename T>
	class Model : public IConcept
	{
		T _instance;
	public:
		template <typename Arg>
		Model(Arg&& arg) :
			_instance{
			// Perfect forwarding
			std::forward<Arg>(arg)
		}
		{
		}

		void attack() override
		{
			_instance.attack();
		}
	};

	Weapon(IConcept* concept) :
		_concept{ concept }
	{
	}

public:
	template <typename T, typename Arg>
	static Weapon create(Arg&& arg)
	{
		return Weapon
		{
			new Model<T>
			{
				// Perfect forwarding
				std::forward<Arg>(arg)
			}
		};
	}

	void attack()
	{
		_concept->attack();
	}

	virtual ~Weapon()
	{
		delete _concept;
	}
};

// Not a template, even though the actual type of a weapon is not known!
void attack(Weapon w)
{
	w.attack();
}

int main()
{
	Sword s;
	Bow b{ 10 };
	StaffAdaptor t;

	std::cout << "Attack with the copies!\n";
	attack(Weapon::create<Sword>(s));
	attack(Weapon::create<Bow>(b));
	attack(Weapon::create<StaffAdaptor>(t));

	std::cout << "Attack with the originals!\n";
	attack(Weapon::create<Sword>(std::move(s)));
	attack(Weapon::create<Bow>(std::move(b)));
	attack(Weapon::create<StaffAdaptor>(std::move(t)));

	std::cout << "Attack with a conversion-constructed bow!\n";
	attack(Weapon::create<Bow>(100));
}

================================================================================
Bitwise Operation: 
------------------
Decimal     Binary  Octal   Hexadecimal          
0           0000    0       0
1           0001    1       1
2           0010    2       2
3           0011    3       3
4           0100    4       4
5           0101    5       5
6           0110    6       6
7           0111    7       7
-------------------------------------------- Oct 
8           1000    10      8
9           1001    11      9
--------------------------------------------- Hex 
10          1010    12      A 
11          1011    13      B
12          1100    14      C 
13          1101    15      D
14          1110    16      E
15          1111    17      F 

AND & 
------------
x & y = z 
0   0   0
1   0   0
0   1   0
1   1   1 

OR |
------------
x | y = z 
0   0   0 
1   0   1 
0   1   1
1   1   1

NOT ~
-------
~x = y 
 0   1 
 1   0 

XOR ^
----------
x ^ y = z
0   0   0 
1   0   1
0   1   1
1   1   0

Bit shift left <<  
------------------------------
0b0000'0001 << 3 = 0b0000'1000

Bit shift right >> 
-------------------------------
0b1000*0000 >> 7 = 0b0000*0001

--------------------------------------------------------------------------------
#include <iostream>

int main()
{
    unsigned int a = 0b0011'0110;
    std::cout << "A: " << std::dec << a << std::endl;       // 2+4+16+32 = 54

    unsigned int b = 0b0011'0110;
    std::cout << "B: " << std::oct << b << std::endl;       // 110'110 = 66

    unsigned int c = 0b0001'1111;                           
    std::cout << "C: " << std::hex << c << std::endl;       // 1+2+4+8+16 = 31 = 1F 

    //-----------OR----------------------

    unsigned int d = 0b0011'1110;
    unsigned int e = 0b0001'1110;

    unsigned int f = d | e;     //0b0011'1110 = 62 in dec, 76 in oct, 3e in hex 

    std::cout << "F: " << std::dec << f << " " << std::oct << f << " " << std::hex << f << std::endl; //62 76 3e 

    //------------AND-----------------------

    unsigned int g = d & e;     //0b0001'1110 = 30 in dec, 36 in oct, 1e in hex 

    std::cout << "G: " << std::dec << g << " " << std::oct << g << " " << std::hex << g << std::endl; //30 36 1e

    //------------NOT------------------------
    
    unsigned int h = 0b0101'1111;    //95 in dec, 137 in oct, 5F in hex 
    unsigned int i = 0b0010'1011;    //43 in dec, 53 in oct, 2B in hex

    unsigned int j = static_cast<unsigned short int>(~h);   //0b1010'0000    32+128 = 160 in dec, 240 in oct, A0 in hex 
    unsigned int k = i & j;                                 //0b0010'0000    32 in dec, 40 in oct, 20 in hex 

    std::cout << "H: " << std::dec << h << " " << std::oct << h << " " << std::hex << h << std::endl; //95 137 5f
    std::cout << "I: " << std::dec << i << " " << std::oct << i << " " << std::hex << i << std::endl; //43 53 2B
    std::cout << "J: " << std::dec << j << " " << std::oct << j << " " << std::hex << j << std::endl; //65440 177640 ffa0
    std::cout << "K: " << std::dec << k << " " << std::oct << k << " " << std::hex << k << std::endl; //32 40 20

    //-------------XOR---------------------------

    unsigned int l = 0b1010'1010;   //170 in dec, 252 in oct, AA in hex
    unsigned int m = 0b0011'0011;   //51 in dec, 63 in oct, 33 in hex

    unsigned n = l ^ m;             //0b1001'1001 153 in dec, 231 in oct, 99 in hex 

    std::cout << "L: " << std::dec << l << " " << std::oct << l << " " << std::hex << l << std::endl; //170, 252, AA 
    std::cout << "M: " << std::dec << m << " " << std::oct << m << " " << std::hex << m << std::endl; //51, 63, 33
    std::cout << "N: " << std::dec << n << " " << std::oct << n << " " << std::hex << n << std::endl; //153, 61, 99

    //-----------Bitwise shift left---------------

    unsigned char o = 3;

    unsigned int p = 0b0000'1100;   //12 in dec, 14 in oct, C in hex 
    unsigned int q = p ^ (1 << o);  //0b0000'1000 
                                    //0b0000'0100   4 in dec, 4 in oct, 4 in hex 

    std::cout << "P: " << std::dec << p << " " << std::oct << p << " " << std::hex << p << std::endl; //12, 14, C 
    std::cout << "Q: " << std::dec << q << " " << std::oct << q << " " << std::hex << q << std::endl; //4, 4, 4

    //-----------Bitwise shift right ------------

    unsigned int r = 0b0000'1100; //12 in dec, 14 in oct, C in hex 
    unsigned int s = 0b0000'0101; //5 in dec, 5 in oct, 5 in hex 

    unsigned int t = r >> 2;    //0b0000'0011 3 in dec, 3 in oct, 3 in hex
    unsigned int u = t ^ s;     //0b0000'0110 6 in dec, 6 in oct, 6 in hex 

    std::cout << "R: " << std::dec << r << " " << std::oct << r << " " << std::hex << r << std::endl; //12, 14, c
    std::cout << "S: " << std::dec << s << " " << std::oct << s << " " << std::hex << s << std::endl; //5, 5, 5
    std::cout << "T: " << std::dec << t << " " << std::oct << t << " " << std::hex << t << std::endl; //3, 3, 3
    std::cout << "U: " << std::dec << u << " " << std::oct << u << " " << std::hex << u << std::endl; //6, 6, 6
    
    //---------Bitwise shift right ---------------

    unsigned char v = 0;
    unsigned int w = 0b0000'1000;       //8 

    while (w)    //y = 8 
    {
        v++;
        w = w >> 1;

        if (w == 0)
        {
            std::cout << "Mask was empty" << std::endl;
        }
        else
        {
            std::cout << "Bit index was: " << (v - 1) << std::endl;    //bit index was: 3 
        }

    }

    //---------Change 2nd last bit from 1 to 0 (Controlled clearing with 0) ------------

    unsigned int A = 0b0000'0110;  //want to change last 2nd from 1 to 0 
    unsigned int B = 0b0000'0010;  //create a mask 

    unsigned int C = A & ~B;       //~B = 0b1111'1101
                                   //C = 0b0000'0100

    std::cout << "C: " << std::dec << C << std::endl; // 4 = 0b0000'0100

    //----------Change last 4th bit from 0 to 1 (Controlled setting with 1) ------------

    unsigned int D = 0b000'0110;  //6
    unsigned int E = 0b000'1000;  //create a mask

    unsigned int F = D | E;       //0b0000'1110

    std::cout << "F: " << std::dec << F << std::endl; //14 

    //---Change last 3rd bit from 1 to 0 or 0 to 1 (Controlled negation with 1) --------

    unsigned int G = 0b0000'1110;   //
    unsigned int H = 0b0000'0100;   //create a mask 

    unsigned int I = G ^ H;         //0b0000'1010

    std::cout << "I: " << std::dec << I << std::endl; //10 = 0b0000'1010 

    //-----------Bitwise shift left------------
    unsigned int J = 0b0000'0001;
    unsigned int K = J << 3;

    std::cout << "K: " << std::dec << K << std::endl; // 8 = 0b0000'1000;

    //-----------Bitwise shift right ------------
    unsigned int L = 0b1000'0000;
    unsigned int M = L >> 7;

    std::cout << "M: " << std::dec << M << std::endl; // 1 = 0b0000'0001;
}

================================================================================
Pointer to member functions:
----------------------------
#include <iostream>
#include <string>

class myClass
{
public: 
    std::string hello {"Hello "};
    std::string world {"World\n"};
    
    void printHello() {std::cout << hello; };
    void printWorld() {std::cout << world; };
};

int main()
{
    //Pointer to member functions
    //  void (myClass::*f)();

    using F = void (myClass::*)();
    F f = &myClass::printHello;

    myClass obj;
    (obj.*f)();                     //hello

    f = &myClass::printWorld;
    myClass* obj_ptr = &obj;
    (obj_ptr->*f)();                //world 
    
    //Pointer to a data member
    //std::string (myClass::*d);

    using D = std::string (myClass::*);
    
    D d = &myClass::hello;
    (obj.*d) = "Bye ";              
    obj.printHello();               //Bye
    
    d = &myClass::world;
    (obj_ptr->*d) = "bye!";         
    obj_ptr->printWorld();          //bye!
    
}

================================================================================
Unique Pointer
--------------
#include <iostream>
#include <memory>

class myClass 
{
public:
    void print() {std::cout << "Hello World!" << std::endl;}
};

//by lvalue ref of an object 
void print1(myClass& obj){obj.print();}

//by universal ref of a pointer 
template <typename T> 
void print2(T&& obj_ptr){obj_ptr->print();}

//by-value of a pointer
void print3(std::unique_ptr<myClass>ptr){ptr->print();}

int main()
{
    //std::unique_ptr<myClass> up{new myClass};
    std::unique_ptr<myClass> up = std::make_unique<myClass>();
    
    print1(*up);        //Hello World - deference
    print1(*up.get());  //Hello World 
    
    print2(up);         //Hello World - smart pointer 
    print2(up.get());   //Hello World - pointer
    
    print3(std::move(up));  //Hello World - moved smart pointer 
    //print3(up);         //error , takes in parameter by value , means by copy 
}

================================================================================
Shared Pointer:
---------------
//Shared Pointer 
#include <iostream>
#include <memory>

class myClass 
{
public:
    void print() {std::cout << "Hello World!" << std::endl;}
};

//by lvalue ref of an object 
void print1(myClass& obj){obj.print();}

//by universal ref of a pointer 
template <typename T> 
void print2(T&& obj_ptr){obj_ptr->print();}

//by-value of a pointer
void print3(std::shared_ptr<myClass> ptr){ptr->print();}

int main()
{
    //std::unique_ptr<myClass> up{new myClass};
    std::shared_ptr<myClass> sp = std::make_shared<myClass>();
    
    print1(*sp);        //Hello World 
    print1(*sp.get());  //Hello World 
    
    print2(sp);         //Hello World 
    print2(sp.get());   //Hello World 
    
    print3(sp);         //Hello World 
}

================================================================================
Weak Pointer:
-------------
#include <iostream>
#include <memory>

class myClass 
{
public:
    void print() {std::cout << "Hello World!" << std::endl;}
};

//by lvalue ref of an object 
void print1(myClass& obj){obj.print();}

//by universal ref of a pointer 
template <typename T> 
void print2(T&& obj_ptr){obj_ptr->print();}

//by-value of a pointer
void print3(std::weak_ptr<myClass> ptr)
{
    if(std::shared_ptr<myClass> sp = ptr.lock())
    {
        sp->print();
    }
}

int main()
{
    //std::unique_ptr<myClass> up{new myClass};
    std::shared_ptr<myClass> sp = std::make_shared<myClass>();
    
    print1(*sp);        //Hello World 
    print1(*sp.get());  //Hello World 
    
    print2(sp);         //Hello World 
    print2(sp.get());   //Hello World 
    
    print3(sp);         //Hello World 
}

================================================================================
Weak pointer - lock
-------------------
#include <iostream>
#include <memory>

class myClass 
{
public:
    void print() {std::cout << "Hello World!" << std::endl;}
};

//by lvalue ref of an object 
void print1(myClass& obj){obj.print();}

//by universal ref of a pointer 
template <typename T> 
void print2(T&& obj_ptr){obj_ptr->print();}

//by-value of a pointer
void print3(std::weak_ptr<myClass> ptr)
{
    if(std::shared_ptr<myClass> sp = ptr.lock())
    {
        sp->print();
    }
    else
    {
        std::cout << "lock unsuccessful!" << std::endl;
    }
}=

int main()
{
    //std::unique_ptr<myClass> up{new myClass};
    std::shared_ptr<myClass> sp = std::make_shared<myClass>();
    
    print1(*sp);        //Hello World 
    print1(*sp.get());  //Hello World 
    
    print2(sp);         //Hello World 
    print2(sp.get());   //Hello World 
    
    std::weak_ptr<myClass> wp{sp};
    sp.reset();
    
    print3(wp);        //lock unsuccessful!
}

================================================================================
//Allocator
#include <iostream>
#include <vector>

template<typename T>
class MyAllocator
{
public:
	using value_type = T;

	T* allocate(size_t count)
	{
		std::cout << "Allocated " << count << " objects" << std::endl;
		return (T*)::operator new(count * sizeof(T));

	}

	void deallocate(T* ptr, size_t count)
	{
		std::cout << "Deallocated " << count << " objects" << std::endl;
		::operator delete(ptr, count * sizeof(T));
	}
};

int main()
{
	std::vector<int, MyAllocator<int> > v;	//std::vector takes in 2 parameters

	for (int i = 0; i < 17; ++i)
	{
		v.push_back(i);
	}

	for (const int& i : v)
	{
		std::cout << i << " ";
	}

	std::cout << std::endl;

	std::vector<int> v2;
    
    return 0;
}
================================================================================
#include <iostream> 

//Create a new type to store the sequence 
template <size_t...Ns>
struct index_sequence
{
    static void print()
    {
        size_t numbers[] = {Ns...};
        for(const auto& num : numbers)
        {
            std::cout << num << " ";
        }
        
        std::cout << std::endl;     
        
    }
}

//Make sequence implementation
template <size_t counter, size_t Rest>
struct make_sequence_impl
{
    using type = typename make_sequence_impl<counter-1, counter - 1, Rest...>::type;
};

template<size...Rest>
struct make_sequence_impl<0, Rest...>
{
    using type = index_sequence<Rest...>;
};

template<size_t T>
using make_sequence = make_sequence_impl<N>::type;

int main()
{
    make_sequence<10>::print();
}

================================================================================
//Trialing return types
//Callable objects, std::functions

#include <iostream>
#include <functional>	// std::function

int sum = 0;

void add(int x)
{
	sum += x;
}

struct Adder
{
	void operator()(int x)
	{
		sum += x;
	}
};

struct Adder2
{
	Adder2(int& sum) : _sum{ sum }
	{

	}

	void operator()(int x)
	{
		_sum += x;
	}
private:
	int& _sum;
};

struct Adder3
{
	void add(int x)
	{
		sum += x;
	}
};

int main()
{
	std::cout << "\nFunction:" << std::endl;
	{
		std::function< void(int) > myadd = add;

		myadd(1);
		myadd(2);
		std::cout << sum << std::endl;
	}

	std::cout << "\nFunction pointers:" << std::endl;
	{
		std::function<void(int) > myadd = &add;

		myadd(10);
		myadd(20);

		std::cout << sum << std::endl;
	}

	std::cout << "\nFunctor" << std::endl;
	{
		std::function<void(int) > myadd = Adder{};

		myadd(100);
		myadd(200);

		std::cout << sum << std::endl;
	}

	std::cout << "\nFunctor with a state" << std::endl;
	{
		int sum = 0;
		std::function<void(int) > myadd = Adder2{ sum };

		myadd(1000);
		myadd(2000);

		std::cout << sum << std::endl;
	}

	std::cout << "\nLambda expression with a capture" << std::endl;
	{
		int sum = 0;
		std::function<void(int) > myadd = [&sum](int x) {sum += x;};	//capture sum by reference

		myadd(10000);
		myadd(20000);

		std::cout << sum << std::endl;
	}

	std::cout << "\nLambda expression without a capture" << std::endl;
	{
		std::function<void(int)> myadd = [](int x) {sum += x;};

		myadd(1);
		myadd(2);

		std::cout << sum << std::endl;
	}

	std::cout << "\nLambda with deduced parameters without a capture" << std::endl;
	{
		std::function<void(int) > myadd = [](auto x) {sum += x; };

		/*
		template<typename T>
		void XXX(T)
		{
			sum+= x;
		}

		*/

		myadd(1);
		myadd(2);

		std::cout << sum << std::endl;
	}

	std::cout << "\nMember function" << std::endl;
	{
		std::function<void(Adder3&, int) > myadd = std::mem_fn(&Adder3::add);

		Adder3 adder;

		myadd(adder, 10);
		myadd(adder, 20);

		std::cout << sum << std::endl;
	}
}
================================================================================
//Type Erasure Pattern (TEP)


================================================================================
SFINAE //Substitution failure is not an error 


================================================================================
//std::optional
//std::nullopt 
//std::make_optional

#include <iostream>
#include <optional>

std::optional<int> divide(int x, int y)
{
	return (y == 0) ? std::nullopt : std::make_optional<int>(x / y);
}

int main()
{
	int i;
	std::cin >> i;

	auto r = divide(10, i);
	if (r)
	{
		std::cout << (*r) << std::endl;
	}
	else
	{
		std::cout << "Division by 0" << std::endl;
	}

}

--------------------------------------------------------------------------------
//std::any
//std::any_cast 
//std::bad_any_cast

#include <any>
#include <iostream>

int main()
{
	std::any a = 10;

	try 
	{
		int i = std::any_cast<int>(a);
		std::cout << i << std::endl;
	}
	catch (const std::bad_any_cast& a)
	{
		std::cout << "Error: " << a.what() << std::endl;
	}
}

--------------------------------------------------------------------------------
//std::ignore

#include <iostream>
#include <tuple>

int main()
{
	decltype(std::ignore) i;
	i = 20;

	std::ignore = 10;
}

--------------------------------------------------------------------------------
//std::ignore

#include <iostream>

struct my_ignore_t
{
	template<typename T>
	my_ignore_t& operator = (T&&)
	{
		return *this;
	}
}myignore;

int main()
{
	decltype(myignore) i;
	i = 20;

	myignore = 10;
}

--------------------------------------------------------------------------------
//std::reference_wrapper

#include <functional>
#include <vector>
#include <iostream>

int main()
{
	int i = 10;
	int j = 20;

	std::vector<std::reference_wrapper<int>> v = { std::ref(i), std::ref(j) };

	for (int& r : v)
	{
		r += 100;
	}

	std::cout << i << ", " << j << std::endl;   //110, 120
}

--------------------------------------------------------------------------------
//std::reference_wrapper

#include <functional>
#include <vector>
#include <iostream>

template<typename T>
class ref_wrapper
{
	T& _ref;
public:
	ref_wrapper(T& ref) : _ref{ ref }
	{

	}

	operator T& () const
	{
		return _ref;
	}
};

template<typename T>
auto ref_ref(T& t)
//ref_wrapper<T> ref_ref(T& t)
{
	return ref_wrapper<T>(t);
}

int main()
{
	int i = 10;
	int j = 20;

	std::vector<ref_wrapper<int>> v = { ref_ref(i), ref_ref(j) };

	for (int& r : v)
	{
		r += 100;
	}

	std::cout << i << ", " << j << std::endl;
}

--------------------------------------------------------------------------------
//std::tuple
#include <tuple>
#include <iostream>

std::tuple<int, bool> divide(int x, int y)
{
	if (y == 0)
	{
		return { 0, false };
	}
	
	return {x / y, true};
}

int main()
{
	int i;
	std::cin >> i;

	bool success;
	int value;

	std::tuple<int&, bool&> t{ value, success };
	t = divide(10, i);

	if (success)
	{
		std::cout << value << std::endl;
	}
	else
	{
		std::cout << "Division by 0!" << std::endl;
	}

	std::cout << "Done!" << std::endl;
}
--------------------------------------------------------------------------------
//std::tie 
#include <tuple>
#include <iostream>

std::tuple<int, bool> divide(int x, int y)
{
	if (y == 0)
	{
		return { 0, false };
	}
	
	return {x / y, true};
}

int main()
{
	int i;
	std::cin >> i;

	bool success;
	int value;

	std::tie(value, success) = divide(10, i);

	if (success)
	{
		std::cout << value << std::endl;
	}
	else
	{
		std::cout << "Division by 0!" << std::endl;
	}

	std::cout << "Done!" << std::endl;
}

--------------------------------------------------------------------------------
//std::ignore 
#include <tuple>
#include <iostream>

std::tuple<int, bool> divide(int x, int y)
{
	if (y == 0)
	{
		return { 0, false };
	}
	
	return {x / y, true};
}

int main()
{
	int i;
	std::cin >> i;

	bool success;

	std::tie(std::ignore, success) = divide(10, i);

	if (success)
	{
		std::cout << "Division worked fine." << std::endl;
	}
	else
	{
		std::cout << "Division by 0!" << std::endl;
	}

	std::cout << "Done!" << std::endl;
}

--------------------------------------------------------------------------------
#include <tuple>
#include <iostream>

std::tuple<int, bool> divide(int x, int y)
{
	if (y == 0)
	{
		return { 0, false };
	}

	return { x / y, true };
}

int main()
{
	int i;
	std::cin >> i;

	//structered binding
	auto [value, success] = divide(10, i);
	(void)value;

	if (success)
	{
		std::cout << "Division worked fine." << std::endl;
	}
	else
	{
		std::cout << "Division by 0!" << std::endl;
	}

	std::cout << "Done!" << std::endl;
}

--------------------------------------------------------------------------------
//structured binding 
#include <tuple>
#include <iostream>

struct Data
{
	int x;
	int y;
};

int main()
{
	auto [t1, t2] = std::tuple<int, int>{1, 2};
	std::cout << t1 << ", " << t2 << std::endl;

	auto [p1, p2] = std::pair<int, int>{ 3,2 };
	std::cout << p1 << ", " << p2 << std::endl;

	int a[] = { 5,6 };
	auto [a1, a2] = a;
	std::cout << a1 << ", " << a2 << std::endl;

	Data s = { 7,8 };
	auto [s1, s2] = s;
	std::cout << s1 << ", " << s2 << std::endl;
}

--------------------------------------------------------------------------------
//iterators
#include <tuple>
#include <iostream>

int main()
{
	int a[] = { 0,1,2,3,4,5,6,7,8,9 };
	const size_t count = sizeof(a) / sizeof(*a);

	int* begin = a;
	int* end = a + count;

	while (begin != end)
	{
		std::cout << *begin++ << ", ";
	}

	std::cout << std::endl;
}
================================================================================
/*
Tuple
std::tie()

structured binding 

std::for_each 
std::begin
std::end
std::count 
std::count_if 
std::copy_if()
stf::copy()
*/

#include <algorithm>
#include <tuple>
#include <iostream>
#include <vector>
#include <map>

namespace ns
{
	template <typename TIter, typename F>
	void for_each(TIter begin, const TIter end, F f)
	{
		while (begin != end)
		{
			f(*begin++); 
		}
	}

	template<typename TIter, typename TValue>
	size_t count(TIter begin, TIter end, TValue value)
	{
		size_t result = 0;
		while (begin != end)
		{
			if (*begin++ == value)
			{
				++result;
			}
		}
		return result;
	}

	template<typename TIter, typename F>
	size_t count_if(TIter begin, const TIter end, F f)
	{
		size_t result = 0;

		while (begin != end)
		{
			if (f(*begin++))
			{
				++result;
			}
		}

		return result;
	}

	template<typename TInIter, typename TOutIter, typename F>
	void copy(TInIter ibegin, TInIter iend, TOutIter obegin, F f)
	{
		while (ibegin != iend)
		{
			if (f(*ibegin))
			{
				*obegin++ = *ibegin;
			}
			*ibegin++;
		}
	}
}

int main()
{
	auto print = [](int value)
	{
		std::cout << value << ", ";
	};

	std::vector<float> vf = { 1.0f, 2.0f, 3.5f };
	ns::for_each(std::begin(vf), std::end(vf), print);
   	std::cout << std::endl;

    //-------------------------------------------------------
    
	int a[] = { 0,5,2,3,4,5,6,7,8,9 };	

	ns::for_each(std::begin(a), std::end(a), print);
	std::cout << std::endl;

    //-------------------------------------------------------
    
	std::cout << ns::count(std::begin(a), std::end(a), 5);
	std::cout << std::endl;

    //-------------------------------------------------------
    
	auto isEven = [](auto x)
	{
		return x % 2 == 0;
	};

	size_t n = ns::count_if(std::begin(a), std::end(a), isEven);
	std::cout << n << std::endl;

    //-------------------------------------------------------
    
	std::vector<int> vi;
	vi.resize(n);

	ns::copy(std::begin(a), std::end(a), std::begin(vi), isEven);
	std::cout << std::endl;
}

================================================================================
#include <iostream>
#include <functional> 
#include <algorithm>
#include <numeric>
#include <execution>
#include <array>
#include <vector>
#include <deque>
#include <tuple>
#include <cmath>

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

	//std::cout << "Reduce : " << std::reduce(std::execution::par, std::begin(d), std::end(d), 0) << std::endl;

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
	/*
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
	*/

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

	std::cout << std::apply(power, get_parameters()) << std::endl;
}



