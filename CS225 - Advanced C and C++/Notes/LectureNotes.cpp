Week 1 Lecture 1:
=================


- CPU understands machine code only (binary) [op-codes]
- Machine codes are a sequence of bytes
- unstructured programming

Assembly Language:
------------------
ADD AX,BX
JMP 256

- High-level programs use *abstractions* to hide executions details
- Structured programming
a;
if()
{
   b; 
}
else
{
   c;
}
d;

- Functions as a sequence of instructions 
- Sub-routine procedure function
- Procedural programming paradigm 

main:
if()
{
    
}

do
{
    
}

-Text
-ROData
-Data

-Stack will be at the end of the memory 
-We want to isolate responsibilities for managing data 

OOP:
====


Generic Programming:
====================
- Using templates

V<T>


Week 1 Lecture 2:
=================
Topics:
1.Relationships between objects[class]
2.Layout of objects in memory
3.Effect of Inheritance

1.Relationship between objects[class]
-------------------------------------
class Arm;
class Terrain;
class Robot
{
  Arm LArm;
  Arm RArm;
  
  void find(Terrain & t);
};

class BipodRobot:Robot
{
    
};

what is the relationship of the robot with the terrain and arm?

1. Dependency
- A class (Robot) depends on another class (Terrain); this other class does 
impact the size of the original class. 

Class Robot                           -->  Class Terrain      
-LArm:Arm
-RArm:Arm
-find(t:Terrain)returns void

2.Association
- A class Student is associated with Teacher; Student may have a pointer(or null)
  to a Teacher.
  
Class Student                                --> class Teacher
- has variable myTeacher of type Teacher*

3.Aggregation

- 1 Robot -> 2 Arms
- Arm is part of Robot, Arm can exist independently. 

4. Composition
 
- 1 Robot -> 2 Arms
- Arm is part of Robot, but once Robot is destroyed, Arm is destroyed as well. 

5. Inheritance

            Robot
    -------------------------
    |       |               |
    v       v               v
Drone   BipodRobot  SwimmingRobot

- From bottom to up is called Generalisation 
- From top to bottowm is called Specialisation 
- Going down but from interface classess called Realization

2. Layout:
----------
Primitive Objects:

char - 1 bytes 
double 8 bytes (IEEE754)
float  4 bytes 
short  at least 16+ bits
int    4 bytes or as big as short which is at least 16+ bits
long   at least 32+ bits
long long at least 64+ bits

1 byte is not always in bits
1 byte is the smallest thing can address in memory 

1 byte 8 bits

sizeof(char - 64bits) <= sizeof(short-64bits) <= sizeof(int - 64bits) <= sizeof(long-64bits) <= sizeof(long long - 64bits)

- Unsigned Uint32_t (alias)
may not exist but if exist is 32 bits

research on window linux 

Pointers, arrays, references
----------------------------
Pointers:
- void * is 64 bits in 64bit systems or 32bits in 32bit systems
- int *
- any pointers 

Arrays:
-static arrays(may decay): int x[10]; //size 40

void f(int x [10])
{
    std::cout << sizeof(x); //40
}

void f(int *x)      //points to the start of the array, decays
{
    std::cout << sizeof(x) // 8 
}


int* x = new int[10]    //8 bytes, 64 bits because its a pointer

References: 
- Can be 0 bytes; Can be eliminated by changing i instead of introducing new variable r
int i = 10;
int &r = i;
r = 20;
std::cout << r;


int i = 10;
i = 20;

std::cout << i;

- Or it can be a pointer 

int a = 10;
int b = 20;
f(&a);
f(&b);

void f(int *r)  //8bytes which is 64 bits in 64bit system because its a pointer 
{
    *r = 30;
}

char c;
char &r = c;
std::cout <<sizeof(c);          //1 , gives the size of the reference object 

class X
{
    int &r;
}

std::cout << sizeof(x); //8 because r is a pointer 
std::cout << sizeof(x.r); //4, because r is int??

3.Object (without inheritance)
------------------------------
- Class is a composition of its data members + paddings between data members + padding at the end

class C         //sizeof(C) = 2 bytes;
{
    char c;     //1byte
    bool b;     //1byte
}

class C       //26 bytes 
{
    double d; //8 bytes 
    int i;    //4 bytes
    char c;   //1 bytes 
    double d2;//8 bytes 
    short s;  //2 bytes 
}

2 types of padding:

- between data members 
- after each array 

Why padding is neccessary?
--------------------------
class C       //23 bytes 
{
    double d; //8 bytes 
    int i;    //4 bytes
    char c;   //1 bytes 
    double d2;//8 bytes 
    short s;  //2 bytes 
}

number 8 and 4 is size of CPU registers 

- Dont have to care about taking 1/2 of 8 bytes and another 1/2 of 8 bytes then merge both 
- Padding can just take from 8-16 bytes instead of 13-21 bytes.
- An empty class data member adds 1 byte of bits padding to the class size 

Class E  //size of empty class?  1 bytes
{
    //1 byte of padding 
};

Class C // 1 byte in c++17, use this 
{
    E e; // 1 byte 
}

Class C // 1 byte in c++20 with 1 padding 
{
    E e // 0 byte, dont bring in the padding into new class C 
}

Object with inheristance:
-------------------------
- Base class data members goes at the top 
- Empty box optimisation
    - Inheriting from an empty class brings 0 bytes
    - Derived class can reuse void pointer of the first base class 

class B 
{
    char c;
};

class D:B
{
  int i;
};

- Base class data member goes first 

in memory: 

Class D: 8 bytes
char c - 1 byte 
padding - 3 bytes 
int - 4 bytes 

******************************************************************************

Class B
{
    
};

class D:B
{
    int i;
};

in memory:

class D: 8 bytes 

1 byte padding in class B;
padding - 3 bytes  
int i - 4 bytes 

but 

in memory:
class D: 4 bytes 

it i - 4 bytes

Multiple Inheritance:
---------------------

class B1        //total 1 bytes 
{
    char c;     //1 bytes
};

class B2        //total 4 bytes
{
    int i;      //4 bytes 
};

class D:B1,B2
{
    char c2;
;

in memory class D: total 9 bytes 

class B1 char - 1 bytes 
padding 3 bytes
class B2 int - 4 bytes 
class D char c2 - 1 bytes 

if add virtual member to a class 

class B1        //total 1 bytes 
{
    char c;     //1 bytes
    virtual void f();
};

class B2        //total 4 bytes
{
    int i;      //4 bytes 
    virtual void f2();
};

class D:B1,B2
{
    char c2;
;

in memory: B1 - 16 bytes 

class B1 void ptr - 8 bytes 
class B1 char char c- 1 bytes 
padding - 7 bytes 

in memory: B2 - 16 bytes

class B2 void pointer - 8 bytes 
class B2 int i - 4 bytes 
padding - 4 bytes 


in memory: class D - total 33 bytes 

class B1 void ptr - 8 bytes 
class B1 char c - 1 bytes 
padding - 7 bytes 
class B2 void pointer - 8 bytes 
class B2 int i - 4 bytes 
padding - 4 bytes  
class D char c2 - 1 bytes


Effects of Inheritance
----------------------
- Inheritance of implementation and interface 
    - Inheritance from non-interface class 
    - Public inheritance

- Inheritance of interface but not-implementation
    - Public inheritance
    - From an interface class 

- Inheritance of implementation but not interface
    - Non public inheritance
    - from non-interface class 
    
Week 2 Lecture 1:
================================================================================
Advanced Inheritance
--------------------

1. Rationale
    - Generalisation 
        - Reuse of code (interface and implementation)
        - Interface is the part on the outside (messy or neat, superficial)
        - Implementation is the inside (detailed)
        
    - Specialisation (add more stuff) 
        - Extension, overriding  
        
    - Realization
        - Implement interface classes 
        
        
class Robot //sizeof robot is 8 bytes because its virtual pointer 
{
    public: 
        virtual int getX() = 0;         
        virtual int getY() = 0;
        virtual ~Robot() = default;
};

class Drone:public Robot    //size of Drone is 8 bytes becuz inherit 8 bytes from robot 
{
public:
    int getX() overide {return 10}; //override 
    int getY() override {return 20};    
    
}

void print(Robot& r)
{
    std::cout << r.getX() << "," << r.getY() << std::endl;
}

- Interface classes 
    - Everything is public, if private then have to be called from function  
    - No data members 
    - No static members 
    - All members functions are pure virtual (destructor can be =default) 
    - The layout contains nothing but virtual pointer? 
    

2. Multiple Inheritance

class A             //sizeof A: 4 bytes - int. (no virtual pointer, 1 int variable)
{
private:
    int _a;

public: 
    A(int):_a{a}{}
    int getA() const{return _a};
};

class B             //sizeof B : 4 bytes 
{
private:
    int _b;
    
public:
    int getB() const{return _b};
    int getB() const {return _b};
};

class C: public A, B    //inherit public A, private B 
{                       //sizeof C: 12 bytes 4 from A, 4 from B , 4 from C 
private:
    int _c;

public:
    C(int a, int b, int c): A{a}, B{b}, _c{c}{}
    int getC() const {return _c};
}

int main(void)
{
    C c{1,2,3};
    
    std::cout << c.getA() << std::endl;     // ok 
    std::cout << c.getC() << std::endl;     // ok 
    std::cout << c.getB() << std::endl;     // not okay , B class is inherited private 
}

- Restriction: a class can inherit from a certain base class only once 
e.g. class C: A, B , B  // A okay, B okay, B again is not okay 

- Order of construction: A , B , C | ~C ~B ~A 

The deadly diamond of death 
---------------------------

            A
            |
     ___________________
     |                  |
     B                  C
     ___________________
             |
             D
             
             
Layout: 

A: A
B: A 
C: A 
D: B:A 
   C:A          
   
- Lots of duplicated data in memory 


- Ambiguity:

D d; 
D.d = 10; //ok
d.c = 20; //ok
d.b = 30; //ok 
d.a = 40; //not okay , not sure to put in D:B.A or D:C.A 

B&b = d;  //do it like this 
b.a = 50;


- In languages other than C++ 
    - inheritance from non-interface class is only supported as single inheritance,
    but interitance from interface classes is supported as multiple inheritance 

-In C++     
class WalkingRobot: Robot, Agent, Transmitter, Obstacle // each of all these can be a class 

- Virtual Inheritance 
    - Layout 

            A
            |
<virtual> __________ <virtual>
     |                  |
     B                  C
     ___________________
             |
             D
             
B: virtual -> other virtual info -> find A 
C: virtual -> other virtual info -> find A 

D: virtual -> find A
   B 
   
   virtual -> find A 
   C
   
   A
   
- Virtual base classes are located at the end of an object. Their derived classes 
find their offsets through virtual tables. 

class A
{
    int _a;

public: 
    A(int a);_a{a}{}
    
    virtual ~A() = default;
    int getA() const {return _a};    
};

class B: public virtual A
{
    int _b; 
    
public:
    B(int a, int B): A{a} ,_b{b}{} 
    
    int getB() const {return _b);
};

class C: public virtual A 
{
    int _c;
    
public:
    c(int a, int c): ,A{a}_c{c}{}
    int getC() const {return _c};
};

class D: public B, public C 
{
    int _d;
    
public: 
    D(int a ,int b, int c, int d):A{a}, B(,b), C(,c), _d{d}{}
    
    int getD const{return _d};
};

D d{1,2,3,4}
C c{1,3};
B b{1,2} 
A a{1}; 

- The most derived class only constructs virtual base classes 

-Text
-ROdata 
-Data 
-BSS 
-Stack 

D d {1,2,3,4};
D* p = new D{1,2,3,4};


How functions are being called 
------------------------------

a) global function 

void f(int); 

int main(void)
{
    f(10); //&f(10); 
}

b) static member function 

class C
{
    static void f();
};

C c;
c.f();      //&c::f(); 


c) non-virtual member function 

class C
{
    void f();
};

C c; 
C.f(); // (&C::f)(&c);

d) virtual member function 

class B
{
public:
    virtual void f();
};

class D:public B 
{
public: 
    virtual void(f) override;
}

B b; 
B.f();  // (&b::f)(&b)      //isit virtual, pointer or reference? 

D d; 
d.f()   //(&D::f)(&d); //isit virtual, pointer or reference 

D d; 
B& b = d; 
b.f();  //b.vptr[&f](&b);       //is virtual, is reference 


Week 2 Lecture 2: Virtual Method Table (vTables)
================================================

Header
ROData 
Data
.BSS
Free Store 
Stack 

1. Function execution 

a. Non-Member Functions     
    - void f(); // (&f)();  (0x123)();
    
b. Static member functions    
    - class MyClass
    {
        static void f(); 
    };
    
    MyClass obj; 
    obj.f();
    myClass::f(); //(0x987)();
    
c. Non-static, non-virtual member functions     
    - class myClass 
    {
      void f();   
    };
    
    myClass obj; 
    obj.f();    //myClass::f(&obj);
    
d. Virtual member functions 

class B // sizeof(B) = 1 
{
public: 
        ~B();
        void f1();
        void f2(); 
};

class D:public B
{
public: 
    ~D();
    void f2();
    void f3(); 
}

int main(void)
{
    B obj;
    //B:: ~B();
}

int main(void)
{
  B* objp = new B;  //allocate memory and call constructor
  delete objp;      //call destructor and delete memory 
  //B::~B();
};

int main(void)
{
    D obj;
    //D::~D(&obj); 
}

int main(void)
{
    D*objp = new D;
    delete objp;
    //D::~D(objp);
}

int main(void)
{
    B*objp = new D;
    delete objp; //B::~B(objp);
}

- if destructor function is non virtual // B::~B(objp)
- if destructor function is virtual // objp = vptr[0](objp);

Creation of vTables
===================

1. No base class = create an empty table. If there is Base class - copy from basic vTable 
2. Add addresses of own virtual member functions, 3 virtual address = 8x3 = 24 bytes
3. Update addresses of overriden functions 

B vTables
---------
B::~B
B::f1
B::f2 

D:vTable
--------
B::~B  update to D::~D
B::f1  
B::f2  update to D::f2
D::f3 

int main(void)
{
    B*b = new B; 
    b->f1();    //b->vptr[1]();
    delete b;
    
    B*d = new D;
    d->f1()     //b->vptr[1]();
    delete d;
    
    B*d = new D;
    d->f3()     //b->vptr[1]();
    delete d;
}

What is in vTables?
-------------------

- An array of 8 bytes objects 

8bytes 
8bytes
8bytes 

B::~B
D::f1 
D::f2 


Item
-id:int 
-getID():int 

<<virtual>
ItemToBuy
- Price: int 
-getPrice():int 

<<virtual>> 
ItemToSell:
- price:int 
- getPrice():int 
    
Sword
- attack: int 
- getAttack():int 

#include <iostream>

class Item
{
    int _id;

public:
    Item(int id): _id{id}
    {
        
    }   
    
    virtual ~Item() = default;
    
    int getId() const 
    {
        return _id;
    }
};

class Item_To_Buy: public virtual item 
{
  int _price;
  
public: 
    Item_To_Buy(int id, int price): _id{id},_price{price}{}
    
    int getPrice() const
    {
        return _price; 
    }
};

class Item_To_Sell : public virtual Item
{
    int _price;

public:
    Item_To_Sell(int id, int price): item{id}, _price{price}{}
    
    int getPrice() const
    {
        return _price;
    }
};    

class Sword: public Item_To_Buy, public Item_To_Sell
{
    int _attack;

public:
    Sword(int id, int price_buy, int price_sell, int attack):
    Item{id},Item_To_Buy{price_buy}, ItemToSell{price_sell}, 
    {} 
    
    int getAttack() const 
    {
        return _attack;
    }
}

void print()
{
    Sword sword {0xABCD,16, 8, 0x1234};
    
    std::cout << sword.getId() << std::endl; //43981
    
    Item_To_Buy& itb = sword; 
    
    std::cout << itb.getPrice() << std::endl; //16
    std::cout << sword.Item_To_Sell::getPrice() << std::endl; //8


}

int main()
{
    std::cout << sizeof(Item) //16 bytes 
    std::cout << sizeof(Item_To_Buy) //32 bytes 
    std::cout << sizeof(Item_To_Sell) //32 bytes 
    std::cout << sizeof(sword) //48 bytes 

}

In memory:
===========
vptr 8 bytes 
price 4 bytes 
padding 4 bytes 

vptr 8 bytes 
id 4 bytes 
padding 4 bytes 

In memory
------
Item to buy: 
vptr 8 bytes 
price 4 bytes 
padding 4 bytes 

Item to sell: 
vptr 8 bytes 
price 4 bytes 
padding 4 bytes

Sword:


Item:
vptr 8  bytes 

-g 

gdb main 

set print demangle on 
set print asm-demangle on 
b main 
r - run  
s - step into 
n - next instruction 
p sword  - print address of sword 
x/48xb 0xffffcb90

class vector
{
    size_t count;
    int* elements;
    
public:
    vector(size_t count, int* elements):_count{count},_elements{new int[count]}
    {
        std::copy(elements, elements + count, _elements);
    }
    
    vector():_count{0},_elements{new int[count]}
    {
        
    }
    
    int* begin()
    {
        return _elements;
    }
    
    int* end()
    {
        return _elements + _count;
    }
    
    ~vector()
    {
        delete[] _elements;
    }
    
    vector& operator = (const vector &rhs)
    {
        if(this!=&rhs)
        {
            int* temp = new int[rhs.count];
            std::copy(rhs.elements, rhs._elements + rhs
        }
    }
};

int main(void)
{
    size_t ac = 10;
    int *ai = new int[ac];
    
    int arr[3] = {10,20,30};
    vector va = new vector{3,arr};
    
    / ---------------------------------------------------
    
    int* current = va.begin;
    int* last = va.end();
    
    while(current !=last)
    {
        *current = 1;
        std::cout << "\t" << *current << std::endl;
        ++current;
    }
    
    int arr2[2] = {100,200};
    vector vb{2,arr2];
    
    size_t bc = 10;
    int* bi = new int[bc];
    
    while(current! = last)
    {
        std::cout << "\t" << *current << std::endl;
        ++current;
    }
    
    /*  b <-----a */
    
    int* temp = new int[ac];
    std::copy(ai,ai+ac, temp);
    delete[] bi;
    bi = temp;
    bc = ac;
    
    /* a  <----- a*/
    
    //if(this != &rhs){....}
    
    delete va;
}

Week 3 Lecture 1:
=================

Design pattern:

1. Creational design patterns 
   - Factory method design pattern 
   - Prototype design patterns 
   
2. Behavioural Design patterns 
3. Structural 


Factory method design pattern:
------------------------------

factory                 ->          Product 
+create(..):Product*                  ^
                                      |
                                    Sword 

#include <iostream>

struct Product 
{
    virtual void print() const {std::cout << "Hello from Product\n" << std::endl;
    virtual ~Product() = default; 
};

struct Sword : Product 
{
    void print() const override {std::cout << "Hello from Sword\n" << std::endl; 
}

struct Factory 
{
  Product* create(int type)
  {
      if(type == 0)
      {
        return new Product;
      }
      
      return new Sword;
  }  
};

int main()
{
    int n;
    std::cin >> n;                          //input 0 - Hello from Product
                                            //input 1 - Hello from sword 
    Factory factory;
    Product * p = factory.create(n);
    p->print();
    delete p;
}

--------------------------------------------------------------------------------

factory                     ->           Product 
+create():Product *                         ^
    ^                                       |
    |                                       | 
FactorySword                ->            Sword
+create():Product*

#include <iostream>

struct Product 
{
    virtual void print() const {std::cout << "Hello from Product\n" << std::endl;
    virtual ~Product() = default; 
};

struct Sword : Product 
{
    void print() const override {std::cout << "Hello from Sword\n" << std::endl; 
}

struct Factory 
{
  virtual Product* create(int type)
  {
      if(type == 0)
      {
        return new Product;
      }
  }  
};

struct FactorySword: Factory
{
    Product* create() override
    {
        return new Sword;
    }
};

int main()
{
    int n;
    std::cin >> n;                          //input 0 - Hello from Product
                                            //input 1 - Hello from sword 
    Factory f;
    FactorySword fs;    
    
    Factory* factor = {n == 0}?&f:&fs;
    
    Product* p = factor->create();         //Factory::create(factory);
    p->print();
    delete p;
}

--------------------------------------------------------------------------------
cannot do:

Product p1;
Sword s2;

Product* p = &p1;

Product n(*p);      // wrong 

Product* n = p->clone();  //can 

--------------------------------------------------------------------------------
#include <iostream>

//Covariant return types 

struct Product 
{
    virutal Product* clone() const {return new Product(*this);}
    virtual void print() const {std::cout << "Hello from Product\n" << std::endl;
    virtual ~Product() = default; 
};

struct Sword : Product 
{
    Sword* clone() const override {return new Sword(*this);}

    void print() const override {std::cout << "Hello from Sword\n" << std::endl; 
}

struct Factory 
{
  virtual Product* create(int type)
  {
      if(type == 0)
      {
        return new Product;
      }
  }  
};

struct FactorySword: Factory
{
    Product* create() override
    {
        return new Sword;
    }
};

int main()
{
    int x;
    std::cin >> x;                          //input 0 - Hello from Product
                                            //input 1 - Hello from sword 
    Product p1;
    Sword s2;
    
    Product*p = (x==0)? &p1: &s2;
    
    Product* n = p->clone();                
    n->print();
    delete n;
    
    ///////
    
    Product p;
    Product* pp = p->clone();               //0 - Hello from Product 
    pp->print();        
    delete pp;
    
    Sword s;
    Product* ps = s->clone();               //0 - Hello from sword 
    ps->print();
    ps->f();
    delete ps;
}

--------------------------------------------------------------------------------
Week 3 Lecture 2 
----------------

Function dispatch 

static dispatch
===============
class C
{
    void f();
};

C c;
c.f(), // c::f(&c);


single dispatch:
================
f(p); 

void f(p)
{
    if(..) f1();
    else if (...) f2();
    else f3();
}

double dispatch
---------------
p.f();

f(B* b1, B* b2);


Circle::iShape{};
Rectangle::iShape{};

bool x = shape1->isCollision(shape2);


//Double dispach
#include <iostream>

class Circle;
class Rect;

struct IShape
{
    virtual bool isCollision(IShape* rhs) = 0;
    
    virtual bool isCollision(Circle& c) = 0;
    virtual bool isCollision(Rect& r) = 0;
    virtual ~IShape() = default;
};

class Circle: public IShape 
{
public:
    virtual bool isCollision(IShape* rhs) override
    {
       return  rhs->isCollision(*this);
    }
    
    virtual bool isCollision(Circle&) override 
    {
        std::cout << "R-C" << std:: endl;
        return false;
    }
    
    virtual bool isCollision(Rect&) override 
    {
        std::cout << "R-R" << std::endl;
        return false;
    }
};

int main()
{
    Rect r1, r2;
    Circle c1, c2;
    
    IShape* s1 = nullptr;
    IShape* s2 = nullptr;
    
    int x;
    
    std::cin >> x;
    
    switch(x)
    {
        case 0:
            s1 = &c1;
            s2 = &c2;
            break;
            
        case 1:
            s1 = &c1;
            s2 = &r1;
            break;
        
        case 2:
            s1 = r1;
            s2 = r2;
            breakl
            
        default:
            s1 = &r1;
            s2 = &c1;
            break;
    }
    
    s1->isCollision(s2);
}

--------------------------------------------------------------------------------
Multiple Dispatch:
------------------
void f(p1,p2,p3,......,pN)

if((p1 belong to B) && (p2 belong to class B) && (b3 belongs to B)) f1();

key = obj.id() + "___" + obj2.id() + "___" + obj3.id();

//Multiple Dispatch 

#include <iostream>

class Circle;
class Rect;

struct IShape
{
    virtual bool isCollision1(IShape* rhs) = 0;
    
    virtual bool isCollision2(Circle& c) = 0;
    virtual bool isCollision2(Rect& r) = 0;
    virtual ~IShape() = default;
};

class Circle: public IShape 
{
public:
    virtual bool isCollision1(IShape* rhs) override
    {
       return  rhs->isCollision2(*this);
    }
    
    virtual bool isCollision2(Circle&) override 
    {
        std::cout << "R-C" << std:: endl;
        return false;
    }
    
    virtual bool isCollision2(Rect&) override 
    {
        std::cout << "R-R" << std::endl;
        return false;
    }
};

int main()
{
    Rect r1, r2;
    Circle c1, c2;
    
    IShape* s1 = nullptr;
    IShape* s2 = nullptr;
    
    int x;
    
    std::cin >> x;
    
    switch(x)
    {
        case 0:
            s1 = &c1;
            s2 = &c2;
            break;
            
        case 1:
            s1 = &c1;
            s2 = &r1;
            break;
        
        case 2:
            s1 = r1;
            s2 = r2;
            breakl
            
        default:
            s1 = &r1;
            s2 = &c1;
            break;
    }
    
    s1->isCollision(s2);
}

--------------------------------------------------------------------------------

Visitor Design patterns:

- 4 elements 

iVisitable ------------------------------------> iVisitor {};
    ^                                              ^
    |                                              |
---------------------                    ------------------------
^                   ^                   ^                       ^
|                   |                   |                       |    
Sword             Staff          AttachVisitor           SpellCastVisitor

--------------------------------------------------------------------------------

class Sword;
class Staff;

struct IVisitor 
{
    virtual ~IVisitor() = default;
    
    virtual void visit(Sword&s) = 0;
    vitual void visit(Staff&s) = 0;
};

struct Ivisitable
{
    virtual ~Visitable() = default;
    
    virtual void accept(IVisitor& v) = 0;
};

//------------------------------------------------------------------------------

Struct IWeapon : IVisitable 
{
    
};

class Sword : public IWeapon
{
public: 
    void accept(Ivisitor& v) override 
    {
        v.visit(*this);
    }
};

class Staff : public IWeapon
{
public: 
    void accept(Ivisitor& v) override 
    {
        v.visit(*this);
    }
};

//------------------------------------------------------------------------------

struct IAction: IVisitor 
{
};

class AttackAction: public IAction
{
    public:
    void visit(Sword& s) override
    {
        std::cout << " Attacked with a sword" << std::endl;
    }
    
    void visit(Staff& s) override
    {
        std::cout << "Attacked with a Staff" << std::endl;
    }
};

class AttackAction: public IAction
{
public:
    void visit(Sword& s) override
    {
        std::cout << " Cast a spell with a sword" << std::endl;
    }
    
    void visit(Staff& s) override
    {
        std::cout << "Boom! Cast a spell with a staff" << std::endl;
    }
};

//------------------------------------------------------------------------------

int main()
{
    Sword sw;
    Staff st;
    
    AttackAction aa;
    SpellcastAction sa;
    
    int wi, ai;
    std::cin >> wi >> ai;
    
    IWeapon* w = (wi == 0)? static_cast<IWeapon*>(&sw):static_cast<IWeapon*>(&st);
    IWeapon* a = (ai == 0)? static_cast<IAction*>(&aa):static_cast<IAction*>(&sa);

    w->accept(*a);
}

--------------------------------------------------------------------------------
Non-Virtual interface idiom Template method pattern

#include <iostream>

struct Action 
{
    void execute()
    {
        f1();           //cannot be overriden (invariant)
        f2();           //must be overriden
        f3();           //can be overriden (opt-in)
        f4();           //can be overriden (opt-out)
    }

private: 
    void f1()
    {
        std::cout << "Action::f1" << std:: endl;
    }
    
    virtual void f2() = 0;      //pure virtual
    virtual void f3()
    {
        std::cout << "Action::f3" << std::endl;
    }

protected::
    virtual void f4() = 0;
};

Action::f4()    //pure virual can have definition but must be outside of scope 
{
    std::cout << "Action::f4" << std::endl;
}

class myAction : public Action 
{
    //in C++ you cannot call private functions of a base class but you can still 
    //override them if they are virtual 
    void f2() override 
    {
        std::cout << "MyAction::f2" << std::endl;
    }
    
    void f4() override 
    {
        Action::f4();
    }
};

int main()
{
    MyAction a;
    Action* aa = &a;
}

--------------------------------------------------------------------------------
#include <iostream>

class Base
{
public:
    virtual void print()
    {
        std::cout << "Base" << std::endl;
    }
};

class Derived : public Basse 
{
public: 
    void print() override 
    {
        std::cout << "Derived" << std::endl;
    }
};

void f(Base &b) //clobal functions that uses double dispatch 
{
    b.print();
}

int main()
{
    Base b;
    Derived d;
    
    Base& bb = d;
    
    f(b);
    f(bb);
}

--------------------------------------------------------------------------------
Quiz 1:
-------

/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

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
    std::cout << gc.PlayerCharacter::Vehicle::velocity << std::endl; 
}

--------------------------------------------------------------------------------
ImoveableObject - 1 byte of padding , when inherited 0 bytes 
Vehicle - 0 bytes from base class, 4 bytes from int 
playerCharacter - 8 byte from vptr which points to int, 4 bytes from int, 4 byte padding - total 16 bytes 
bonusCharacter - 16 bytes 
gameCharacter - 8 vptr from bonusCharacter, 8 vptr from playerCharacter, 4 bytes from Vehicle, 1 byte from imoveable, 3 byte padding, 4 bytes from int, 
                4 byte of padding, 8 bytes of vptr  (total 40) 

output:40
sizeof ImoveableObject: 1
sizeof Vechicle:8
sizeof PlayerCharacter:16
sizeof BonusCharacter: 16
size of GameCharacter: 32

Are virtual destructor neccessary?
- It is not needed. 

Does the access modifier alter the size of derived 
- No, access modifier: private protected and public. public and private will only impact 

Does inheriting from ImoveableObject increases the size of the vehicle class?
- Yes. Empty structure when inherited by an empty class results in 0 bytes 

Difference between composition from inheritence?
- Inheritance is design relationship from composition. We access member data from 
- Composed data 1 byte, inherit 0 bytes. 

Week 4, lecture 1
=================
1. Software develoment life cycle 
2. Code Smells 
3. SOLID


SDLC
----
Inception -> Request Analysis -> Design -> Development -> Test -> Deploy -> Maintain -> EOL --
   ^                                                                                         |
   |_________________________________________________________________________________________|

Design:
- Define Architecture 
- We want software that is complex 
- We want software that is not difficult 

Robert C. Martin (known as "Uncle Bob")
---------------------------------------
- Wrote a book called "Clean Code" 

Code smells are odours of rotting software. 

Rigidity - software hard to change, because one change forces another (fix this then need fix another one to compile) 
Fragility - software hard to change because braking changes are not apparent
Viscosity - software hard to change, because performing a change requires challenges to a larger functionality than desired
Immobility - software hard to change, because its parts are hard to move (perhaps not generic enough)
Opacity - software hard to change, because its internal structure is not apparent. 
Needless complexity - software hard to change because it contains the parts it doesnt need
Needless repetition - software hard to change, because it contains repeated parts or modules. 

SOLID:
------

SRP - Single responsible principle.
OCP - Open(for extension)/Close (for modification) principle 
LSP - Liskov substitution principle 
ISP - Interface segregation principle
DIP - Dependency invertion principle

SRP:
- Each class should have one reason to change. Each class manage one thing. 
- Breaks between classes

OCP:
- every class should be open for extension and close for modification.
- meaning function should be virtual
- derived class should not be constraint 

LSP: 
- a square is not a rectangle 

LSP:
- seperate function based on their responsibilities 

DIP:
- high level code should not be depend on specific implementation of low level behavious
- high level code should depend on the interface of the functionality, not the specific implementation. 

class Rect
{
    int _w;
    int _h;

public:
    Rect(int w, int h):_w{w}, _h{h}{}
    
    int w() const { return _w;}
    int h{} const { return _h;}
    
    void w(int value){_w = value;}
    void h(int value){_h = value;}
    virtual ~Rect() = default; 
};

class Square : public Rect 
{
public: 
    Square(int side): Rect(side, side){}
};

class EditableRect : public Rect
{
    void w(int value){_w = value;}
    void h(int value){_h = value;}
}

void f(Rect& r)
{
    //todo 
    r.w(20);    //violate LSP. everywhere u use a base class, it should be legal to use any derived class of that class 
}

Square s{10};
f(s);

Week 4 lecture 2
================

class Printer 
{
private:

    void rollOutPaper(){}
    void feedPaper(){}

public: 

    void setPrinter(Printer& p)
    {
        p.rollOutPaper();
        p.feedPaper();
    }
};

class ContinuousPaperPrinter: public Printer
{
public: 
    virtual void setPrint(){}
};

void setPrint(Printer& p)
{
    p.setPrint();
}

--------------------------------------------------------------------------------
main:  - sets up the program and pass high level business code to the program

1.Encapsulation
2.Inheritance 
3.Polymorphism 
4.Generic Programming

--------------------------------------------------------------------------------
//Function Templates
#include <iostream>

//void add_and_print<T>(T a, T b)  <--- template
//void add_and_print<int>(int a, int b) <----- template instance 

template <typename T1, typename T2, int TFactor = 1>    //default parameter
void add_and_print(T1 a, T2 b)
{
    T1 s = (a + b) * TFactor;
    std::cout << s << std::endl;
}
/*
void add_and_print(float a, float b)
{
    float s = a + b;
    std::cout << s << std::endl;
}
*/

int main()
{
    int x = 10,
        y = 20;
    
    add_and_print<int, int, -1>(x,y);     //30      //-30

    float fx = 1.5f,
          fy = 3.7f;
    
    add_and_print<double, float, -2>(fx,fy);   //5.2        //-10.4
}

-------------------------

template <typename T1, typename T2, int TFactor>  
void add_and_print(T1 a, T2 b)
{
    T1 s = (a + b) * TFactor;
    std::cout << s << std::endl;
}
/*
void add_and_print(float a, float b)
{
    float s = a + b;
    std::cout << s << std::endl;
}
*/

int main()
{
    int x = 10,
        y = 20;
    
    add_and_print(x,y);     //30 , still works without <int, int>  

    float fx = 1.5f,
          fy = 3.7f;
    
    add_and_printfx,fy);  //5.2, still works without <double, double>
}
--------------------------------------------------------------------------------
#include <iostream> 

void f(int){ std::cout << "f(int)" << std::endl;}

template <typename T> 
void f(T) {std::cout << "f<T>(T)" << std::endl;}

template <>
void f(int*){std::cout << "f<int*>(int*)" << std::endl;} 

int main()
{
    f(10);          //f(int)
    f(static_cast<short>(20)); //f<T>(T)
    
    int* p = nullptr;
    f(p);
    
}

--------------------------------------------------------------------------------
Week 5 lecture 1:

#include <iostream>
#include <vector>

//in C++, you can create templates for namespace variables
template <typename T>
const T PI = static_cast<T>(3.1415);

//In C++, you can create templates for functions 
template <typename T>
T add_value_to_PI(T value)
{
    return PI<T> + value;     
}

//In C++, you can create templates for classes
template <typename T>
class Vector 
{
    size_t _count;
    T* _data;

public:
    //default constructor
    Vector():_count{0},_data{new T[_count]}{}
    
    //copy constuctor
    Vector(const Vector&):_count{rhs._count}, _data{new T[_count]}
    {
        std::copy(rhs._data, rhs._data + rhs._count, _data);
    }
    
    //copy assignment
    Vector& operator=(const Vector& rhs)
    {
        //need check if A=A;
        if(this != &rhs)
        {
            T* temp = new T[rhs._count];
            std::copy(rhs._data, rhs._data + rhs._count, temp);
            delete[] _data;
            _data = temp;
            _count = rhs.count;
        }
        
        return *this;
    }
    
    ~Vector()
    {
        delete[] _data;
    }
};

template <typename T>
Vector<T>::~Vector()
{
    delete[] _data;
}

//in C++, you can create template for data types as aliases to data types 
template <typename T>
using v = std::vector<T>;

int main()
{
    std::cout << PI<float> << std::endl;                        //3.1415
    std::cout << add_value_to_PI<double>(100.0) << std::endl;   //103.141
    
    v<int> v1, v2;
    
    v1=v2;
    
    
}

--------------------------------------------------------------------------------
#include <iostream>

// 1 1 2 3 5 8 13

size_t Fib(size_t N)
{
    if(N == 1) return 1;
    if(N == 2) return 1;
    return Fib(N -1) + Fib(N - 2);
}

int main()
{
    std::cout << Fib(6) << std::endl;   //8 
    std::cout << Fib(12) << std::endl;   //144 
    std::cout << Fib(20) << std::endl;   //6765
}

--------------------------------------------------------------------------------
#include <iostream>

// 1 1 2 3 5 8 13

size_t Fib(size_t N)
{
    if(N == 1) return 1;
    if(N == 2) return 1;
    return Fib(N -1) + Fib(N - 2);
}

template <size_t N>
struct FibM_imple 
{
  static const size_t value = FibM_impl<N-1>::value + FibM_impl<N-2>::value;  
};

template<>
struct FibM_impl<1>             //if N is 1, this struct will be called and return
{
    static const value = 1;
};

template <>
struct FibM_impl<2>
{
    static const value = 2;
}

template <> struct FibM_impl<1> { static const size_t value = 1;};
template <> struct FibM_impl<2> { static const size_t value = 1;};

template <size_t N>
const size_t FibM = FibM_impl<N>::value;

//FibM_imple<1>::value = 1
//FibM_imple<2>::value = 1
//FibM_imple<3>::value = 2
//FibM_imple<4>::value = 3
//FibM_imple<5>::value = 5
//FibM<5> = 5


int main()
{
    std::cout << FibM<5> << std::endl;
    std::cout << FibM<40> << std::endl;
}

--------------------------------------------------------------------------------
// Lvalues, Rvalues 

//a = 5;
//5 = a;

//every Lvalues has a place in memory.


/*
Lvalue:
- A name of a variable, function parameter, data member
- A reference to an lvalue, int& x = i;
- Any reference that has a name is an lvalue
- A function result returned as lvalues reference, int& f();
- string literal: "ABC"
- A result of an operator that returns lvalue; int *p = &i; *p = 5;
*/

/*
Rvalues: (split into 2 category)

pr-value - "pure r-values":
- Literals, except string literals
- Results of functions returning non-reference value, int f();
- the this pointer.

x-value - "expiring values" / "explicit r-values":
- Results of functions returning r-value reference, int&& f();
- Results of explicit conversion, i.e. static_cast<int&&>(v);
*/

include <iostream>

template <typename T>
class Vector
{
    size_t _count;
    T* _data;
    
public:
    
    Vector():_count{0}, _data{new T[_count]}{}
    
    Vector{const Vector& rhs): _count{_rhs._count}, _data{new T{_count}}
    {
        std::cout << "Not so smart!" << std::endl;
        std::copy(rhs._data, rhs._data + rhs._count, _data);
    }
    
    Vector& operator =(Vector&& rhs)
    {
        std::cout << "Smart!" << std::endl;
        
        size_t t_count = rhs._count;
        rhs._count = _count;
        _count = t_count;
        
        T* t_data = rhs._data;
        rhs._data = _data;
        _data = t_data;
    }
    
    Vector& operator=(const Vector& rhs)
    {
        if(this!=&rhs)
        {
        std::cout << "Not so smart!" << std::endl;
            T* temp = new T[rhs._count];
            std::copy(rhs._data, rhs._data + rhs._count, temp);
            delete[] _data;
            _data = temp;
            _count = rhs._count;
        }
        return *this;
    }
    
    //move constructor???
    Vector(Vector&& rhs): _count{rhs._count}, _data{rhs._data}
    {
        rhs._count = 0;
        rhs._data = new T[0];
    }
    
    ~Vector()
    {
        delete[] _data;
    }
};

Vector<int> createVector()
{
    return Vector<int>{};
}

/*

Reference collapsing rules 

int&    T&  -> int&             //lvalues reference
int&&   T&  -> int& 
int&    T&& -> int&             //could be rvalue or lvalue reference 
int&&   T&& -> int&&

*/

//std::remove_reference
//std::remove_reference_t
template <typename T> struct remove_reference{using type = T;};
templare <typename T> struct remove_reference<T&>{using type = T;}


template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

//std::moe
template <typename T>
remove_reference_t<T>&& move(T&& t)             //always return rvalue 
{
    return static_cast<remove_reference_t<T>&&>(t);
}

Vector<int>&& move(Vector<int& v)               //lvalue reference 
{
    return static_cast <Vector<int> &&>(v);     //rvalue reference 
}

Vector<int>&& move(Vector<int>&& v)
{
    return static_cast< Vector<int>&& >(v);
}

int main()
{
    std::cout << "1" << std:;endl;
    Vector<int> v1;
    
    std::cout << "2" << std::endl;      //not so smart 
    Vector<int> v2{v1};
    
    std::cout << "3" << std::endl;      //not so smart
    v2 = v1;                            //v1 is rValue
    v2 = static_cast< Vector<int>&& >(v1);
    v2 = move(v1);
    
    std::cout << "4" << std::endl;      
    Vector<int> v3{createVector()};     
    
    std::cout << "4" << std::endl;      //smart 
    v3 = static_cast < Vector<int>&& > (createVector());//createVector() is pr value
}

--------------------------------------------------------------------------------
Week 5 Lecture 2
================
// Lvalues
// Rvalues 
// pr-values 
// x-value 

// Lvalue references bind to Lvalues, or Rvalues if const e.g. const int& r = 5;
// Rvalue references bind to rvalues  - const T&&
// Universal references - T&& 

one of the exam questions: 

template <typename T> 
void foo(T&& t){ T bar{std::move()};    //T&& t is rvalue 
//void foo(float&& t){float bar{std::move(t);} // funcdamental type have a move constructor 
//move - given lvalue or rvalue, always results to rvalue

//void foo(float& t){float& bar{std::move(t)};} 

//void foo(float&& t){float&& bar{std::move(t)};}

//void foo(float&& t){float bar{std::move(t)};}

int main()
{
    float x = 0.5f 
    float& rx = x;
    float&& rrx = std::move(x); //reference, rrx is lvalue. reference will be used as lvalue in later code 
    float* px = &x;

//int& given to T& result in int& 
//int&& given to T& result in int& 
//int& givent to T&& result in int& 
//int&& given to T&& result in int&& 

//T     T&&     NC
//support moving    

//function call 
// determine whether the function calls are T or T&& or NC for it to work 

    foo<float>(x);  // T is float , T&& is float&& , does it compile? does line compile and function compile?
                //x is Lvalue float, doesnt compile. function compile but this line doesnt compile. 

    foo <float(std::move(x)); //correct way , this line will compile 

    foo <float>(*px); //T is float, T&& is float&&, px is float*, *px is a Lvalue, has memory, this code is NC  

    foo<float&>(std::move(x)); //T is float&, T&& is float&, this line is NC 
    
    foo<float&>(rrx); //T is float&, T&& is float&, compiles(?)
    
    foo<float&&>(std::move(rrx)); //T is float&&, T&& is float&& , rrx is lvalue move to rvalue, move is to convert lvalue to rvalue, compiles both  
    
    foo<float>(rrx); // T is float, T&& is float&&, rrx is lvalue
    
    foo(x); //idk the T, x is float , T&& is float&, T is float& (a pair)  
    
    foo(std::string{"A"}); // conversion constructor of std::string, returns rvalue string, std::string{"A") is rvalue, 
                           //function must be std::string&&, T is string, T&& is std::string&& 
    
    foo<std::string&&>(std::string{"A"}); 
    
    foo(px); //T idk, px is lvalue, T&& is float*&, T is float*&, doesnt compile 
    
    foo(px+7); //px + 7 is rvalue, T&& is float*&&, T is float*, compiles 
}

--------------------------------------------------------------------------------
#include <iostream> 

/*

memorise these functions:

std::remove_reference
std::swap 
std::move 
.....



*/

template <typename T> remove_reference      {using type = T;};
template <typename T> remove_reference<T&>  {using type = T;};
template <typename T> remove_reference<T&&> {using type = T;};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

remove_reference_t<T>&& move(T&& t)
{
    return static_cast<remove_reference_t<T>&&>(t);
}

template <typename T>
T&& forward(revemoe_reference_t<T>& t)  //caste to lvalue
{
    return static_cast<T&&>(t);
}

template <typename T>
void swap(T& a, T& b)
{
    T t{move(a)};
    a = move(b);
    b = move(t);
    
    //T t{std::move(a)};
    //a = std::move(b);
    //b = std::move(t);
}

template <typename T>
class vector
{
    size_t _count; 
    T* _data;     
}

public: 
    vector(): _count{0}, _data{new T{_count}}
    {
                std::count << "vector::vector()" << std::endl;

    }       
        
    ~vector()
    {
        delete[] _data;
    }
    
    //copy semantics 
    vector{const vector& rhs): _count{rhs._count}, _data{new T[_count]}
    {
        std::copy(rhs._data, rhs._data + rhs._count, _data);
        std::count << "vector::copy()" << std::endl;
    }
    
    vector& operator=(const vector& rhs)
    {
        if(this !=&rhs)
        {
            T* temp = new T[rhs._count];
            stf::copy(rhs._data, rhs._data + rhs._count, temp);
            delete[] _data;
            _data = temp;
            _count = rhs._count;
            std::count << "vector::op=()" << std::endl;
        }
        
        return *this;
    }
    
    //move semantics
    vector(vector&& rhs): _count{rhs._count}, _data{rhs._data}
    {
        rhs._count = 0;
        rhs._data = new T[rhs._count];
        std::count << "vector::move()" << std::endl;
    }
    
    //assignment operator 
    vector& operator=(vector&& rhs)
    {
        size_t t =_count;
        _count = rhs._count;
        rhs._count = t;
        
        std::count << "vector::op=(move)" << std::endl;
        swap(_count, rhs._count);
        swap(_data, rhs._data);
    }
};

template <typename T1, typename T2>
void assign(T1& a, T2&& b)              //copy assignment 
{
    a = forward<T2>(b);
}

template<typename T>
class UniquePtr
{
  T* _data; 
  
  UniquePtr(T* data) : _data{data}
  {

  }
  
public: 
    template <typename Arg> 
    static UniquePtr create(Arg&& arg)
    {
        return UniquePtr{new T}
        {
            new T{arg};
        }
    }
    
    UniquePtr(const UniquePtr{&} = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    
    ~UniquePtr()
    {
        delete _data;
    }
};

int main()
{
    vector<int> v1; //default constructor
    vector<int>v2{v1};//copy constructor
    vector<int>v3(std::move(v1);//move constructor 
    
    v2 = v1;            //copy assignment 
    v3 = std::move(v2);//move assignment 
    
    assign(v2, v1);
    assign(v3, std::move(v2));
    
    (void) v3; 
    
    using UP = UniquePtr < vector<int> >; 
    UP u1 = UP::create(v1); 
    UP u2 = UP::create(std::move(v1)); 
    
    UniquePtr < vector<int> > u1 = UniquePtr < vector<int> >:: create(v1);
    
    (void) v3;
    (void) v1;
    (void) v3;
}
--------------------------------------------------------------------------------

Week 6 lecture 2: 
-----------------
//Type Erasure Pattern
What information is provided by a data type(i.e of a variable)

- size, alignment 
- representation 
- supported and disallowed operations 
- int vs 4 char, difference is int need to align to 4 bytes. 
- legal values 

int a / int b  //result int 
float a % float b //  nc

c++ is a strongly-typed language, c++ is a statically typed language 

phython and javascript, not statically typed.

- Base class(IConcept) pointers let us accept derived objects of different types 
- A class wrapping a base class pointer as a private data member can abstract 
  away inheritance from the user. 
- We can wrap target data types into class templates model<T>
 sharing a common base class pointer. 

- We will used perfect forwardning to pass parameters to the constructor of the 
target object inside a model. 

#include <iostream>

struct Sword
{
    Sword()
    {
        std::cout << "Sword::Sword()" << std::endl;
    }
    
    Sword(const Sword&)
    {
        std::cout << "Sword::Sword(copy)" << std::endl;
    }
    
    ~Sword()
    {
        std::cout << "Sword::~Sword()" << std::endl;
    }
    
    void attack()
    {
        std::cout << "Attacked with a sword!" << std::endl;d
    }
}


class Bow 
{
    int _arrows;
    
public: 
    Bow(int arrows):_arrows{arrows}
    {
        std::cout << "Bow::Bow(int)" << std::endl;
    }
    
    Bow(const Bow& rhs): _arrows{rhs._arrows} 
    {
        std::cout << "Bow::Bow(copy)" << std::endl;
    }
        
    ~Bow()
    {
        std::cout << "Bow::~Bow()" << std::endl;
    }
    
    void attack()
    {
        std::cout << "Attacked with a bow" << --_arrows << "arrows left" << std::endl;
    }
}

struct Staff
{
  Staff()
  {
    std::cout << "Staff::Staff()" << std::endl;
  }      
  
  Staff(const Staff&)
  {
    std::cout << "Staff::Staff(copy)" << std::endl;
  }
  
  ~Staff()
  {
    std::cout << "Staff::~Staff()" << std::endl;
  }
  
  void doAttack()
  {
    std::cout << "Attacked with a staff!" << std::endl;
  }
  
};

//Adaptor design pattern 
class StaffAdaptor
{
    Staff _instance;
public: 
    void attack() 
    {
        _instance.doAttack();
    }
}

class Weapon
{
  struct IConcept
  {
    virtual void attack() = 0;
    virtual ~IConcept() = default;
  };
  
  IConcept* _concept; 
  
  template<typename T> 
  class Model : public IConcept
  {
      T _instance; 

    public:
        template <typename Arg> 
        Model(Arg&& arg) : _instance{std::forward<Arg>(arg)}
        {
        
        }
        
        void attack() override 
        {
            _instance.attack();
        }
  };
  
  Weapon(IConcept* concept): _concept{concept}
  {
    
  }
  
public: 
    template <typename T, typename Arg> 
    static Weapon create(Arg&& arg)
    {
        return Weapon{new Model<T>{std::forward<Arg>(arg)};
    }        
    
    void attack()
    {
        _concept->attack();
    }
    
    ~Weapon()
    {
        delete _concept;
    }
    
};

void performAttack(Weapon&& w)  //rvalue 
{
    w.attack();
}

int main()
{
    Sword s; 
    Bow b{50}; 
    StaffAdaptor t; 
    
    performAttack(Weapon::create<Sword>(s));    //copy construction 
    performAttack(Weapon::create<Bow>(b));    //copy construction 
    
    performAttack(Weapon::create<Bow>(100));    //copy construction 
    performAttack(Weapon::create<Staff>(b));    //copy construction 
}

-------------------------------

ACACE
ECACA


#ifndef L_H
#define L_H

#include <string>

struct logger 
{
    virtual void log(const std::string&)=0;
    virtual ~Logger() = default;
};

#endif 



DIP, sizeof(logger) = 8 bytes , 

--------------------------------------------------------------------------------
template<typename T> 
class my_basic_string 
{
  size_t _length;
  T* _txt;
  
public: 
  // All required declarations are here 
};

template<typename T>
my_basic_string<T>::my_basic_string{const my_basic_string& rhs}:_length{rhs.length}, _text{new T[rhs.length + 1]}
{
    std::copy(rhs._text, rhs.text + rhs.length + 1, _text);
}

template<typename T>
my_basic_string<T>::my_basic_string(my_basic_string&& rhs):_length{rhs._length}, _text{rhs._text}
{
    rhs._length = 0;
    rhs._text = new T[1];
    rhs._text[0] = 0;
}

template <typename T>
my_basic_string<T>& my_basic_string<T>::operator=(const my_basic_string<T>& rhs)
{
    if(this != &rhs)
    {
        T* temp = new T[rhs._length + 1];
        
        std::copy(rhs._text, rhs._text + rhs._length + 1, temp);
        
         delete[] _text;
        _text = temp;
        _lenth = rhs.length;
    }
    
    return *this;
}
template <typename T>
my_basic_string<T>& my_basic_string<T>::operator=(my_basic_string&& rhs)
{
    if(this != &rhs)
    {
        std::swap(_length, rhs._length);
        std::swap(_text, rhs.text);
    }
    
    return *this;
}


template<typename T>
my_basic_string<T>::~my_basic_string()
{
    delete[] _text;
}


--------------------------------------------------------------------------------
Week 8 lecture 1 
----------------

Int literals in C++

-Decimals       int x = 10;
-Hexadecimal    int h = 0xF0;
-Octal          int o = 0100;   //0 eighth, 1 sixtyfour 
-Binary         int b = 0b;     // 0000110101

which context would use hexadecimal?

-address. 

Hexadecimal is shorter than decimals. 

00001111 // first 4 zero will be 0, next 4 zero will be F = 0x0F

Octals:

000001111 // first 4 zero = 0, next 001 = 1, next 1111 = 7 , total = 017


Health of player, position : decimal
Hexadecimal - visual representation
Binary - CPU instruction

Why binary?

- Not all data is byte-aligned (compression, encryption) 
- Computers are based on binary 
- Some data is byte aligned [RGB (255,128,0) - 0xFF8000]
- Not all memory representats RAM (memory mapped I/O)
- A single byte can store multiple information elements. 

Bitwise operations in C++
--------------------------
NOT : ~

unsigned int x = 6;   // 0b00110
unsigned int z = ~x; //  0b11001   

AND : &

unsigned int x = 6;     // Ob000110
unsigned int y = 12;    // Ob001100
unsigned int z = x&y;   // Ob000100 // 4 

//Controlled clearing (with 0)

unsigned int x = 0b00 0110  //want to change last 2nd from 1 to 0 
unsigned int y = 0b00 0010  //create a mask 

unsigned int z = x & ~y;    

OR: |

unsigned int x = 6;      //0b00..0110
unsigned int y = 12;     //ob00.01100
unsigned int z = x | y ; //0b0..01110

//Controlled setting (with 1)

unsigned int x = 0b000110;  // want to turn on the last 4th from 0 to 1
unsigned int y = 0b001000;

unsigned int z = x | y; // 0b001110

XOR : ^

unsigned int x = 6;     //0b0....0110
unsigned int y = 12;    //0b0...01100
unsigned int z = x ^ y; //0b0...01010 (if different 1, same 0)

//Controlled negation (with 1)

unsigned int x = 0b0000..01110; //switch third one, if 1 become 0, if 0 become 1 
unsigned int y = 0b000...00100;

unsigned int z = x ^ y;         //0b....01010


--------------------------------------------------------------------------------
Week 8 Lecture 2: 
-----------------
//Examples of bitwise operations 
//bit fields
//Union / std::variant 
//What is a bit or byte?
//Complex declarations 
//C++ pointers and operators 

#include <iostream>
#include <iomanip>

int main()
{
    /////////////////////////////OR////////////////////////////////////////////

    unsigned short int x = 0b0000'0000'0000'1100;
    unsigned short int y = 0b0000'0000'0000'1010;
    
    unsigned short int z = x | y; 
    
    std::cout << "X: " << std::dec << X << "; 0x" << std::hex << x << std::endl;      //12; 0xc
    std::cout << "Y: " << std::dec << X << "; 0x" << std::hex << Y << std::endl;      //10; 0xa 
    std::cout << "Z: " << std::dec << X << "; 0x" << std::hex << Z << std::endl;      //14; 0xe 1110
    
    unsigned short int x = 0b0000'0000'1100'1100;
    unsigned short int y = 0b0000'0000'1010'1010;
    
    unsigned short int z = x | y; 
    
    std::cout << "X: " << std::dec << X << "; 0x" << std::hex << x << std::endl;    //204; 0xcc
    std::cout << "Y: " << std::dec << X << "; 0x" << std::hex << Y << std::endl;      //170; 0xaa 
    std::cout << "Z: " << std::dec << X << "; 0x" << std::hex << Z << std::endl;      //238; 0xee 
    
    ////////////////////////////////////////////////////////////////////////////
    
    unsigned short int x = 0b0000'0000'0000'1100;
    unsigned short int y = 0b0000'0000'0000'1010;
    
    unsigned short int z = static_cast<unsigned short int>(x & ~y);  //negation promotes int to short unsigned in  
    
    std::cout << "X: " << std::dec << X << "; 0x" << std::hex << x << std::endl;      //12; 0xc
    std::cout << "Y: " << std::dec << Y << "; 0x" << std::hex << Y << std::endl;      //10; 0xa 
    std::cout << "Z: " << std::dec << Z << "; 0x" << std::hex << Z << std::endl;      //4; 0x4

    /////////////////////////////XOR////////////////////////////////////////////

    unsigned short int x = 0b0000'0000'0000'1100;
    unsigned short int y = 0b0000'0000'0000'1010;
    
    unsigned short int z = x ^ y;  //negation promotes int to short unsigned in  
    
    std::cout << "X: " << std::dec << X << "; 0x" << std::hex << x << std::endl;      //12; 0xc
    std::cout << "Y: " << std::dec << Y << "; 0x" << std::hex << Y << std::endl;      //10; 0xa 
    std::cout << "Z: " << std::dec << Z << "; 0x" << std::hex << Z << std::endl;      //6; 0x6
    
    std::cout << "X: " << std::dec << X << "; 0x" << std::oct << x << std::endl;      //12; 0x14
    std::cout << "Y: " << std::dec << Y << "; 0x" << std::oct << Y << std::endl;      //10; 0x12 
    std::cout << "Z: " << std::dec << Z << "; 0x" << std::oct << Z << std::endl;      //6; 0x6
        
    unsigned short int x = 0b0000'0000'0000'1100;
    unsigned short int y = 0b0000'0000'0000'1000;
    
    unsigned short int z = x ^ y;  //negation promotes int to short unsigned in  
    
    std::cout << "X: " << std::dec << X << "; 0x" << std::hex << x << std::endl;      //12; 0xc
    std::cout << "Y: " << std::dec << Y << "; 0x" << std::hex << Y << std::endl;      //8; 0x8 
    std::cout << "Z: " << std::dec << Z << "; 0x" << std::hex << Z << std::endl;      //4; 0x4
    
    /////////////////////////////BITWISE SHIFT LEFT/////////////////////////////
    
    unsigned char i = 3;
    
    unsigned short int x = 0b0000'0000'0000'1100;
    unsigned short int y = 0b0000'0000'0000'1000;
    
    unsigned short int z = static_cast<unsigned short int>(x ^ (1 << i));  //1 * 2^3 = 8
    
    std::cout << "X: " << std::dec << X << "; 0x" << std::hex << x << std::endl;      //12; 0xc
    std::cout << "Y: " << std::dec << Y << "; 0x" << std::hex << Y << std::endl;      //8; 0x8 
    std::cout << "Z: " << std::dec << Z << "; 0x" << std::hex << Z << std::endl;      //4; 0x4
    
    /////////////////////////////BITWISE SHIFT RIGHT////////////////////////////
    
    unsigned char j = 0;
    
    while(y)    //y = 8 
    {
        j++;
        y = y >> 1;
    }
    
    if(j ==0)
    {
        std::cout << "Mask was empty";
    }
    else
    {
        std::cout << "Bit index was: " << (j-1);    //bit index was: 3 
    }
    
    //shifting left to right, if unsigned or positive sign, will be shifting leading 0 
    //shifting left to right, if unsigned or negative sign, will be shifting leading 1 
    //shifting right to left, 0?
}

struct RGB16
{
    unsigned char r : 5;
    unsigned char g : 6; 
    
    unsigned char   : 0;    //the following data member will start with amount of padding to align with next 
    
    unsigned char b : 5;
}

// 2 bytes = 16 bits
// 5 bits for red, 6 bits for green , 5 for blue 

struct Data
{
    unsigned char motor : 4;
    
    unsigned char led1 : 1; 
    unsigned char led2 : 1;   
    unsigned char led3 : 1;
    unsigned char led4 : 1;
}
--------------------------------------------------------------------------------
Union is like a structure, but data overlaps. 

struct MyFloat //IEEE754 
{
    unsigned int m : 23;        //mentissa
    unsigned int e : 8; 
    signed int s : 1;
};

struct MyData 
{
  bool use_union;   //1 byte 
                    //3 byte padding 
  union             //4 bytes 
  {
    float f; 
    MyFloat my;
  };
};

int main()
{
    Mydata data;
    data.use_union = true; 
    data.my.m = 0;
    data.mf.e = 126;
    data.mf.s = -1;
    
    (void)data;
    
    data.use_union = false;
    std::cout << sizeof(MyData) << std::endl;   //8 
    std::cout << std::hex << std::endl;
    
    std::cout << data.f;    //-0.5
    
    data.f = 23.4f;
    
    (void)data;
}

----------------------------------
#include <iostream> 
#include <variant>

int main()
{
    std::variant<int, float> v = 100;
    std::cout <<  std::get<int>(v) << std::endl;    //100
    
    try
    {
        std::cout << std::get<float>(v) << std::endl;
    }
    catch (const std::bad_variant_access& e)
    {
        std::cout << e.what() std::endl;
        std::cout <<  std::get<int>(v) << std::endl; 
    }
}

-------------------------------------
<climits>

CHAR_BIT = 8;

-------------------------------------

void* p1; 
unsigned int* p2;   //pointer to 4 byte object in memory 
100/ CHAR_BIT rounded up will give the number of bytes i need //define an array of 100 bits

[20 bits / 8 rounded up ]->3 will give u 8 bytes which is 24 bits 
[20 bits % 8 ] // give u position of bits

0000 0000
0000 0000
0001 0000 

-----------------------------------------
Pointers to data:
-----------------
struct S
{
    int x;
};


in C++: S* p3;      //pointer to struct S
in C: struct S* p4; //pointer to struct S

////////////
S* p5[100];          //array of pointers //pointer to struct array of 100 

///////////
S(*p6)[100];         //pointers of array //pointer to 100 s 

(*p7)[100][500]     //a pointer to an array of 100 arrays of 50s

C:
typedef S row[50];
typedef row table [100]; 
table* p7; 

C++:
using row = S[500];
using table = row[100];
table* p7;

Pointers to functions: 

//f1 is a pointer to a function that returns void and access nothing 

C++:    void(*f1)();
C:      void(*f1)(void);

using F1 = void(*)();

int a; 
double d;

F1 f1; 

// S(*p7)[100][50];

S(*)[100][50] MyFunction()
{
    S(*p7)[100][50];
    return p7;
}

using pTable = S(*)[100][50];

pTable MyFunction(F1 f1)
{
    pTable 7;
    return p7;
}

//using f2 = ptable(*)();
//using f2 = (S(*)[100][50])();

using f2 =  pTable(*)(F2);

//Pointers to methods(member function)

#include <iostream>

class MyClass
{
public: 
    void print()
    {
        std::cout << "Hello world!" << std::endl;
    }
};

int main()
{
    using Func = void(MyClass::*)();
    Func f = &MyClass::print;
    
    MyClass obj;
    obj.print();        //Hello world!
    
    (obj.*f)();         //Hello world!
    
    (void)f;

    MyClass* obj_ptr = &obj;
    obj_ptr->print();   //Hello world!   
    (obj_ptr->*f)();    //Hello world!
}

--------------------------------------------------------------------------------
Week 9 Lecture 1:
-----------------
#include <iostream>
#include <string>

class myClassx
{
public: 
    std::string hello {"Hello "};
    std::string world {"World\n"};
    
    //void printHello() {std::cout << " Hello "; };
    //void printWorld() {std::cout << "World\n"; };
    
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
    (obj_ptr->*f)();                //hello world 
    
    /////////////////////////////////////////////////
    //Pointer to a data member
    
 //   std::string (myClass::*d);
    using D = std::string (myClass::*);
    
    D d = &myClass::hello;
    (obj.*d) = "Bye ";              
    obj.printHello();               //Bye
    
    d = &myClass::world;
    (obj_ptr->*d) = "bye!";         
    obj_ptr->printWorld();          //Bye bye!
    
}
--------------------------------------------------------------------------------
//Smart pointers 

//std::unique_ptr<T> 
//std::shared_ptr<T>
//std::weak_ptr<T>

//std::auto_ptr<T>      //dont use, implemented before C++ move semantics 

//unique pointers have move semantics but dont have copy semantics 
unique pointer<myClass*>    myClass  
------------------------    -------
ptr                     ->  obj //no one else can point to obj, its unique 

//shared pointers 
shared pointer<myClass*>    myClass  
------------------------    -------
ptr                     ->  obj //only last shared pointer to the object will destroy the object  
ptr2                    -----^

shared pointer -> control block, 2 data members: strong ref counter(++count, --count), and pointer -> obj 

//circuar linked list has a shared pointer with the head and the last data but cannot be deleted 
//To delete circular linked list, change shared pointer to weak pointer 
//weak pointer points to the object without the increasing count of control block

--------------------------------------------------------------------------------
//Unique Pointer 
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
    
    print1(*up);        //Hello World 
    print1(*up.get());  //Hello World 
    
    print2(up);         //Hello World 
    print2(up.get());   //Hello World 
    
    print3(std::move(up));  //Hello World     
    //print3(up);         //error , takes in parameter by value , means by copy 
}

--------------------------------------------------------------------------------
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
    std::shared_ptr<myClass> up = std::make_shared<myClass>();
    
    print1(*up);        //Hello World 
    print1(*up.get());  //Hello World 
    
    print2(up);         //Hello World 
    print2(up.get());   //Hello World 
    
    print3(up);         //Hello World 
}

--------------------------------------------------------------------------------
//Weak pointer 
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
    
    print3(sp);         //Hello Word 
}

--------------------------------------------------------------------------------
//Weak pointer - lock?
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
}

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

--------------------------------------------------------------------------------
/*
try - mark in the call stack a place that can handle exceptions; 
throw obj; - crease an exception description object on start stack unwinding 
catch - represents a sequence of statements for handling exceptions of the specific type 
throw - resume stack unwinding; rethrow an exception 

Stack unwinding 
----------------
- Variables of primitive type (int, float) - discard 
- Variables of class types - run destructor, discard 
- Function parameters of primitive types - discard 
- Function parameters of class types - call destructor, discard 
- Function return object - discard 
- Function return address - jump to the address, discard, resume unwinding 
- Place marked by TRY, match CATCH clauses and execute the first mtch, or continue unwinding if none. 
- Bottom ofo the call stack - std::terminate();

Exception safety guarantees
---------------------------
- No gurantee , no leaks, ends in a valid state (e.g fstream open)
- Basic guarantee, if exception happens, no memory leaks but will switch states 
- Strong guarantee, like basic + rollback(if fail can retry) e.g. std::vector
- no throw, exceptions that happen internally will not be thrown e.g. std::swap
- no fail, no exceptions will even happen. e.g. 3 way swap 
*/

#include <iostream> 

void doSomething()
{
    if(...)
    {
        throw myExeption{};
    }
}

int main()
{
    try 
    {
        doSomething();
        doOtherThing();
    }
   
   catch(const std::exception& e)
    {
        std::cerr << e.what() std::endl;
        throw;
    }

    catch(const char* s)
    {
        std::cerr << s << std::endl;
    }

    catch(...)
    {
        std::cerr << "Something terrble has happened!" << std::endl;
    }

    //continue with code 
}

--------------------------------------------------------------------------------
Week 9 Lecture 2:
-----------------
- Exceptions 
- Exceptions safety guarantees 
- Rule of 3/5/0
- ALL(?)
- operators new/delete 
- quiz 

Good Exception:
- Stardardised exception reporting and handling 
- Clever execution flow in case of exceptions 
- Handling exception description obj in a type-specific way 

Bad Exception:
- Exception thrown break sequential nature of execution 
- Exceptions may crash programs. 
- Leaning curve 
- Primitve object type(in, float, int*) get discarded during unwinding without cleanup. 


                No Guarantee    Basic Guarantee      Strong Guarantee   No throw / No fail 
Can throw?           Yes             Yes                     Yes                 No 

Memory leak?         Yes             No                      No                  No 

Object in valid      No              Yes                     Yes                 Yes 
technical state?

Object in valid      No              No                      Yes                 Yes                   
business state?

Object in previous   No              No                      Yes                  - 
state?

Strategy            None         Do not leak            Commit/Rollback     Never unwind callstack outside or at all 

Exception safety guarantees:
----------------------------
- rely on destructor to clean up 

Rule of 3: (as long as have one of these, need all 3) 
----------
- Needs Destructor to prevent memory leaks
- Needs copy constructor to prevent from stealing resources  
- Needs copy assignment to prevent from stealing resources


Rule of 5:
----------
- Needs to implement 

    Classes manages resources:
    
    - Destructor
    - Copy constructor 
    - Copy assignment
    
    Offer move semantics:
    
    - move constuctor 
    - move assignment
    
Rule of 0:
----------
- Dont implement anything 

class IMyClass 
{
public: 
    virtual ~IMyClass() = default;
};

Resource Acquisition in(is) initialization
------------------------------------------
//Wrong implementation 
{
    time start = getCurrentTime();
    
    /*
    Codes....
    */
    
    timeEnd = getCurrentTime();
    log(end - start);
}

-------------------------------------
//Correct implementation
{
    Timer t{};
    
    /*
    Codes...
    */
}

class Timer
{
  time start;

public: 
    Timer:start[getCurrentTime()]{}
    
    Timer(const timer&) = default; 
    Timer& operator = (const Timer&) = default;
    
   ~Timer()
   {
       TIME end = getCurrentTime();
       log(end - start);
   }
};

Naive examples:
---------------
- std::string 
- std::vector 

Basic examples:
---------------
- smart pointers

Advanced examples:
------------------
- lock guards / multithreaded


std::lock guard 

----------------------------------------------------------
#include <iostream> 

void* operator new(size_t size)
{
    void* ptr = malloc(size);
    
    std::cout << "new(" << size << ")->" << ptr << std::endl;
    
    if(!ptr)
    {
        throw std::bad_alloc{};
    }
    
    return ptr;
}

void operator delete(void*ptr)
{
        std::cout << "delete(" << ptr << ")" << std::endl;

    free(ptr);
}

void operator delete(void* ptr, size_t size)
{
    std::cout << "delete(" << ptr << ", " << size << ")" << std::endl;
    free(ptr);
}

int main()
{
    int* p = new int;
    
    *p = 10;
    
    delete p;
}

------------------------------------------------
#include <iostream> 

class MyClass
{
public:
    
    void* operator new[](size_t size)
    {
        void* ptr = ::
    }
    

    void* operator new(size_t size)
{
    void* ptr = ::malloc(
    
    std::cout << "new(" << size << ")->" << ptr << std::endl;
    
    if(!ptr)
    {
        throw std::bad_alloc{};
    }
    
    return ptr;
}

void operator delete(void*ptr)
{
        std::cout << "delete(" << ptr << ")" << std::endl;

    free(ptr);
}

void operator delete(void* ptr, size_t size)
{
    std::cout << "delete(" << ptr << ", " << size << ")" << std::endl;
    free(ptr);
}
};

void* operator new(size_t size)
{
    void* ptr = malloc(size);
    
    std::cout << "new(" << size << ")->" << ptr << std::endl;
    
    if(!ptr)
    {
        throw std::bad_alloc{};
    }
    
    return ptr;
}

void operator delete(void*ptr)
{
        std::cout << "delete(" << ptr << ")" << std::endl;

    free(ptr);
}

void operator delete(void* ptr, size_t size)
{
    std::cout << "delete(" << ptr << ", " << size << ")" << std::endl;
    free(ptr);
}

int main()
{
    int* p = new int[10];
    *p = 10;
    delete p;
    
    MyClass* obj = new MyClass;
    delete obj;
}

================================================================================
Week 10 lecture 1:

Expression new:
--------------
T* p = new T;

- Calculate the size 
- Call operator new 
    - void* operator new (size_t s);

- Call T's constructor (if any)


Expression delete 
-----------------
delete p;

- Call T's destructor (if any)
- Call operator delete 
    - void* operator delete(void* ptr);
    - void* operator delete(void* size_t s);

Operator new/delete can be overloaded

- non array / array version 
- globally / in specific class 

--------------------------------------------------------------------------------
#include <iostream>
#include <new> 
#include <exception>

class MyClass
{
public:
	MyClass() { std::cout << "MyClass::MyClass()\n"; }
	~MyClass() { std::cout << "MyClass::~MyClass()\n"; }

};

void* operator new(size_t size)		//static by default
{
	void* ptr = malloc(size);
	std::cout << "Allocation; " << size << ", " << ptr << "\n";

	if (!ptr)
	{
		throw std::bad_alloc();
	}

	return ptr;
}

void operator delete(void* ptr)		//static by default
{
	std::cout << "Deallocation; " << ptr << "\n" << std::endl;
	free(ptr);
}

void operator delete(void* ptr, size_t size)		//static by default
{
	std::cout << "Deallocation; " << size << ", " << ptr << "\n" << std::endl;
	free(ptr);
}

int main()
{
	int* a = new int;
	(void)a;

	delete a;

	//-------------------------

	int* p = new int[10];
	(void)p;

	delete[] p;

	//-------------------------

	MyClass* q = new MyClass[10];
	(void)q;

	delete[] q;

	//-------------------------

	MyClass* r = new MyClass;
	(void)r;

	delete r;

	//--------------------------

}

--------------------------------------------------------------------------------
#include <iostream>
#include <new> 
#include <exception>

class MyClass
{
public:
	MyClass() { std::cout << "MyClass::MyClass()\n"; }
	~MyClass() { std::cout << "MyClass::~MyClass()\n"; }

	static void* operator new(size_t size, bool canThrow)
	{
		void* ptr = malloc(size);
		std::cout << "Allocation; " << "CanThrow? " << canThrow << ", " << size << ", " << ptr << "\n";

		if (!ptr)
		{
			throw std::bad_alloc();
		}

		return ptr;
	}

	static void* operator new(size_t size)		//static by default
	{
		void* ptr = malloc(size);
		std::cout << "Allocation; " << size << ", " << ptr << "\n";

		if (!ptr)
		{
			throw std::bad_alloc();
		}

		return ptr;
	}

	static void operator delete(void* ptr)		//static by default
	{
		std::cout << "Deallocation; " << ptr << "\n" << std::endl;
		free(ptr);
	}

	static void operator delete(void* ptr, size_t size)		//static by default
	{
		std::cout << "Deallocation; " << size << ", " << ptr << "\n" << std::endl;
		free(ptr);
	}


};

int main()
{
	MyClass* s = new(false) MyClass;
	(void)s;
	delete s;

	//---------------------------

	MyClass* t = new(true) MyClass;
	(void)t;
	delete t;

	//----------------------------

	char buffer;

	MyClass* u = new(&buffer) MyClass;   
	(void)u;
	delete u;

	u->~MyClass();
}
--------------------------------------------------------------------------------
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

--------------------------------------------------------------------------------
Templates: 
----------
- Generic Programming
- Template specialization 
- using (data types, class templates, templated variables)

universal references: 
---------------------
- auto 
- decltype 
- contexpr 

keyword: auto
--------------
//Represents a data type with explicit indication of references 

int x = 20;
auto i = 10;        //removes, ref, removes const; acts as assignment by copy
auto& i = x;        //Does not remove ref and const; acts as binding to lvalue ref 
auto&& i = x;       //Does not remove ref and const; acts as binding to uni ref 

auto is like calling function void f(T&&)     //passing in int& i = x

keyword: decltype   
-----------------
//Represents a data type preserving its references 

decltype(obj)
- if object is:
    - a variable 
    - a function parameter 
    - a data member 
    
  returns exactly this type; 
  
  e.g. 
  
  class MyClass
  {
    int& d;  
  };
  
  const MyClass o;
  decltype(o.d) x = 10; //return int& x = 10; cannot bind lvalue to Rvalue 



  
decltype(expr)      
- returns a data type returned by this expression, preserving references 
    - returns lvalue: we get lvalue reference;  int&  
    - returns Rvalue: we get Rvalue reference;  int&&
    - returns PR value: we get no reference;    int

    decltype((o.d)) x = 10;     //const int& = const lvalue; can compile 
                                //can bind const lvalue to Rvalue 
                                
                                
decltype(auto)
    - decltype(auto) x = initializer;   //is like writing 
    - decltype(initializer) x = initializer;
    

    std::move // can take in lvalue or Rvalue, but always results in Rvalue 

#include <iostream>

template <typename T>
decltype(auto) move(T&& t)
{
	return static_cast<std::remove_reference_t<T>&&>(t);
}

template<typename T>
void f()
{
	T t;
	auto x = t.f();
}

int main()
{
	return 0;
}

================================================================================

unsigned short clear14(unsigned short val)
{
    return static_cast<unsigned short> (value & ~(1<<14));
}


0x413E

0100 0001 0011 1110
1011 1111 1111 1111

0000 0001 0011 1110

0x13E

--------------------------------------------------------------------------------

unsigned char toggleN(unsigned char val, unsigned char pos)
{
    return static_cast<unsigned char> (value ^ ( 1 << pos));
}

045     6

00100101
00000001

001 100 101
0145

value | mask  setting 
value &~ mask clearing 
value ^ mask negating  

--------------------------------------------------------------------------------
Week 10 lecture 2
-----------------

auto l = r;
auto& l = r;
auto&& l = r;

decltype(obj)
decltype(expr) // if expr is lvalue, give u lvalue 
               // if expr is Rvalue, give u Rvalue 
               // if expr is PRvalue, give u PRvalue 
               // decltype give u best r value, that can give u 

contexpr var     //constant expression 
------------
#include <iostream>

int main()
{
    //int i;
    //std::cin >> i;

    ////x is a compile-time const 
    ////x is a run-time const (a read-only object)
    //const int x = 10;          
    //(void)x;

    // x is a run-time const ( a read-only object)
    //constexpr is a strong const than 
    constexpr int x = 10;
    (void)x;

    int arr[x];                
    (void)arr;
}

--------------------------------------------------------------------------------
contexpr f(2)     //constant expression 
------------
#include <iostream>

//if give e as 2, will return compile time const 4
//if give e as i, will not return compile time i
constexpr int f(int e)
{
    return 1 << e;
}

int main()
{
    //int i;
    //std::cin >> i;

    //x is a compile-time const 
    //x is a run-time const (a read-only object)
    //const int x = 10;          
    //(void)x;

    // x is a run-time const ( a read-only object)
    //constexpr is a stronger than const
    constexpr int x = f(2);
    std::cout << x << std::endl;
    (void)x;

    int arr[x];                
    (void)arr;
}

--------------------------------------------------------------------------------
//variadic templates 

//A parameter pack 
//- one identifier that represents 0 or more elements 
//- a pack has to be unpacked to individual elements 
//- packs are available in templates only 

//Two types of packs:
/*
    - Template parameter pack
    template<typename... Ts> 

    template <size_t...Ns>
    
    Template parameer pack can represent 0 or more types or compile time constants. 
    
    Packs can be unpacked in:
    - in a list of class' base templates 
    - when passing template parameters for another template 
    - in a list of function templates parameters to create a function parameter pack 
    
    - Function parameter pack 
    
    template <typename... Ts>
    void T(Ts... args);
    
    Function parameter pack can represent 0 or more variables of parameters with their values 
    
*/

--------------------------------------------------------------------------------
#include <iostream>
/*
//run time
template<size_t n, size_t m>
size_t sum()
{
	return n + m;	//30
}
*/

template<size_t N, size_t M>
constexpr auto sum()
{
	return N + M;			//30
}

int main()
{
	std::cout << sum<10, 20>();

}

--------------------------------------------------------------------------------
#include <iostream>

template<size_t First, size_t... Rest>
constexpr auto sum()
{
	if constexpr (sizeof...(Rest) == 0)
	{
		return First;
	}
	else
	{
		return First + sum(Rest...);
	}
}

int main()
{
	std::cout << sum<10, 20, 30, 40, 50, 60>();
}

--------------------------------------------------------------------------------
#include <iostream>

template<typename T, typename...Ts>
constexpr auto sum(T first, Ts... rest)
{
	if constexpr (sizeof...(rest) == 0)
	{
		return first;
	}
	else
	{
		return first + sum(rest...);
	}
}

int main()
{
	std::cout << sum(10, 20, 30, 40, 50, 60);   //210
}

--------------------------------------------------------------------------------
#include <iostream>

// Ts..... means T0, T1, T2, T3 
//sum(T first, Ts...rest) = sum(10,20,30) = sum(int first, int rest 20, int rest 30)

template<typename T, typename...Ts>
constexpr auto sum(T first, Ts... rest)
{
	if constexpr (sizeof...(rest) == 0)
	{
		return first;
	}
	else
	{
		return first + sum(rest...);
	}
}

int main()
{
	std::cout << sum(10, 20L, static_cast<short>(30));  //60, auto returns the type 
}

--------------------------------------------------------------------------------
//Generating sequences - next thurs quiz 

/*
counter | pack 
10		| ...

decrease counter | decrease counter 
9		|              9
8		|			8, 9
7		|		 7, 8, 9
6		|	  6, 7, 8, 9
1		| 1,2,3,4,5,6,7,8,9
0		| 0,1,2,3,4,5,6,7,8,9

*/
#include <iostream>

template <size_t...Ns>
struct index_sequence
{
	static void print()
	{
		size_t arr[] = { Ns... };

		for (const auto& i : arr)
		{
			std::cout << i << " ";
		}
	}
};

//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////

int main()
{
	//index_sequence <0,1,2,3,4,5,6,7,8,9>
	make_sequence<10>::print();				//0 1 2 3 4 5 6 7 8 9
}
--------------------------------------------------------------------------------
#include <iostream>

template <size_t...Ns>
struct index_sequence
{
	static void print()
	{
		size_t arr[] = { Ns... };

		for (const auto& i : arr)
		{
			std::cout << i << " ";
		}
	}
};

//////////////////////////////////////////////////////////////////////////

template <size_t Counter, size_t... Rest>
struct make_sequence_impl
{
	using type = typename make_sequence_impl<Counter - 1, (Counter - 1)%2, Rest...>::type;
};

template<size_t...Rest>
struct make_sequence_impl<0, Rest...>
{
	using type = index_sequence<Rest...>;
};

template<size_t N>
using make_sequence = typename make_sequence_impl<N>::type;

//////////////////////////////////////////////////////////////////////////

int main()
{
	make_sequence<10>::print();				//0 1 0 1 0 1 0 1 0 1
}

--------------------------------------------------------------------------------
//Generating sequences - next thurs quiz 

/*
counter | pack 
10		| ...

decrease counter | decrease counter 
9		|              9
8		|			8, 9
7		|		 7, 8, 9
6		|	  6, 7, 8, 9
1		| 1,2,3,4,5,6,7,8,9
0		| 0,1,2,3,4,5,6,7,8,9

*/
#include <iostream>

template <size_t...Ns>
struct index_sequence
{
	static void print()
	{
		size_t arr[] = { Ns... };

		for (const auto& i : arr)
		{
			std::cout << i << " ";
		}
	}
};

//////////////////////////////////////////////////////////////////////////

template <size_t Counter, size_t... Rest>
struct make_sequence_impl
{
	using type = typename make_sequence_impl<Counter - 1, Counter - 1, Rest...>::type;
};

template<size_t...Rest>
struct make_sequence_impl<0, Rest...>
{
	//... in unpacking follows a pattern
	//- a pattern can be a single pack 
	//- a pattern can be an expression on a pack 
	//			(Rest % 2)...
	//			Rest0 % 2, Rest1 % 2, Rest2 % 2, Rest3 % 2
	//- a pattern can include multiple packs, but they have to be of the same size (cardinality)
	//			(P + Q)...
	//			(P0 + Q0), (P1 + Q1), (P2 + Q2)

	using type = index_sequence<(Rest % 2)...>;
};

template<size_t N>
using make_sequence = typename make_sequence_impl<N>::type;

//////////////////////////////////////////////////////////////////////////

int main()
{
	//index_sequence <0,1,2,3,4,5,6,7,8,9>
	make_sequence<10>::print();				//0 1 0 1 0 1 0 1 0 1
}

--------------------------------------------------------------------------------
#include <iostream>

template <size_t...Ns>
struct index_sequence
{
	static void print()
	{
		size_t arr[] = { Ns... };

		for (const auto& i : arr)
		{
			std::cout << i << " ";
		}
	}
};

template <size_t Counter, size_t... Rest>
auto make_sequence_impl()
{
	if constexpr (Counter == 0)
	{
		return index_sequence<Rest...>{};
	}
	else
	{
		return make_sequence_impl <Counter -1, Counter -1, Rest...>();
	}
}

template<size_t N>
using make_sequence = decltype(make_sequence_impl<N>());

int main()
{
	//index_sequence <0,1,2,3,4,5,6,7,8,9>
	make_sequence<10>::print();				//0 1 2 3 4 5 6 7 8 9
}

--------------------------------------------------------------------------------
#include <iostream>

template <size_t...Ns>
struct index_sequence
{
	static void print()
	{
		size_t arr[] = { Ns... };

		for (const auto& i : arr)
		{
			std::cout << i << " ";
		}
	}
};

template <size_t Counter, size_t... Rest>
auto make_sequence_impl()
{
	if constexpr (Counter == 0)
	{
		return index_sequence<Rest...>{};
	}
	else
	{
		return make_sequence_impl <Counter -1, (Counter -1)%2, Rest...>();
	}
}

template<size_t N>
using make_sequence = decltype(make_sequence_impl<N>());

int main()
{
	//index_sequence <0,1,2,3,4,5,6,7,8,9>
	make_sequence<10>::print();				//0 1 0 1 0 1 0 1 0 1
}

--------------------------------------------------------------------------------
#include <iostream>

template <size_t...Ns>
struct index_sequence
{
	static void print()
	{
		size_t arr[] = { Ns... };

		for (const auto& i : arr)
		{
			std::cout << i << " ";
		}
	}
};

template <size_t Counter, size_t... Rest>
auto make_sequence_impl()
{
	if constexpr (Counter == 0)
	{
		return index_sequence<Rest...>{};
	}
	else
	{
		return make_sequence_impl <Counter -1, 1, (Rest*10)...>();
	}
}

template<size_t N>
using make_sequence = decltype(make_sequence_impl<N>());

int main()
{
	//index_sequence <0,1,2,3,4>
	make_sequence<5>::print();				//1 10 100 1000 10000
}

--------------------------------------------------------------------------------
Week 11 Lecture 1:
------------------
//Trailing return types
//Callable objects, std::function 
#include <iostream>

int f(int a, int b)
{
	return a + b;
}

int main()
{
	std::cout << f(10, 20); //30
}


--------------------------------------------------------------------------------
double f(float a, double b)
{
	return a + b;
}

int main()
{
	std::cout << f(10.0f, 20.0);    //30
}

--------------------------------------------------------------------------------
#include <iostream>

template < typename T1, typename T2>
double f(T1 a, T2 b)
{
	return a + b;
}

int main()
{
	std::cout << f(10.0f, 20.0);    //30
}

--------------------------------------------------------------------------------
#include <iostream>

template < typename T1, typename T2>
auto f(T1 a, T2 b)                      //by copy 
{
	return a + b;
}

int main()
{
	std::cout << f(10.0f, 20.0);    //30
}

--------------------------------------------------------------------------------
#include <iostream>

template < typename T1, typename T2>
decltype(a = b) f(T1 a, T2 b)		// return data type of A, not compilable
{
	return a + b;
}

int main()
{
	std::cout << f(10.0f, 20.0);
}
    
--------------------------------------------------------------------------------
#include <iostream>

template < typename T1, typename T2>
auto f(T1 a, T2 b) -> decltype(a + b)	// return data type of A
{
	return a + b;
}

int main()
{
	std::cout << f(10.0f, 20.0);        //30
}

--------------------------------------------------------------------------------
/*
A callable object is an object of a type that can be called like a function. 
- Function 
- Function pointer 
- Member function 
- Member function pointers 
- Functor; a class with an overloaded operator() like std::function 
- Lambda expressions with a capture 
- Lambda expressions without a capture clause 
*/
#include <iostream>

int sum = 0;

void add(int x)
{
	sum += x;
}

int main()
{
	std::cout << "\nFunction:" << std::endl;
	{
		add(1);
		add(2);
		std::cout << sum << std::endl;
	}
}


-----
#include <iostream>
#include <functional>	// std::function

int sum = 0;

void add(int x)
{
	sum += x;
}

int main()
{
	std::cout << "\nFunction:" << std::endl;
	{
		std::function< void(int) > myadd = add;

		myadd(1);
		myadd(2);
		std::cout << sum << std::endl;
	}
}

------------------------
#include <iostream>
#include <functional>	// std::function

int sum = 0;

void add(int x)
{
	sum += x;
}

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
		using F = void(*)(int);
		F myadd = &add;

		myadd(10);
		myadd(20);

		std::cout << sum << std::endl;
	}
}

-----------------------------
#include <iostream>
#include <functional>	// std::function

int sum = 0;

void add(int x)
{
	sum += x;
}

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
}

---------------------------------
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
		Adder myadd;
		myadd(100);
		myadd(200);

		std::cout << sum << std::endl;
	}
}

---------------------------------------------------------
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
}
-------------------------------------------------------
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
	void operator()(int x)
	{
		_sum += x;
	}

	int _sum = 0;
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
		Adder2 adder;

		adder(1000);
		adder(2000);

		std::cout << adder._sum << std::endl;

	}
}

------------------------------------------------------------
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
	void operator()(int x)
	{
		_sum += x;
	}

	int _sum = 0;
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
		Adder2 adder;
		std::function<void(int) > myadd = adder;

		myadd(1000);
		myadd(2000);

		std::cout << adder._sum << std::endl;

	}
}

------------------------------------------------------------
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

	int& _sum;
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
}

----------------------------------------------------------------
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
		auto lambda = [&sum](int x) {sum += x;};	//capture sum by reference

		lambda(10000);
		lambda(20000);

		std::cout << sum << std::endl;
	}
}

--------------------------------------------------
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


}

--------------------------------------------------------------------------------
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
		auto lambda = [](int x) {sum += x;};

		lambda(1);
		lambda(2);

		std::cout << sum << std::endl;
	}
}

-------------------------------------------------------
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
		using F = void(*)(int);
		F lambda = [](int x) {sum += x;};

		lambda(1);
		lambda(2);

		std::cout << sum << std::endl;
	}



}

---------------------------
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




}

--------------------------------------------------------
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

	std::cout << "\nMember function" << std::endl;
	{
		using F = void(Adder3::*)(int);
		F f = &Adder3::add;
		
		Adder3 adder;
		(adder.*f)(10);
		(adder.*f)(20);

		std::cout << sum << std::endl;
	}




}
---------------------------------------------
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

	std::cout << "\nMember function" << std::endl;
	{
		auto myadd = std::mem_fn(&Adder3::add);

		Adder3 adder;

		myadd(adder, 10);
		myadd(adder, 20);

		std::cout << sum << std::endl;
	}




}

--------------------------------------------------------------
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

	std::cout << "\nMember function" << std::endl;
	{
		std::function<void(Adder3&, int) > myadd = std::mem_fn(&Adder3::add);

		Adder3 adder;

		myadd(adder, 10);
		myadd(adder, 20);

		std::cout << sum << std::endl;
	}
}

-----------------------------------------
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
		auto lambda = [](auto x) {sum += x; };

		/*
		template<typename T>
		void XXX(T)
		{
			sum+= x;
		}

		*/

		lambda(1);
		lambda(2);

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

--------------------------------------------------------------------------------
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
----------------------------------------------------------------------------------
#include <iostream>
#include <functional>	// std::function
#include <memory>		//smart pointer 


namespace cs225
{
	template<typename TResult, typename... TArgs>
	class function
	{
		struct IConcept
		{
			virtual TResult operator()(TArgs... args) = 0;	//not accepting TArgs&&
			virtual ~IConcept() = default;
		};

		template <typename F>
		class Model : public IConcept
		{
			F _f;

		public:
			Model(F f) : _f{ f }
			{

			}

			TResult operator()(TArgs... args)override
			{
				return _f(std::forward<TArgs>(args)...);
			}
		};

		std::shared_ptr<IConcept> _concept;

	public:
		template<typename F>
		function(F f) : _concept{ std::make_shared<Model<F>>(f) }
		{

		}

		TResult operator()(TArgs... args)
		{
			return _concept->operator()(std::forward<TArgs>(args)...);
		}
	};
}

int main()
{
	cs225::function<void, int> f = [](int x) {std::cout << x << std::endl;};
	f(123);
}

--------------------------------------------------------------------------------
#include <iostream>
#include <functional>	// std::function
#include <memory>		//smart pointer 


namespace cs225
{
	template<typename TResult, typename... TArgs>
	class function;

	template<typename TResult, typename... TArgs>
	class function<TResult(TArgs...)>
	{
		struct IConcept
		{
			virtual TResult operator()(TArgs... args) = 0;	//not accepting TArgs&&
			virtual ~IConcept() = default;
		};

		template <typename F>
		class Model : public IConcept
		{
			F _f;

		public:
			Model(F f) : _f{ f }
			{

			}

			TResult operator()(TArgs... args)override
			{
				return _f(std::forward<TArgs>(args)...);
			}
		};

		std::shared_ptr<IConcept> _concept;

	public:
		template<typename F>
		function(F f) : _concept{ std::make_shared<Model<F>>(f) }
		{

		}

		TResult operator()(TArgs... args)
		{
			return _concept->operator()(std::forward<TArgs>(args)...);
		}
	};
}

int main()
{
	cs225::function<void(int)> f = [](int x) {std::cout << x << std::endl;};
	f(123);
}

--------------------------------------------------------------------------------
#include <iostream>
#include <functional>	// std::function
#include <memory>		//smart pointer 
#include <vector>
#include <algorithm>

namespace cs225
{
	template<typename TResult, typename... TArgs>
	class function;

	template<typename TResult, typename... TArgs>
	class function<TResult(TArgs...)>
	{
		struct IConcept
		{
			virtual TResult operator()(TArgs... args) = 0;	//not accepting TArgs&&
			virtual ~IConcept() = default;
		};

		template <typename F>
		class Model : public IConcept
		{
			F _f;

		public:
			Model(F f) : _f{ f }
			{

			}

			TResult operator()(TArgs... args)override
			{
				return _f(std::forward<TArgs>(args)...);
			}
		};

		std::shared_ptr<IConcept> _concept;

	public:
		template<typename F>
		function(F f) : _concept{ std::make_shared<Model<F>>(f) }
		{

		}

		TResult operator()(TArgs... args)
		{
			return _concept->operator()(std::forward<TArgs>(args)...);
		}
	};
}

int main()
{
	cs225::function<void(int)> f = [](int x) {std::cout << x << std::endl;};
	f(123);

	std::vector<int> v = { 10, 20, 0, 30, 40 };
	std::cout << std::count_if(v.begin(), v.end(), [](int i) {return i > 20;});
}

--------------------------------------------------------------------------------
#include <iostream>
#include <functional>	// std::function
#include <memory>		//smart pointer 
#include <vector>
#include <algorithm>

namespace cs225
{
	template<typename TResult, typename... TArgs>
	class function;

	template<typename TResult, typename... TArgs>
	class function<TResult(TArgs...)>
	{
		struct IConcept
		{
			virtual TResult operator()(TArgs... args) = 0;	//not accepting TArgs&&
			virtual ~IConcept() = default;
		};

		template <typename F>
		class Model : public IConcept
		{
			F _f;

		public:
			Model(F f) : _f{ f }
			{

			}

			TResult operator()(TArgs... args)override
			{
				return _f(std::forward<TArgs>(args)...);
			}
		};

		std::shared_ptr<IConcept> _concept;

	public:
		template<typename F>
		function(F f) : _concept{ std::make_shared<Model<F>>(f) }
		{

		}

		TResult operator()(TArgs... args)
		{
			return _concept->operator()(std::forward<TArgs>(args)...);
		}
	};
}

bool gt20(int i)
{
	return i > 20;
}

int main()
{
	cs225::function<void(int)> f = [](int x) {std::cout << x << std::endl;};
	f(123);

	std::vector<int> v = { 10, 20, 0, 30, 40 };
	std::cout << std::count_if(v.begin(), v.end(), gt20);
}

----------------------------
#include <iostream>
#include <functional>	// std::function
#include <memory>		//smart pointer 
#include <vector>
#include <algorithm>

namespace cs225
{
	template<typename TResult, typename... TArgs>
	class function;

	template<typename TResult, typename... TArgs>
	class function<TResult(TArgs...)>
	{
		struct IConcept
		{
			virtual TResult operator()(TArgs... args) = 0;	//not accepting TArgs&&
			virtual ~IConcept() = default;
		};

		template <typename F>
		class Model : public IConcept
		{
			F _f;

		public:
			Model(F f) : _f{ f }
			{

			}

			TResult operator()(TArgs... args)override
			{
				return _f(std::forward<TArgs>(args)...);
			}
		};

		std::shared_ptr<IConcept> _concept;

	public:
		template<typename F>
		function(F f) : _concept{ std::make_shared<Model<F>>(f) }
		{

		}

		TResult operator()(TArgs... args)
		{
			return _concept->operator()(std::forward<TArgs>(args)...);
		}
	};
}

bool gt20(int i)
{
	return i > 20;
}

void print_count()
{

	std::vector<int> v = { 10, 20, 0, 30, 40 };
	std::cout << std::count_if(v.begin(), v.end(), gt20);
}

int main()
{
	cs225::function<void(int)> f = [](int x) {std::cout << x << std::endl;};
	f(123);

	print_count();
}

--------------------------------------------------------------------------------
#include <iostream>
#include <functional>	// std::function
#include <memory>		//smart pointer 
#include <vector>
#include <algorithm>

namespace cs225
{
	template<typename TResult, typename... TArgs>
	class function;

	template<typename TResult, typename... TArgs>
	class function<TResult(TArgs...)>
	{
		struct IConcept
		{
			virtual TResult operator()(TArgs... args) = 0;	//not accepting TArgs&&
			virtual ~IConcept() = default;
		};

		template <typename F>
		class Model : public IConcept
		{
			F _f;

		public:
			Model(F f) : _f{ f }
			{

			}

			TResult operator()(TArgs... args)override
			{
				return _f(std::forward<TArgs>(args)...);
			}
		};

		std::shared_ptr<IConcept> _concept;

	public:
		template<typename F>
		function(F f) : _concept{ std::make_shared<Model<F>>(f) }
		{

		}

		TResult operator()(TArgs... args)
		{
			return _concept->operator()(std::forward<TArgs>(args)...);
		}
	};
}

bool gt20(int i)
{
	return i > 20;
}

template<typename F>
void print_count(F predicate)
{

	std::vector<int> v = { 10, 20, 0, 30, 40 };
	std::cout << std::count_if(v.begin(), v.end(), predicate);
}

int main()
{
	cs225::function<void(int)> f = [](int x) {std::cout << x << std::endl;};
	f(123);

	print_count(gt20);
}

--------------------------------------------------------------------------------
#include <iostream>
#include <functional>	// std::function
#include <memory>		//smart pointer 
#include <vector>
#include <algorithm>

namespace cs225
{
	template<typename TResult, typename... TArgs>
	class function;

	template<typename TResult, typename... TArgs>
	class function<TResult(TArgs...)>
	{
		struct IConcept
		{
			virtual TResult operator()(TArgs... args) = 0;	//not accepting TArgs&&
			virtual ~IConcept() = default;
		};

		template <typename F>
		class Model : public IConcept
		{
			F _f;

		public:
			Model(F f) : _f{ f }
			{

			}

			TResult operator()(TArgs... args)override
			{
				return _f(std::forward<TArgs>(args)...);
			}
		};

		std::shared_ptr<IConcept> _concept;

	public:
		template<typename F>
		function(F f) : _concept{ std::make_shared<Model<F>>(f) }
		{

		}

		TResult operator()(TArgs... args)
		{
			return _concept->operator()(std::forward<TArgs>(args)...);
		}
	};
}

bool gt20(int i)
{
	return i > 20;
}

void print_count(cs225::function<bool(int)> predicate)
{

	std::vector<int> v = { 10, 20, 0, 30, 40 };
	std::cout << std::count_if(v.begin(), v.end(), predicate);
}

int main()
{
	cs225::function<void(int)> f = [](int x) {std::cout << x << std::endl;};
	f(123);

	print_count(gt20);
}

--------------------------------------------------------------------------------
SFINAE

STL data types:
std::optional
std::any
std::ignore, std::reference_wrapper

tuples
STL data types 

Functional / data processing 
Final test preparation 

--------------------------------------------------------------------------------
//Substitution failure is not an error 

#include <iostream>

template<typename T>
void f(T&& t)
{
	std::cout << t << " (T&&)" << std::endl;
}

int main()
{
	double d = 1.0;
	int i = 2;

	f(d);
	f(i);
}
--------------------------------------------------------------------------------
#include <iostream>

template<typename T>
void f(T&& t){std::cout << t << "\t(T&&)" << std::endl;}

void f(const int& t){std::cout << t << "\t(const int&)" << std::endl;}

int main()
{
	double d = 1.0;
	const int i = 2;

	f(d);   //1 T&&
	f(i);   //2 const int&
}

--------------------------------------------------------------------------------
#include <iostream>

template<typename T>
struct is_not_int
{
	using type = T;
};

template<>
struct is_not_int<int&>
{

};

template<typename T>
using is_not_int_t = typename is_not_int<T>::type;

template<typename T, typename Predicate = is_not_int_t<T>>
void f(T&& t) 
{std::cout << t << "\t(T&&)" << std::endl;}

void f(const int& t)
{
	std::cout << t << "\t(const int&)" << std::endl;
}

int main()
{
	double d = 1.0;
	const int i = 2;

	f(d);
	f(i);
}

--------------------------------------------------------------------------------
#include <iostream>

template<typename T>
struct is_not_int
{
	using type = T;
};

template<>
struct is_not_int<int&>
{

};

template<typename T>
using is_not_int_t = typename is_not_int<T>::type;

template<typename T, typename Predicate = enable_if_t<!is_int_v<std::remove_reference_t<T>>>>
void foo(T&& t) 
{std::cout << t << "\t(T&&)" << std::endl;}

void foo(const int& t)
{
	std::cout << t << "\t(const int&)" << std::endl;
}

template<bool B, typename T = void>
struct enable_if {};

template<typename T>
struct enable_if<true, T>
{
	using type = T;
};

//alias 
template<bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

template<typename T>
struct is_int
{
	static constexpr bool value = false;

};

template<>
struct is_int<int&>
{
	static constexpr bool value = true;
};

template<typename T>
constexpr bool is_int_v = is_int<T>::value;

//-----------
template<typename T, typename Predicate> = enable_if_t<!is_integral_v<std::remove_reference_t<T>>>>
void baz(T&& t)
{
	std::cout << t << "\t(T&&)" << std::endl;
}

void baz(const int& t)
{
	std::cout << t << "\t(const int&)" << std::endl;
}

int main()
{
	double d = 1.0;
	const int i = 2;

	foo(d);
	foo(i);

}
-----------------------
//std::optional
//std::any
//std::ignore
//std::reference_wrapper

#include <iostream>
#include <memory>

auto divide(int x, int y)
{
	if (y)
	{
		return std::make_shared<int>(x / y);
	}
	return std::shared_ptr<int>{};
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


-----------------------------------
#include <iostream>
#include <memory>
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
---------------------------------------
//std::any

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

---------------------------
//std::ignore

#include <iostream>
#include <tuple>

int main()
{
	decltype(std::ignore) i;
	i = 20;

	std::ignore = 10;
}

-------
//std::reference_wrapper

#include <functional>
#include <vector>
#include <iostream>

int main()
{
	int i = 10;
	int j = 20;

	std::vector<int&> v = { i, j };

	for (int& r : v)
	{
		r += 100;
	}

	std::cout << i << ", " << j << std::endl;
}
-------------------------
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

	std::cout << i << ", " << j << std::endl;
}

----------------------
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
ref_wrapper<T> ref_ref(T& t)
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

-------------------
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
----------------------

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
Week 12 Lecture 1:
------------------

TUPLE:
------

template<typename... Ts>
auto f(Ts... t)
{
    return std::tuple<Ts...>(t...);
    //return std::make_tuple(t...);
}

std::tuple<int, int, double> t;

double d = std::get<2>(t);

--------------------------------------------------------------------------------
//A tuple is a data type for objects storing data members of heterogenuous data types.
//Count and types of data members will be specified in the compile-time in the template instantion 
//There is such type in C++ STL:: std::tuple    #include <tuple>

/*
Terms:

- leaf : A data type for storing a single data member of a tuple. 
- tuple: A data type storing a collection of leaves

Two major implementation types:
- Deep inheritance (easy to implement, reverses the order of DM) 
- Flat inheritance (complex implementation, correct order of DM)

typle<int, int, double> t = std::tuple<int,int,double>(A,1,1.0f);
*/

#include <iostream>

namespace deep
{
	template<typename...>
	struct leaf{};

	template<typename TFirst, typename... TRest>
	struct leaf<TFirst, TRest...>:leaf<TRest...>
	{
		TFirst _value;

		//default constructor
		template<typename TValue, typename... TValues>
		explicit leaf(TValue&& value, TValues&&... values) : leaf<TRest...>(std::forward<TValues>(values)...), _value(std::forward<TValue>(value))
		{

		}

		//Copy constructor
		leaf(const leaf& l) : leaf<TRest...>(static_cast<const leaf<TRest...>&>(l)), _value{ l._value }
		{

		}

		//default constructor
		leaf()
		{

		}
	};

	template<typename... Ts>
	struct tuple_impl : leaf<Ts...>
	{
		template<typename... TValues>
		explicit tuple_impl(TValues&&... values) : leaf<Ts...>(std::forward<TValues>(values)...)
		{

		}

		tuple_impl(const tuple_impl& l) : leaf<Ts...>(static_cast<const leaf<Ts...>&>(l))
		{

		}
	};

	template<typename... Ts>
	using tuple = tuple_impl<Ts...>;

	template<typename... Ts>
	auto make_tuple(Ts... t)
	{
		return tuple<Ts...>(t...);
	}

	template<size_t N, typename TFirst, typename... TRest>		//2, int , double
	auto remove_N_type()
	{
		if constexpr (N == 0)
		{
			return leaf<TFirst, TRest...>{};
		}
		else
		{
			return remove_N_type<N - 1, TRest...>();
		}
	}

	template<size_t N, typename... Ts>
	using remove_N_type_t = decltype(remove_N_type<N, Ts...>());

	template<size_t N, typename... Ts>
	auto get(tuple<Ts...> t)
	{
		using leaf = remove_N_type_t<N, Ts...>;
		return static_cast<leaf&>(t)._value;
	}
}

namespace ns = deep;

int main()
{
	auto t = ns::make_tuple(10, 20, 3.0);
	std::cout << sizeof(t) << "\n" << ns::get<2>(t) << std::endl;
}

--------------------------------------------------------------------------------
tuple_imp<int,int,double>

leaf<int>
int _value;

leave <int>
int _value;

leave<double>
double _value;

tuple_imp<index_seq<0,1,2>, int,int,double> 
leaf<0, int>
int _value;

leave <1, int>
int _value;

leave<2, double>
double _value;

--------------------------------------------------------------------------------
#include <iostream>

namespace deep
{
	template<typename...>
	struct leaf{};

	template<typename TFirst, typename... TRest>
	struct leaf<TFirst, TRest...>:leaf<TRest...>
	{
		TFirst _value;

		//default constructor
		template<typename TValue, typename... TValues>
		explicit leaf(TValue&& value, TValues&&... values) : leaf<TRest...>(std::forward<TValues>(values)...), _value(std::forward<TValue>(value))
		{

		}

		//Copy constructor
		leaf(const leaf& l) : leaf<TRest...>(static_cast<const leaf<TRest...>&>(l)), _value{ l._value }
		{

		}

		//default constructor
		leaf()
		{

		}
	};

	template<typename... Ts>
	struct tuple_impl : leaf<Ts...>
	{
		template<typename... TValues>
		explicit tuple_impl(TValues&&... values) : leaf<Ts...>(std::forward<TValues>(values)...)
		{

		}

		tuple_impl(const tuple_impl& l) : leaf<Ts...>(static_cast<const leaf<Ts...>&>(l))
		{

		}
	};

	template<typename... Ts>
	using tuple = tuple_impl<Ts...>;

	template<typename... Ts>
	auto make_tuple(Ts... t)
	{
		return tuple<Ts...>(t...);
	}

	template<size_t N, typename TFirst, typename... TRest>		//2, int , double
	auto remove_N_type()
	{
		if constexpr (N == 0)
		{
			return leaf<TFirst, TRest...>{};
		}
		else
		{
			return remove_N_type<N - 1, TRest...>();
		}
	}

	template<size_t N, typename... Ts>
	using remove_N_type_t = decltype(remove_N_type<N, Ts...>());

	template<size_t N, typename... Ts>
	auto get(tuple<Ts...> t)
	{
		using leaf = remove_N_type_t<N, Ts...>;
		return static_cast<leaf&>(t)._value;
	}

	template<size_t Counter, size_t N, typename TFirst, typename... TRest>
	auto get_Nth_type()
	{
		if constexpr (Counter == 0)
		{
			return leaf<N, TFirst>{};
		}
		else
		{
			return get_Nth_type<Counter - 1, N, TRest...>();
		}
	}

	template<size_t N, typename... Ts>
	using get_Nth_type_t = decltype(get_Nth_type<N, N, Ts...>());

	template<size_t N, typename... Ts>
	auto get(tuple<Ts...> t)
	{
		using leaf = get_Nth_type_t<N, N, Ts...>;
		return static_cast<leaf&>(t)._value;
	}
}

namespace flat
{
	template<size_t N, typename T>
	struct leaf
	{
		T _value;

		//Constructor with some value
		template<typename TValue>
		explicit leaf(TValue&& value) : _value{ std::forward<TValue>(value) }
		{

		}

		//copy constructor
		leaf(const leaf& l) : _value{ l._value }
		{

		}

		//default constructor
		leaf() ::_value{}
		{

		}
	};

	template<size_t... Ns>
	struct index_sequence {};

	template<size_t Counter, size_t... Ns>
	auto index_sequence_imp()
	{
		if constexpr(Counter == 0)
		{
			return index_sequence< Ns... >;
		}
		else
		{
			return index_sequence_imp<Counter - 1, Counter - 1, Ns...>();
		}
	}

	template<size_t N>
	using make_sequence = decltype(index_sequence_imp<N>());

	template<typename Seq, typename... Ts>
	struct tuple_impl {};

	template<size_t... Ns, typename... Ts>
	struct tuple_impl<index_sequence<Ns...>, Ts...>: leaf<Ns, Ts>...
	{
		template<typename... Ts, TValues>
		explicit tuple_impl(TValues&&... values) : leaf<Ns, Ts>(std::forward<TValues>(values))...
		{

		}

		//copy constructor
		tuple_impl(const tuple_impl& l) : leaf<Ns, Ts>(static_cast<const leaf<Ns, Ts>&>(l))...
		{

		}

		//default constructor
		tuple_impl() : leaf<Ns, Ts>{}...
		{

		}
	};

	template<typename... Ts>
	struct tuple : tuple_impl<make_sequence<sizeof...(Ts) >, Ts...>
	{
		template<typename... TValues>
		explicit tuple(TValues&& values) : tuple_impl<make_sequence<sizeof...(Ts)>, Ts...>(std::forward<TValues>(values)...)
		{

		}

		tuple(const tuple& t) : tuple_impl(static_cast<const tuple_impl&>(t))
		{

		}

		tuple() : tuple_impl<make_sequence<sizeof...(Ts)>, Ts...>(std::forward<TValues>(values)...)
		{

		}
	};
}

//namespace ns = flat;
namespace ns = deep;

int main()
{
	auto t = ns::make_tuple(10, 20, 3.0);
	std::cout << sizeof(t) << "\n" 
		<< ns::get<2>(t) 
		<< std::endl;
}

---------------------
leaf<T>
leaf<N, T>
leaf<Ts...>
leaf<N, Ts...>

which one is more suitable for flat tuple implentation. Ans: 2nd 

deep inheritance: 3rd 

drawback of deep implementation: base class go first, implementation will be opposite

================================================================================
Week 12 Lecture 1:
------------------

constructors(default, conversion, copy, move, user-defined, initializer-list)

destructor

overloading of operators (operator=)

std::string 

vector<T> 

STL containers : std::deque, std::copy, std::count

--------------------------------------------------------------------------------
int divide(int x, int y)
{
	return x / y;
}

--------------------------------------------------------------------------------
#include <tuple>
#include <iostream>

//Not the best code , messy 
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

	std::tuple<int, bool> t = divide(10, i);

	if (std::get<1>(t))
	{
		std::cout << std::get<0>(t) << std::endl;
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
	
	return {x / y, true};
}

int main()
{
	int i;
	std::cin >> i;

	std::tuple<int, bool> t = divide(10, i);
	bool success = std::get<1>(t);
	int value = std::get<0>(t);

	if (std::get<1>(t))
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
	
	return {x / y, true};
}

int main()
{
	int i;
	std::cin >> i;

    //structered binding
	auto[value, success] = divide(10, i);
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

--------------------------------------------------------------------------------
#include <tuple>
#include <iostream>

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
}

int main()
{
	int a[] = { 0,1,2,3,4,5,6,7,8,9 };
	const size_t count = sizeof(a) / sizeof(*a);

	int* begin = a;
	int* end = a + count;

	auto print = [](int value)
	{
		std::cout << value << ", ";
	};

	ns::for_each(begin, end, print);
	std::cout << std::endl;
}
--------------------------------------------------------------------------------
#include <tuple>
#include <iostream>
#include <vector>

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
}

int main()
{
	int a[] = { 0,1,2,3,4,5,6,7,8,9 };
	const size_t count = sizeof(a) / sizeof(*a);

	int* begin = a;
	int* end = a + count;

	auto print = [](int value)
	{
		std::cout << value << ", ";
	};

	ns::for_each(begin, end, print);
	std::cout << std::endl;

	std::vector<float> vf = { 1.0f, 2.0f, 3.5f };
	ns::for_each(vf.begin(), vf.end(), print);
}

--------------------------------------------------------------------------------
//std::begin 
//std::end

#include <tuple>
#include <iostream>
#include <vector>

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
}

int main()
{
	int a[] = { 0,1,2,3,4,5,6,7,8,9 };
	const size_t count = sizeof(a) / sizeof(*a);

	int* begin = a;
	int* end = a + count;

	auto print = [](int value)
	{
		std::cout << value << ", ";
	};

	ns::for_each(std::begin(a), std::end(a), print);
	std::cout << std::endl;

	std::vector<float> vf = { 1.0f, 2.0f, 3.5f };
	ns::for_each(std::begin(vf), std::end(vf), print);
}

--------------------------------------------------------------------------------
#include <tuple>
#include <iostream>
#include <vector>

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
}

int main()
{
	auto print = [](int value)
	{
		std::cout << value << ", ";
	};

	std::vector<float> vf = { 1.0f, 2.0f, 3.5f };
	ns::for_each(std::begin(vf), std::end(vf), print);

	int a[] = { 0,1,2,3,4,5,6,7,8,9 };	

	ns::for_each(std::begin(a), std::end(a), print);
	std::cout << std::endl;

	std::cout << ns::count(std::begin(a), std::end(a), 5);
}


--------------------------------------------------------------------------------
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

	int a[] = { 0,5,2,3,4,5,6,7,8,9 };	

	ns::for_each(std::begin(a), std::end(a), print);
	std::cout << std::endl;

	std::cout << ns::count(std::begin(a), std::end(a), 5);
	std::cout << std::endl;

	auto isEven = [](auto x)
	{
		return x % 2 == 0;
	};

	size_t n = ns::count_if(std::begin(a), std::end(a), isEven);
	std::cout << n << std::endl;

	std::vector<int> vi;
	vi.resize(n);

	ns::copy(std::begin(a), std::end(a), std::begin(vi), isEven);
	std::cout << std::endl;

	/*for (size_t i = 0; i < vi.size(); ++i)
	{
		std::cout << vi[i] << ", ";
	}*/

	/*for (auto it = std::begin(vi); it != std::end(vi); ++it)
	{
		auto v = *it;
		std::cout << v << ", ";
	}*/

	for (auto v : vi)
	{
		std::cout << v << ", ";
	}

	std::cout << std::endl;

	/* Sequential

	std::array<int, 4>
	std::vector<int> 
	std::deque<int> 
	std::forward_list<int>
	std::list<int>

	Associative 

	std::set 
	std::multiset

	set::map<std::string, int>
	std::multimap<std::string, int> 

	unordered 

	*/

	std::map<std::string, unsigned int> colors =
	{
		{"red", 0xFF0000 },
		{"green", 0x00FF00}
	};
	colors["blue"] = 0x0000FF;

	for (std::pair<std::string, unsigned int> c : colors)
	{
		std::cout << c.first << ", " << std::hex << c.second << "\n";
	}
	std::cout << std::endl;

	for (auto c : colors)
	{
		std::cout << c.first << ", " << std::hex << c.second << "\n";
	}

	std::cout << std::endl;

	/*for (auto[name, value] : colors)
	{
		std::cout << c.first << ", " << std::hex << c.second << "\n";
	}*/
}

--------------------------------------------------------------------------------
#include <iostream>

template<size_t... Ns>
struct index_seq {};

template<size_t N, size_t M, size_t... Rest>
constexpr auto seq_impl()
{
    if constexpr (N == 0)
    {
        return index_seq<(Rest * M)..., 1>{};
    }
    else
    {
        return seq_impl<N - 1, M, (Rest* M)..., 1>();
    }
}

template<size_t N, size_t M>
using seq = decltype (seq_impl<N +1, M>());

int main()
{
    seq<4, 3> s;
    int i = s;
}

--------------------------------------------------------------------------------
Week 13 Lecture 1:
------------------

Concept of functional programming:
- We care about objects' immutability 

- We treat functions as first class citizens 

- Map Reduce : std::transform , std::accumulate() , std::reduce 

std::transform - transform from data type to structures 
std::accumulate() - combine every 2 structure into one. 
std::reduce - every 2 accumulated structure into 1. 

--------------------------------------------------------------------------------
#include <iostream>
#include <functional> 
#include <algorithm>
#include <numeric>	//reduce
#include <execution>
#include <array>
#include <vector>
#include <deque>
#include <tuple>

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
}

--------------------------------------------------------------------------------
#include <iostream>
#include <functional> 
#include <algorithm>
#include <numeric>	//reduce
#include <execution>
#include <array>
#include <vector>
#include <deque>
#include <tuple>

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
}

--------------------------------------------------------------------------------
#include <iostream>
#include <functional> 
#include <algorithm>
#include <numeric>	//reduce
#include <execution>
#include <array>
#include <vector>
#include <deque>
#include <tuple>

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
}

--------------------------------------------------------------------------------
#include <iostream>
#include <functional> 
#include <algorithm>
#include <numeric>	//reduce
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

	ns::transform(
		std::begin(v), std::end(v),
		std::begin(v4),
		[](int x)
	{
		return power(x, 3);
	}
	);

	ns::for_each(std::begin(v4), std::end(v4), print);
	std::cout << std::endl;
}
--------------------------------------------------------------------------------
#include <iostream>
#include <functional> 
#include <algorithm>
#include <numeric>	//reduce
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

	ns::transform(
		std::begin(v), std::end(v),
		std::begin(v4),
		f
	);

	ns::for_each(std::begin(v4), std::end(v4), print);
	std::cout << std::endl;
}

--------------------------------------------------------------------------------
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

--------------------------------------------------------------------------------
/*
std::begin() / std::end(),
std::size()
std::generate_n()
std::for_each()
std::transform()    //1 //2
std::accumulate()
std::reduce()
std::bind ()
std::back_inserter()
std::insert_iterator
std::next(it, n) / std::prev()
std::apply(f, t);

*/

--------------------------------------------------------------------------------
Finals:

callable objects, STL templates. std::transform, std::bind 

2nd and question 5 is pre midterm content

3 and 4 is post mid term 


Not in exam 




