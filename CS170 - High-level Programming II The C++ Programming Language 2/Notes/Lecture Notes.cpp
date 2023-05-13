*******************************Lesson 1*****************************************
/*Comment*/
//Comment
// two types of commenting

#include <cmath> // c++ no need.h e.g. <math.h>
#include <cstdio>

int main()
{
  printf("Hello world!\n");
  //return 0;   C++ no need return for main but other function need  
}

//to compile: g++ -Wall -Wextra -Werror -Wconversion -pedantic -std=c++11 -o main.exe main.cpp

#include <iostream>

int main()
{
  std::cout << "Hello world" << std::endl
  
  (std::cout << "Hello world") << std::endl;  //same as line 22
  
  std::cout << "Hello world"; //same as line 22
  
    /*
    std::ostream operator <<(std::ostream cout, char* text)
    {
      printf(text);
      return cout;
    }
  */
  
  std::cout << std::endl; //same as line 22
  
  /*
    std::ostream operator <<(std::ostream cout, endl)
    {
      printf("\n");
      fflush(cout);
      return cout;
    }
  */
}

#include <iostream>

int main()
{
  int i;
  std::cin >> i;        //input:10
  
  std::cout << std::setw(10) << (i+1) << std::endl;    //output 10+i=11
  //setw = set width to 10
}

//compile with vs: run vs, change folder path, type:

//std:c++17: cl /W4 /WX /Za /nologo /EHa /std:c++17 /Femain.exe main.cpp
//std:c++14: cl /W4 /WX /Za /nologo /EHa /std:c++14 /Femain.exe main.cpp
//std:c++11: cl /W4 /WX /Za /nologo /EHa /Femain.exe main.cpp  

#include <iostream>

int main()
{
  for( int i = 0; i<10; ++i)
  {
    std::cout << float (i)2  << ' '; //dont use casting float(i) in c++
    
    std::cout << static_cast<float>(i) /2 << ' '; //only use static_cast instead  
  }
  
  std::cout << std::endl; //output: 0 0.5 1.0 1.5 2.0 2.5 3.0 3.5 4.0 4.5
  
  /*
    
    const float f = 1.0f;
    f(const_cast<float>(f))   //const_cast, purpose: remove const. never use it. 
    
  */
  
  //covered later: reinterpret_cast, dynamic_cast, any_cast
  
  
  /* in C
  
    int a = 10;
    int b = 20;
    
    printf("%d %d", a, b);
    
    int c = a + b;
    
    printf("%d, c);
  
  */
}

#include <iostream>

int main()
{
  bool x = (10 < 20);
  
  std::cout << (!x)? "false" : "true" << std:: endl;  //output: true
  
  std::cout << x << std:: endl; //output: 1
}

#include <iostream>

struct X
{
  int i;
};

int main()
{
  X x;    //C++ no need to write struct 
  (void)x;
}

*********************************Lesson 2***************************************

//in C:

const in i = 10;  
const int* const p = &i;

//in C++

#include <iostream>

int main()
{
  const int i = 10; 
  int arr[i] = {0, 1, 2, 3, 4}; //in C++ can use const int i into array, C cant
  
  (void)i;
  (void)arr;
}  



L-value references (C++ convert to C under the hood)
____________________________________________________
//In c 

#include <iostream>

// passing parameters in C++
// - by copy
// - by a pointer 
// - by a reference (by an l-value reference)

//References
// - under the hood they are pointers
// - the compiler hides a pointer syntax
// - a reference is never NULL
// - a reference cannot be re-pointer to another object

/*
 void my Swap(int* const pa, int* const pb)
 {
    int t = *a;
    *a - *b;
    b = t;
*/
void mySwap(int*a, int*b)       //Normal coding like in C
{
  int t = *a;
  *a = *b;
  *b = t;
}

int main()
{
  int a = 10;
  int b = 20;
  mySwap(&a,&b);=

  std::cout<< a << "" << b << std::endl;
}

--------------------------------------------------------------------------------
#include <iostream>

void mySwap(int& a, int& b)     //C++
{
  int t = a;
  a = b;
  b = t;
}

int main()
{
  int a = 10;
  int b = 20;
  mySwap(a, b);

  std::cout<< a << "" << b << std::endl;
}

--------------------------------------------------------------------------------
#include <iostream>

void mySwap(int& a, int& b)     //C++
{
  int t = a;
  a = b;
  b = t;
}

int main()
{
  int a = 10;
  int b = 20;
  mySwap(a + 1, b);     //cannot &(a+1), &b
  
  //Binding (mean r is an "alias" of a)
  //right-hand object must be an l-value
  
  int& r = a;     // r is an alias of a, 
  r = r * 100;    //whatever assign to r changes a
  
  or 
  
  //assignment
  //right-hand object can be l or r value 
  r = a + 1;  
  
  or
  
  int &r = a;
  r = a;
  
  std::cout<< a << "" << b << std::endl;
}
-------------------------------------------------------------------------------

#include <iostream>

void f(int& x)
{
  std::cout <<sizeof(x) <<std::endl;
}

int main()
{
  int a = 10;
  f(a);
}

--------------------------------------------------------------------------------
#include <iostream>

void f(int& x)
{
  std::cout <<sizeof(x) <<std::endl;
}

int main()
{
  int a = 10;
  f(a);
}

--------------------------------------------------------------------------------
#include <iostream>

void f(const int& x)  //variable init as float but can pass through const int&
{
  std::cout <<sizeof(x) <<std::endl;
}

int main()
{
  float a = 10;
  f(a);
}

--------------------------------------------------------------------------------
#include <iostream>

//STACK STRUCTURE

//text
//data
//heap
//stack

//  &x
//  return address 
//  double x
void f(double& d)  //variable init as float but can pass through const int&
{
  std::cout <<sizeof(d) <<std::endl;
}

int main()
{
  double x;
  f(x);
}

--------------------------------------------------------------------------------
#include <iostream>

//STACK STRUCTURE

//text
//data
//heap
//stack

//  &(double)x
//  return address
//  (double) x 
//  int x
void f(const double& d)//variable init as float but can pass through const int&
{
  std::cout <<sizeof(d) <<std::endl;
}

int main()
{
  int x;
  f(x);
}

--------------------------------------------------------------------------------
#include <iostream>

void f(int (&a)[4])  //passing by reference, a is a reference of 4 array of int 
{
  std::cout <<sizeof(a) <<std::endl;
}

int main()
{
  int arr[] = {0, 1, 2, 3};
  f(arr);     //array passed by reference do not decay
}

--------------------------------------------------------------------------------
#include <iostream>

//b=20
//a=10
//return value = &a/&b
//return address

int& my_max(int a, int b)
{
  if(a>0)
  {
    return a; //return reference to local parameters
  }
  return b;
}

int main()
{
  std::cout << my_max(10, 20) << std::endl;
}

--------------------------------------------------------------------------------
#include <iostream>

int& my_max(int& a, int& b)
{
  if(a>0)
  {
    return a;
  }
  return b;
}

int main()
{
  int x = 10;
  int y = 20;
  std::cout << my_max(x, y) << std::endl;
}

- if accept parameter as reference, if parameter is const, passed in value can 
be another type

- array pass by reference do not decay

--------------------------------------------------------------------------------
NAMESPACES: //Keyword in C++
__________

#include <iostream>

int v = 10;

int main()
{
  std::cout << v << std::endl;    //output:10;
}

--------------------------------------------------------------------------------
#include <iostream>
#include "my.h"

namespace errorReporting
{
  int v = 10;
}

int v = 20;

int main()
{
  int v = 30;
  std::cout << errorReporting::v << std::endl;     //output:10
}

--------------------------------------------------------------------------------
#include <iostream>
#include "my.h"

namespace errorReporting
{
  int v = 10;
}

int v = 20;

int main()
{
  int v = 30;
  std::cout << v << std::endl;     //output:30
}

--------------------------------------------------------------------------------
#include <iostream>
#include "my.h"

namespace errorReporting
{
  int v = 10;
}

int v = 20;

int main()
{
  int v = 30;
  std::cout << ::v << std::endl;     //output:20
  (void)v;
}

--------------------------------------------------------------------------------
#include <iostream>
#include "my.h"

namespace errorReporting
{
  int v = 10;
  namespace File
  {
    int v = 40;
  }
}

int v = 20;

int main()
{
  int v = 30;
  std::cout << errorReporting::File::v << std::endl;     //output:40
  (void)v;
}

--------------------------------------------------------------------------------
#include <iostream>

namespace X
{
  int v = 10;
}

namespace X
{
  int a = 1;
  int b = 2;
}

namespace X
{
  double d = 2.0;
}

#include <iostream> 

using namespace std;

int main()
{
  std::cout << "Hello World!" << std::endl;
}

never create any definition in NAMESPACE 
never import the whole namespace

lesson Overall for namespace:

- A namespace provides a scope for namespace (variables, types, function,..)

- each identifier from a namespace has a full ("qualiified") name, byt we can 
use a shorter name relative to our current namespace

- Namespaces can be nested
- Namespaces do not have to be continuous
- Namespaces can be anonymous (unnamed)
/*
  namespace XYZ;
  {
  
  }

  using namespace XYZ;
*/

- Identifiers from naespace can be imported into current scope: 
  using namespace NAME;
  
- NEVER (EVER!) define anything in std::
- Never import everything from naespaces you do not know in there entirely

*********************************Lesson 3***************************************
Dynamic memory Allocation:
__________________________

int main()
{
  double* d = (double*)malloc(sizeof(double));  // malloc in C
  (void)d;
  free(d);      //free in C
}

--------------------------------------------------------------------------------
int main()
{
  double* d = new double;     //in C++
  (void)d;
  delete(d); //new in C++
  
  double* a = new double[100];  //allocate memory
  (void)a;
  delete[] a;   //delete [] a instead of delete *a, 99% delete, 1% leak if *a 
  
  //called HEAP in C -> called free store in C++ 
}

*************************Class and Objects***************************************
class Shape 
{
 private:
    int area;
 public:
    int get_area();
} triangle; 


class is private by default 

- Private members are accessible by members of same class or friends
- Protected members are accessible by members of same class and friends, but also from members of derived classes
- Public are accessible anywhere where the object is visible

Constructor:
------------
- same name as class
- no return type, not even void 
- must be public (can be private)
- Automatically called when new class object is created 
- Used to initialize data members or assigns dynamic memory 

Default Contructor:
-------------------
- created automatically if there is no any overloaded contructor 
- same name but different types or number of parameters

- As soon as i declare own contructor for a class, compiler no longer provides implicit default contructor 

class CRectangle 
{
    int x, y;
    
    public:
        CRectangle(int _x, int _y)           // Overloaded constructor
        {
            x = _x;
            y = _y;
        }
};
void main() 
{
 CRectangle rect(3, 6); //Using overloaded ctor
 CRectangle rect2; //Error: there is no default ctor
}

Copy Contructor: 
----------------
- Copies all data in another object 

e.g. CRectangle rect(3, 6);
    CRectangle rect2(rect);

- created automatically(synthesized) if not specified
- Synthesized copy constructor makes "shallow" copy 

- Overloaded copy constructor

CRectangle(const CRectangle& rect)
{
 x = rect.x;
 y = rect.y;
}

Copy assignment operator:
-------------------------
- same as copy constructor, makes shallow copy 
- created automatically (synthesized) if not specified. 

CRectangle& operator=(const CRectangle& rect)
{
 x = rect.x;
 y = rect.y;
};

CRectangle rect(3, 6);
CRectangle rect2 = rect;

Destructor:
-----------
- Opposite function to constructor
- Created automatically (synthesized) if not specified 
- Automatically called when object is destroyed
    - function ends 
    - object dynamically assigned and released using operator delete 
- Same name as class but with a tilde sign(~)
- No return value

class CRectangle 
{
    int x, y;
    char* tag;
    
public:
    CRectangle() 
    {
        x = 0;
        y = 0;
        tag = (char*)malloc(10); // allocate memory
    }
    
    ~CRectangle() 
    {
        free(tag); // free memory
    }
};

void main() 
{
 CRectangle rect; // Constructor here
} // Destructor here is called automatically

Pointer to Object and self reference
------------------------------------
- Class name can be used as the type for a pointer to an object:

- Use -> to get access to public member of the object via pointer 
- Use this to point to object whose member function is being executed (self reference)

class CRectangle 
{
    int x, y;

public:
 CRectangle(int x, int y)               // Overloaded constructor
 {
    this->x = x;
    this->y = y;
 }
};

CRectangle *rect;
rect->set_values(5, 6);
this-> here removes umbiguity

**************************Member Initilization List***********************************************

- alternative method of initializing members in constructor

class Foo 
{
 int x, y, z;

 public:
 Foo(int X=0, int Y=0, int Z=0):x(X), y(Y), z(Z){}          // class member(parameter)
}

or 

Foo(int x=0, int y=0, int z=0): x(x), y(y), z(z){}

- Suppose a class has a const or reference data member 

class Foo 
{
 int x;
 const int y;
 int& z;

public:
 Foo(int x = 0, int y = 0, int z = 0);
}

If constructor is like this:

Foo::Foo(int x, int y, int z)               //error message
{
 this->x = x;
 this->y = y;
 this->z = z;
}

- Problem is that both const variable and reference must be initialized
    - Contructors as written performs assignment, not initialization 
    
- Using initialization list solves this problem: 

Foo(int x, int y, int z):x(x), y(y), z(z){}

Remember:
---------

- if class has const or reference data members, 
    - initialize them with initialization list. 
    - compiler will not generate constructor and default assignment operator
    
Order of Initialization:
------------------------
- Always initialized in the order they are declared in the class
- Reordering the initialization list does not matter. 

class D 
{
    int a;
    int b;

public:
    D() : b(3), a(b) 
    {
        cout << a << “ “ << b;  
    }
} d;

Output: 0 3

Initialization list & Default Member Initilizer
-----------------------------------------------
- if non static data member has a default member initializer and also appearsin a member initialization list, 
    then member initializater list is executed and the default member initializer is ignored
    
class S 
{
    int n = 42;                  // C++11 default member initializer

public:
    S() : n(7) {} // will set n to 7, not 42
};


*********************************Member Functions************************************************
const member Functions:
-----------------------
- Can declare member functions as const 
- Const member function can be called on both const and non-const objects
- Cannot call non-const member functions on const object
- const rule does not apply to constructors or destructors
    - cant be modified between creation and destruction
    
Assessors & Mutators:
---------------------
- Assessors ("get" methods)
    - Member functions to read values 
    - return by value or by const reference 
    - Functions often const 
    
- Mutators ("set" functions)
    - Member functions to "write" values;
    - Return type is often void
    - Parameter type 
        - Same as member variable type
        - const reference to member variable type 
    
class Date 
{
public:
 // Accessors
 int get_d() const;
 int get_m() const;
 int get_y() const;
 // Mutators
 void set_d(int);
 void set_m(int);
 void set_y(int);
 
private:
 int d, m, y;
};

Mutable Date Member:
--------------------
- if data member is declared mutatble
    - legal to assign value to this data from a const member function 

class X 
{
public:
 bool GetFlag() const 
 {
    accessCount++; // Legal!
    return flag;
 }

private:
 bool flag;
 mutable int accessCount;
};

Static Members:
---------------
- One copy of static member is shared by all objects of a class in a program 
- Static member not part of class object
- Their content is not different from one object of this class to another 
- Static members do not have access to this pointer 
- Static integral constants may be initialized within a class declaration by a constant expression 
- Static members have the same properties as global variables but need to indicate class scope 

// static members in classes
class CDummy 
{
public:
 static int n;
 static const int MAX = 100;

 CDummy () { n++; };
 ~CDummy () { n--; };
};

int CDummy::n=0;

int main () 
{
 CDummy a;
 CDummy b[5];
 CDummy* c = new CDummy;

 cout << a.n << endl;
 delete c;

 cout << CDummy::n << endl;
 return 0;
}

Output: 7 6

Friends:
--------
- Give access to specific classes or functions 

class CRectangle 
{
 int x, y;

 friend ostream &operator<<(ostream &out,const CRectangle &rect);
};


Properties of friend relations:
-------------------------------
- friendship gives complete access
- friend methods/functions behave like class members
- public, protected, private scopes all accessible by friend


***********************Operator Overloading********************************************************
int add(int a, int b);
float add(float a, float b);
double add(double a, double b);
double add(int a, double b);
...
double d = add(3, 4.1);

Examples without overloading:
-----------------------------
class StopWatch
{
public:
 // Constructors
 StopWatch(void);
 StopWatch(int seconds);
 StopWatch(int hours, int minutes, int seconds);
 void Increment(int seconds = 1);
 void Reset(void);
 int GetSeconds(void) const;
 void Display(void) const;
private:
 int seconds;
};

// Constructors
StopWatch::StopWatch(void){
 seconds = 0;
}
StopWatch::StopWatch(int seconds) {
 this->seconds = seconds;
}
StopWatch::StopWatch(int hour, int minutes, int seconds) {
 this->seconds = (hour*3600) + (minutes*60) + seconds;
}
void StopWatch::Increment(int seconds){
 this->seconds += seconds;
}
void StopWatch::Reset(void) {
 this->seconds = 0;
}
int StopWatch::GetSeconds() const {
 return seconds;
}

void StopWatch::Display() const
{
 int h, m, s;
 h = seconds / 3600;
 m = seconds % 3600 / 60;
 s = seconds % 60;
 std::cout.fill('0');
 std::cout << std::setw(2) << h << ':';
 std::cout << std::setw(2) << m << ':';
 std::cout << std::setw(2) << s << std::endl;
}

StopWatch sw1;
StopWatch sw2(625);
StopWatch sw3(9, 30, 0);
sw1.Display(); // 00:00:00
sw2.Display(); // 00:10:25
sw3.Display(); // 09:30:00
sw1.Increment(); // add 1 sec
sw1.Increment(); // add 1 sec
sw1.Increment(5); // add 5 secs
sw1.Display(); // 00:00:07

- Want to do something like this, but abov code does not compile as compile does not know
  how to add two stopwatch objects

StopWatch sw1(10), sw2(20);
StopWatch sw3 = sw1 + sw2;

Traditional Method: 
-------------------
StopWatch AddStopWatch(const StopWatch& sw1,const StopWatch& sw2)
{
 // Construct a new SW from two
 StopWatch sw(sw1.GetSeconds()+sw2.GetSeconds());
 // Return the result
 return sw;
}

StopWatch sw3 = AddStopWatch(sw1, sw2);

- However, does not scale well if add multiple stopwatches object together. 

StopWatch sw1(10), sw2(20), sw3(30), sw4(40), sw5(50);
StopWatch tempwatch = AddStopWatch(sw1, sw2);
tempwatch = AddStopWatch(tempwatch, sw3);
tempwatch = AddStopWatch(tempwatch, sw4);
StopWatch sw6 = AddStopWatch(tempwatch2, sw5);

or 

StopWatch sw6 = AddStopWatch(AddStopWatch(AddStopWatch(AddStopWatch(sw1, sw2), sw3), sw4), sw5);

What we want to do:
-------------------
- Teach compiler to add stopwatch objects by overloading the + operator 
- Done using an operator function 

StopWatch sw6 = sw1 + sw2 + sw3 + sw4 + sw5;

Syntax:
 return-type operatorop(argument-list)
 {
    statements
 }
 
 - operator keyword
 - op is the operator to overload 
 - argument-list is the list of argument to the operator 
 - argument list constains at most 2 arguments
    - the conditional operator ?: cannot be overloaded 
    
StopWatch operator+(const StopWatch& lhs,const StopWatch& rhs)
{
 StopWatch sw(lhs.GetSeconds()+rhs.GetSeconds());            // lhs is left hand side, rhs is right hand side
 return sw;                                                   // Return the result
}

- same as previous, AddStopWatch function. Only name has changed. 

sw1 + sw2 is equal to calling operator+(sw1, sw2) function 

StopWatch sw1(10), sw2(20), sw3(30), sw4(40), sw5(50);         // 150 seconds

StopWatch sw6 = sw1 + sw2 + sw3 + sw4 + sw5;

sw6.Display();                                                  // 00:02:30
StopWatch sw7 = operator+(sw1, sw2);                            // functional notation

sw7.Display();                                                  // 00:00:30


Example 2:
----------
StopWatch operator-(const StopWatch& lhs, const StopWatch& rhs)
{
 // check for negative seconds
 int seconds = lhs.GetSeconds()-rhs.GetSeconds();
 if (seconds < 0)
    seconds = 0;

 return StopWatch(seconds);                                     //return the result
}

- Now we can do:

StopWatch sw3 = sw1 - sw2;

Example 3:
----------
StopWatch operator*(const StopWatch& lhs, int rhs)
{
 StopWatch sw(lhs.GetSeconds() * rhs);                        // create the new StopWatch
 return sw;                                                  // return the result
}

- Now we can do:

StopWatch sw2 = sw1 * 2;

Example 4:
----------
StopWatch sw2 = 2 * sw1;                    // cannot do this 

-Need to add this function: 

StopWatch operator*(int lhs, const StopWatch& rhs)
{
 StopWatch sw(lhs * rhs.GetSeconds());
 return sw;
}

or better, do this 

StopWatch operator*(int lhs, const StopWatch& rhs)
{
 return rhs * lhs;
}

**********************************Operator Overloading As Methods*********************************
- Previously, all functions are in global 
- made use of the GetSeconds() public method 
- If GetSeconds() method did not exist, non of the function works 

- One solution is to make these function member functions of the stopwatch class 

class StopWatch
{
public:
 // Public methods...
 // Operator overloads
 StopWatch operator+(const StopWatch& rhs) const;
 StopWatch operator-(const StopWatch& rhs) const;
 StopWatch operator*(int rhs) const;

private:
 int seconds;
};

StopWatch StopWatch::operator+(
const StopWatch& rhs) const
{
 // Add seconds to this object's seconds
 StopWatch sw(seconds + rhs.seconds);
 return sw;
}
StopWatch StopWatch::operator-(
const StopWatch& rhs) const
{
 // Add seconds to this object's seconds
 StopWatch sw(seconds - rhs.seconds);
 return sw;
}

- first argument(that corresponds to the lhs operand) is omitted 
    - the calling object is assumed to be the left operand 
    
- Most of the previous statements works as before
- But this no longer works:
    
StopWatch sw2 = 2*sw1;

- left operand is int, not stopwatch 
- So we still need a non member operator for this case 

Overloading for Cout 
--------------------
- Instead of Display(), do:

StopWatch sw1;
std::cout << sw1;

- Achieve by overloading the << operator in a non member function 

std::ostream& operator<<(std::ostream& os, const StopWatch& sw)
{
 ...
 os.fill('0');
 os << std::setw(2) << h << ':';
 os << std::setw(2) << m << ':';
 os << std::setw(2) << s << std::endl;
 // return the reference to ostream
 return os;
}

doing this: 

std::cout << sw1;

is same as 

operator<<(std::cout, sw1);

because we return a reference to an ostream object, can even do this:

std::cout << sw1 << sw2 << sw3;

- declare friend to access private data 

class StopWatch 
{
public:
 ...
 // friend function
 friend std::ostream& operator<<(std::ostream& os, const StopWatch& sw);

private:
 int seconds;
};

Automatic Conversions:
----------------------
StopWatch sw2 = sw1 + 60;                  // can we do this?

- C++ complier performs certain automatic conversions:

3 + 4.1 is 7.1 (double)
Same as (double) 3 + 4.1

- From above example, complier does this:

StopWatch sw2 = sw1 + (StopWatch) 60;
StopWatch sw2 = sw1 + StopWatch(60);
 
- Any constructor that takes one argument is called conversion constructor 
    Implicitly called by the compiler
e.g. 
StopWatch sw1;  // default constructor
sw1 = 60;       // sw1 = (StopWatch) 60;
                // sw1 = StopWatch(60);
 
Calling the function:
void fooSW (const StopWatch & sw)
{
 sw.Display();
}

StopWatch sw(60);
fooSW(sw);
fooSW(60);

- If dont want a constructor to perform automatic conversion, use explicit keyword:

class StopWatch
{
public:
 // explicit constructor
 explicit StopWatch(int seconds);

private:
 int seconds;
};

e.g.
void fooSW(const StopWatch & sw)
{
 sw.Display();
}

- Calling the function:

StopWatch sw(60);
fooSW(sw); // OK
fooSW(60); // Error
fooSW(StopWatch(60)); // OK

- However, cannot do this:

StopWatch sw;
int sec = sw; // Error, 
int sec2 = (int) sw; // Error

- conversion constructor can convert an int to a Stopwatch, but not stopwatch to int 

- To convert a stopwatch object to an int, we can write a function 

class StopWatch 
{
public:
 // Public methods
 ...
 // conversion to int
 int ToInt(void) const;

private:
 int seconds;
};

int StopWatch::ToInt(void) const {
 return seconds;
}

- To use this, call it explicitly:

StopWatch sw(60);
int seconds = sw.ToInt();
std::cout << sw.ToInt();

- If want to give ability for implicit conversion, define a member function using operator keyword 

class StopWatch 
{
public:
 // Public methods
 ...
 // implicit conversion to int
 operator int(void) const;

private:
 int seconds;
};

StopWatch::operator int(void) const
{
 return seconds;
}

- Now implicit conversion works:

Stopwatch sw(60);
int seconds = sw;
std::cout << sw;

Notes:
- General form: operator type()
- It must be a member function
- No parameter (you may have void as parameter)
- No return type (can't even return void)


- Becareful when using implicit conversions:
    - these conversions are done easily 
    
Example: 

int array[10];
StopWatch temp1(60);
int temp2 = 0;
array[temp1] = 10; // Uh-oh...

Side - effects operators:
-------------------------
- side effects operators modify left operand, suppose we want to do this:

 StopWatch sw1(60), sw2(30);
 sw1 += sw2; // Now sw1 == 90
 
- Comparing with operator+:

class StopWatch 
{
public:
 // Public methods...
 // overload for sw1 + sw2
 StopWatch operator+(const StopWatch& rhs) const;
 // overload for sw1 += sw2
 StopWatch& operator+=(const StopWatch& rhs);

private:
 int seconds;
};

- Comparing with operator+:

StopWatch StopWatch::operator+(const StopWatch& rhs) const 
{
 // create a new object from both operands
 StopWatch sw(seconds + rhs.seconds);
 return sw;
}

StopWatch& StopWatch::operator+=(const StopWatch& rhs)
{
 // modify this object directly
 seconds += rhs.seconds;
 return *this;
}

Notes:
○ The method is not marked const because it
changes the calling object
○ It returns a reference because we are not creating
a new object
○ Since this is a pointer to the calling object, *this is
the object itself
○ Returning the object allows the following:

sw1 += sw2 += sw3;

How do you overload the ++ operator?
● Prefix:

class StopWatch 
{
public:
 // Public methods...
 // overload for prefix ++
 StopWatch& operator++(void);
private:
 int seconds;
};

StopWatch& StopWatch::operator++(void)
{
 seconds++;
 return *this;
}

- The postfix version of ++ has an int parameter:

class StopWatch
{
public:
 // Public methods...
 // overload for postfix ++
 StopWatch operator++(int);
private:
 int seconds;
};

StopWatch StopWatch::operator++(int)
{
 StopWatch sw(seconds);
 seconds++;
 return sw;
}

- The int parameter for postfix++ (and postfix--) is not used; it is just to indicate postfix rather than prefix
    ● Because the postfix version requires the creation of a temporary object, in general prefix increment is more efficient than postfix

● Bonus question:
    ○ With these examples, sw1 = (++sw2)++; is legal, and so is (sw1 + sw2) = 30; How do you prevent this?

Issue with operator overloading:
--------------------------------
With operator overloading, in general there are 3 options. E.g., when overloading a binary operator:
○ Member function (one operand, implicit this)
○ Non-member friend function (two operands)
    ■ friend has access to private data
○ Non-member, non-friend function
    ■ data access via public methods
    
- At least one operand must be a user-defined type (you can't overload built-in types)
 // Illegal – overloading int addition
 int operator+(int lhs, int rhs);

● You cannot violate C++ rules for the operator you overload
○ Number of operands
○ Precedence
○ Associativity
○ E.g., you cannot overload modulo (%) to take one operand

- You cannot create new operator symbols. E.g., operator@ is not allowed because @ is not a C++ operator
● The following operators can only be overloaded as member functions (others can be overloaded as non-member functions)

= Assignment operator
() Function call operator
[] Subscripting operator
-> Class member access by pointer operator

- You cannot overload the following operators:
:: Scope resolution operator
.* Pointer-to-member operator
. Membership operator
?: Conditional operator
sizeof sizeof operator
typeid RTTI operator
const_cast A typecast operator
dynamic_cast A typecast operator
reinterpret_cast A typecast operator
static_cast A typecast operator

*************************************Overload Resolution*****************************************
Function Overloading:
---------------------
● In C++ you can have two or more functions with the same name so long as they differ in their parameter lists. This is called function overloading.
● The function is invoked whose parameter list matches the arguments in the call.
● Normally the compiler can deal with overloaded functions fairly easily by comparing the arguments with the parameter lists of the candidate functions.
● However, this is not always a straightforward matter.

- Consider the following code:

void foo(double d1, int i1) { }
void foo(double d1, double d2) { }
int main( ) {
foo(1.0, 2);
return 0;
}

● How does the compiler know which version of foo()
to call?

The compiler works through the following checklist and if it still can't reach a decision, it issues an error:
1. Gather all the functions in the current scope that have the same name as the function called.
2. Exclude those that don't have the right number of parameters to match the arguments in the call.
a. It has to be careful about parameters with default values;

void foo(int x, int y = 0) is a candidate for the call foo(25);
3. If no function matches, the compiler reports an error.
4. If there is more than one match, select the 'best match'.
5. If there is no clear winner of the best matches, the compiler reports an error - ambiguous function call.

Best Matching:
--------------
In deciding on the best match, the compiler works on a rating system for the way the types passed in the call and the competing parameter lists match up.
● In decreasing order of goodness of match:
1. An exact match
2. A promotion
3. A standard type conversion, e.g. int to short
4. A conversion constructor or user-defined type conversion

Exact match:
------------
An exact match is where the parameter and argument types match exactly.
● Example: argument is a double and parameter is a
double.
● Note that, for the purposes of overload resolution a pointer to an array of type x exactly matches a pointer of type x.
○ This is because arrays are always passed by reference, meaning that you actually pass a pointer to the first element of the array.

Exact matches for array:
------------------------
void f(int y[ ]){} // call this f1
void f(int* z){} // call this f2
....
int x[ ] = {1, 2, 3, 4};
f(x); // Both f1 and f2 are exact
matches, so the call is ambiguous.
void sf(const char s[]){}
void sf(const char*){}
....
sf("abc"); // Same problem; both sf
functions are exact matches.

Type Promotion:
---------------
The following are described as "promotions":
○ A char, unsigned char or short can be promoted to an int.
    ■ For example, void foo(int); can be a match for foo('a');
○ A float can be promoted to a double.
○ A bool can be promoted to an int
    ■ false counts as 0, true as 1.

Standards Conversions:
----------------------
All the following are described as "standard conversions":
● conversions between integral types, apart from the ones counted as promotions. Remember that bool and char are integral types as well as int, short and long.
● conversions between floating types: double, float and long double, except for float to double which counts as a promotion.
● conversions between floating and integral types
● conversions of integral, floating and pointer types to bool (zero or NULL is false, anything else is true)
● conversion of an integer zero to the NULL pointer.

All of the standard conversions are treated as equivalent for scoring purposes.
○ A seemingly minor standard conversion, such as int to long, does not count as any "better" than a more drastic one such as double to bool.

Conversions Constructor:
------------------------
Constructors that provides an implicit type conversions.
● This kind of conversion can only work when the constructor can be called with just one argument.
● Generally this means that the constructor will have just one parameter, but it could have more if all but the first of the parameters (or, indeed, all of them) had default values.

Explicit:
---------
It can happen that you have a constructor that can be called with just one argument, and which will therefore behave as a conversion constructor, but you don't want it to behave in this way.
● But implicit type-conversions are a common source of programming error, so you might decide to disallow that sort of conversion.
● We can do that simply by inserting the keyword explicit before the constructor prototype in the class definition

User-defined Conversions:
-------------------------
- Conversion member-functions allow you to specify how you want objects to respond if they are asked to behave as if they were objects of some other type.
● In following example, a Foo is being treated as if it were an int:

class Foo {
public:
 operator int() { return 0; }
}

void boo(int n) { }
Foo foo;
boo(foo);

Choosing a winner:
------------------
- A candidate function is only as strong as its weakest match;
    ○ a candidate requiring three promotions, for example, beats a candidate with two exact matches and a standard conversion.
- Candidates whose weakest matches are equivalently weak are compared on their next-weakest, and so on - a candidate with a standard conversion, a promotion and an exact
  match beats a candidate with a standard conversion and two promotions.
  
**************************************************************************************************
Function Template:
----------------------------------------
Overloading of the cube function:

int cube(int n) {
return n * n * n;
}
long cube(long n) {
return n * n * n;
}
float cube(float n) {
return n * n * n;
}
double cube(double n) {
return n * n * n;
}

- Convenient for users since they can use the cube function for 4 different data types
- Inconvenient for programmer to maintain 4 or more different function of same function

- We want to write one function and apply it to all possible different types 

- function template (Ada, 1983) is a type generic way of describing a function 
- Reffered to as generic programming 
- Templates are also called parameterized types becausing the type is passed as a parameter to a function 

New Cube function:
------------------
template <typename T>
T cube(T v)
{
    return v * v * v;
}

- The keyword template indicates that the function is a template function
- We put the "type parameter" name in angle brackets < > with the typename keyword

- The typename keyword can be replaced with the class keyword (but this is less clear)
- The rest of the function is the same, except that we replace the type (int, float, etc.) with the type parameter
- The type parameter must be a legal identifier
    ○ T is often used
    
Alternative indentations:
-------------------------

template <typename T> T cube(T v)
{
    return v * v * v;
}

template <typename T>
T cube(T v)
{
    return v * v * v;
}

- Template declarations do not generate any code
    ○ Similar to class and struct declarations
    ○ Should usually belong in header files
- Code is only generated when the function is used (ex: called in main function)
- This automatic code generation is called template instantiation

- If we have this statement in our program:

    int i = cube(2);

- Then code similar to this is generated:

int cube (int v)
{
    return v * v * v;
}

- A template is a way to describe to the compiler how to generate functions
- The compiler generates these functions based on the template
    ○ The generated program is no smaller than writing these functions yourself
    ○ This generation is done implicitly, so it is called implicit instantiation
        ■ You cannot see the result of instantiation!      

Automatic Type Deduction:
-------------------------
- The compiler can usually deduce the type of the function arguments (i.e., the type of T)
- However, we can "force" a particular instantiation using
    ○ Explicit function call, or
    ○ Typecasting the argument

We modify our cube function to show the type information:

#include <typeinfo> // for use with typeid

template<typename T> T cube(T v)
{
    std::cout << "<" << typeid(v).name()<< ">" << std::endl;
    return v * v * v;
}

- Learn more about typeid in CS225!

// Compiler deduction Microsoft/Borland GNU
cube(2);                                    // <int> <i>
cube(2.0f);                                 // <float> <f>
cube(2.0);                                  // <double> <d>
cube('A');                                  // <char> <c>
// Explicit call
cube<double>(2);                            // <double> <d>
cube<int>(2.1);                             // <int> <i> (warning)
// Explicit typecasting
cube((double) 2);                           // <double> <d>
cube((int) 2.1);                            // <int> <i> (no warning)

Class Object In Template Function:
----------------------------------
● User-defined class objects can also be used as type parameters
● Does the following compile?

StopWatch sw1(4);
StopWatch sw2;
sw2 = cube(sw1);
std::cout << sw2 << std::endl;

This is what the compiler generates:
StopWatch cube(StopWatch v)
{
    return v * v * v;
}

- This will not compile if there is no overloaded operator* for StopWatch
- We need to define this:

StopWatch StopWatch::operator*(const StopWatch& rhs)
{
    return StopWatch(seconds * rhs.seconds);
}

Now this will compile:
StopWatch sw1(4);                           // Create a StopWatch set to 4 seconds
StopWatch sw2;                              // Create a StopWatch set to 0 seconds
sw2 = cube(sw1);                            // cube sw1, assign it to sw2
std::cout << sw2 << std::endl;              // 00:01:04 (4*4*4 = 64 sec)

Let's try to create a generic max function:

template<typename T>
T Max(T a, T b)
{
    return a > b? a : b;
}

- Using the function:

int i = Max(2, 5);                          // i = 5
double d = Max(2.2, 5.5);                   // d = 5.5
double e = Max(2.2, 5);

- To mix types, we need to specify both parameters:


Multiple Template Parameter:
----------------------------

template<typename T1, typename T2>
T1 Max(T1 a, T2 b)
{
    return a > b ? a : b;
}

- Using the function:
double d = Max(2.2, 5); // d = 5.0
double e = Max(2, 5.5);

- We have to add a third type:

template<typename T1, typename T2, typename T3>
T1 Max(T2 a, T3 b)
{
    return a > b? a : b;
}

- How do we use this function?
    
    double d = Max(2.2, 5); // Error
    d = Max(2, 5.5); // Error

- The compiler cannot deduce the return type
    ○ User must specify it

- Ways of using the function:

d = Max<double, int, double>(2, 5.5);
d = Max<double, double, int>(2.2, 5);
d = Max<double>(2, 5.5);

// Possible warning
d = Max<double, int, int>(2, 5.5);

// Possible warning
d = Max<int, int, double>(2, 5.5);

Explicit Template Specialization:
---------------------------------
Sometimes we may need to handle "special cases" of our template functions
● Example

template<typename T>
bool equal(T a, T b)
{
    std::cout << a << " and " << b << " are ";
    
    if (a == b)
        std::cout << "equal." << std::endl;
    else
        std::cout << "not equal." << std::endl;
    return a == b;
}

- Example usage:
int a = 5, b = 5, c = 8;
equal(a, b);                            // 5 and 5 are equal
equal(a, c);                            // 5 and 8 are not equal
equal(a + 3, c);                        // 8 and 8 are equal
equal(a, 5.0);                          // Error! Conflicting types for parameter ‘T’

- What about these?

const char s1[] = "One";
const char s2[] = "One";
const char s3[] = "Two";

equal(s1, s2);                          // One and One are not equal
equal(s1, s3);                           // One and Two are not equal
equal(s1, "One");                       // One and One are not equal
equal(s1, s1);                          // One and One are equal
equal("One", "One");                    // Undefined behaviour

This is what's actually happening:
bool equal(const char* a, const char* b)
{
    if (a == b)                             // Comparing pointers
    // Etc.
}

- We need a "specialized" version of our template function
- This is called explicit template specialization

- Specialization of equal for C-style strings:

template<>
bool equal<const char*>(const char* a,const char* b)
{
    std::cout << a << " and " << b << " are ";
    bool same = !strcmp(a, b);
    
    if (same)
        std::cout << "equal." << std::endl;
    else
        std::cout << "not equal." << std::endl;
    
    return same;
}

- The syntax is similar to normal templates, except that there are empty angle brackets <> after the template keyword
- The specialization must be after the template declaration
    ○ However, you can use a prototype for your template function
    
Now this works as expected:
const char s1[] = "One";
const char s2[] = "One";
const char s3[] = "Two";

equal(s1, s2);                          // One and One are equal
equal(s1, s3);                          // One and Two are not equal
equal(s1, "One");                       // One and One are equal
equal(s1, s1);                          // One and One are equal
equal("One", "One");                    // One and One are equal

- Actually, the second angle brackets after the function name is optional. So this:

template<>
bool equal <const char*>
(const char* a, const char* b)

- can be changed to this:

template<>
bool equal(const char* a, const char* b)

The compiler's order of preference when choosing functions (from best to worst):

    ○ Regular functions
    ○ Explicit specializations
    ○ Template generated

- However, you can always force the compiler to use a function by stating it explicitly

Example:
// Template function

template<typename T> T cube(T value)
{
    return value * value * value;
}

// Explicit specialization cube<int>
template<> int cube<int>(int value)
{
    return value * value * value;
}

// Regular function
int cube(int value)
{
    return value * value * value;
}

Using the functions:
cube(5);
cube(5.5);
cube<int>(5);
cube<double>(5.5);
cube('A');

Note that you cannot create an explicit specialization after an implicit instantiation is created:

void foo(void)
{
    cube(5);                            // Implicit instantiation
}

template <>
int cube<int>(int v)                    // Explicit specialization cube<int> - error!
{
    return v * v * v;
}

Overloaded Template Functions:
------------------------------

● Suppose we create a template swap function:

template<typename T>
void swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

- Example usage:

int i = 10, j = 20;
swap(i, j);                             // i = 20, j = 10

- What if we try to swap arrays?

int a1[] = {1, 3, 5, 7, 9};
int a2[] = {2, 4, 6, 8, 10};
swap(a1, a2);

- This is what happens:

void swap(int a[5], int b[5])
{
    int temp[5] = a;
    a = b;
    b = temp;
}

- Error assignment of arrays

We can overload the function to deal with arrays:

template <typename T>
void swap(T* a, T* b, int size)
{
    T temp;

    for (int i = 0; i < size; i++)
    {
        temp[i] = a[i];
        a[i] = b[i];
        b[i] = temp[i];
    }
}

Using the function:

int a1[] = {1, 3, 5, 7, 9};
int a2[] = {2, 4, 6, 8, 10};

int size = sizeof(a1) / sizeof(*a);
swap(a1, a2, size);

Explicit Instantiation:
----------------------- 

● So far we have been using template functions via implicit instantiations
○ The compiler generates the functions when it encounters the function call
○ This is the point of templates
   ● However, it is also possible to have explicit instantiations
○ Generating the function definition without a function call

Example:
template <typename T>
T cube(T v)
{
    return v * v * v;
}

// Explicit instantiation of cube<int>
template int cube(int v);
● The angle brackets are omitted
● This is mainly useful for creating library files for distribution because uninstantiated template functions are not compiled into object files

Summary: 
--------

Function templates are used to create generic functions that can take any data type
○ They tell the compiler how to generate a function
○ Code is only generated when called
● Template functions can be called implicitly using automatic type deduction (implicit instantiation)
● They can also be called explicitly by specifying the types in angle brackets (explicit call)

● Class objects can be used in template functions like any other data type
● Multiple parameters can be used
● If a function acts differently for a data type, it can be defined using an explicit specialization
● A template function can also be overloaded to accept different numbers/types of parameters
● Use explicit instantiation to produce a function from a template without a function call


**************************************************************************************************