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

//to compile: g++ -Wall -Wextra -Werror -Wconversion -pedantic -std=c++11 -o 
//main.exe main.cpp


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
  mySwap(&a,&b);

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
  std::cout << errorReporting << std::endl     //output:10
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
  std::cout << v << std::endl     //output:30
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
  std::cout << ::v << std::endl     //output:30
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
  std::cout << errorReporting::File::v << std::endl     //output:40
  (void)v;
}

--------------------------------------------------------------------------------
#include <iostream>
#include "my.h"

namespace errorReporting
{
  int v = 20;
  namespace File
  {
    int v = 40;
  }
  
  void(f)
  {
    std::cout << ::errorReporting:File::v;
  }
}

namespace   //anonymous namespace
{
  int v = 100;
}

int main()
{
  int v = 30;
  std::cout << errorReporting::File::v << std::endl     //output:
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
  std::cout << "Hello World!" << std::endl
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

--------------------------------------------------------------------------------
