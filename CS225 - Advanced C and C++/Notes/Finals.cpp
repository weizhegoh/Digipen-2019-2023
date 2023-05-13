//Copy and move semantics

#include <iostream>
#include <algorithm>

template<typename T>
class vector
{
    size_t  _count;
    T* _data;
public:

    vector();

    vector(const vector& rhs);
    vector(const vector&& rhs);

    vector& operator=(const vector& rhs);
    vector& operator=(const vector&& rhs)
        ~vector();
};

template<typename T>
vector<T>::vector() :_count{ 0 }, _data{ new T[_count] }
{
    std::cout << "Constructor called" << std::endl;
}

template<typename T>
vector<T>::vector(const vector<T>& rhs) :_count{ rhs._count }, _data{ new T[_count] }
{
    std::copy(rhs._data, rhs._data + rhs._count, _data);
    std::cout << "copy constructor called" << std::endl;
}

template<typename T>
vector<T>::vector(vector<T>&& rhs) :_count{ rhs._count }, _data{ rhs._data }
{
    rhs._count = 0;
    rhs._data = new T[rhs._count];
    std::cout << "move constructor called" << std::endl;
}

template<typename T>
vector<T>& vector<T>::operator=(const vector<T>& rhs)
{
    if (this != &rhs)
    {
        T* t = new T[rhs._count];
        std::copy(rhs._data, rhs._data + rhs._count, t);
        delete[] _data;
        _data = t;
        _count = rhs._count;
    }

    std::cout << "copy assignment called" << std::endl;
    return *this;
}

template<typename T>
vector<T>& vector<T>::operator=(vector<T>&& rhs)
{
    if (this != &rhs)
    {
        std::swap(_count, rhs._count);
        std::swap(_data, rhs._data);
    }

    std::cout << "move assignment called" << std::endl;
    return *this;
}

template<typename T>
vector<T>::~vector()
{
    delete[] _data;
    std::cout << "destructor called" << std::endl;
}

int main()
{
    vector<int> v1;
    vector<int> v2{ v1 };
    vector<int> v3{ std::move(v1) };

    v2 = v1;
    v3 = std::move(v2);
}

//--------------------------------------------------------------------------------
#include <algorithm>

//std::remove_reference
template<typename T>
struct remove_reference { using type = T; };

template<typename T>
struct remove_reference<T&> { using type = T; };

template<typename T>
struct remove_reference<T&&> { using type = T; };

template<typename T>
using remove_reference_t = typename remove_reference<T>::type;

//std::move 
template<typename T>
remove_reference_t<T>&& move(T&& t)
{
    return static_cast<remove_reference<T>&&>(t);
}

//std::forward
template<typename T>
T&& forward(remove_reference_t<T>& t)
{
    return static_cast<T&&>(t);
}

//std::swap
template<typename T>
void swap(T& a, T& b)
{
    T t{ std::move(a) };
    a = std::move(b);
    b = std::move(t);
}

int main()
{
    return 0;
}

//--------------------------------------------------------------------------------
//Trialing return types
//Callable objects, std::functions

#include <functional>

//function 
std::function<void(int)> myAdd = add;

//function pointer 
std::function<void(int)> myAdd = &add;

//functor 
struct Adder
{
    void operator()(int x)
    {
        sum += x;
    }
}

std::function<void(int)> myAdd = Adder{};

//functor with state 
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

std::function<void(int)> myAdd = Adder2{ sum };

//lambda expression with capture 
sum = 0;
std::function<void(int)> myAdd = [&sum](int x) {sum += x;};

//Lamba expression without capture
std::function<void(int)> myAdd = [](int x) {sum += x;};

//Lambda with deduced parameter without capture 
std::function<void(int)> myAdd = [](auto x) {sum += x;};

//member function 
struct Adder3
{
    void add(int x)
    {
        sum += x;
    }
};

std::funcion<void(Adder3&, int)> myAdd = std::mem_fn(&Adder3::add);

Adder3 adder;

myAdd(adder, 10);
myAdd(adder, 20);

//--------------------------------------------------------------------------------
#include <iostream>
#include <functional>

class Division
{
    int _sum;

public:

    int operator()(int x, int y)
    {
        if (y == 0)
            return 0;
        else
            _sum = x / y;

        return _sum;
    }

    int divide(int x, int y)
    {
        if (y == 0)
            return 0;
        else
            _sum = x / y;

        return _sum;
    }
};

int main()
{
    Division d;
    std::function<int(int, int)> myDivide = d;

    std::cout << myDivide(4, 2) << std::endl;   //2

    //----------------------------------
    std::function<int(int, int)> myDivide2 = [](int x, int y)
    {
        if (y == 0)
            return 0;
        else
            return x / y;
    };

    std::cout << myDivide2(10, 2) << std::endl;  //5

    //----------------------------------

    std::function<int(Division&, int, int)> myDivide3 = std::mem_fn(&Division::divide);
    Division d1;

    std::cout << myDivide3(d1, 12, 3) << std::endl;   //4
}

//--------------------------------------------------------------------------------

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
//--------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------
//std::ignore

#include <iostream>
#include <tuple>

int main()
{
    decltype(std::ignore) i;
    i = 20;

    std::ignore = 10;
}

//-------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------
//std::tuple
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

//------------------------------------------------------------------------------
//std::tie 
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

//------------------------------------------------------------------------------
//std::ignore 
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

//----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
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
    auto [t1, t2] = std::tuple<int, int>{ 1, 2 };
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

//-------------------------------------------------------------------------------
//for_each()
template<typename TIter, typename F>
F for_each(TIter begin, const TIter end, F f)
{
    while (begin != end)
    {
        f(*begin++);
    }

    return f;
}

//count()
template <typename TIter, typename TValue>
size_t count(TIter begin, const TIter end, TValue value)
{
    size_t result = 0;
    while (begin != end)
    {
        if (*begin++ == value)
            ++result;
    }

    return result;
}

//count_if()
template<typename TIter, typename F>
size_t count_if(TIter begin, const TIter end, F f)
{
    size_t result = 0;
    while (begin != end)
    {
        if (f(*begin++))
            ++result;
    }

    return result;
}

//copy()
template<typename TInIter, typename TOutIter>
TOutIter copy(TInIter ibegin, TInIter iend, TOutIter obegin)
{
    while (ibegin != iend)
    {
        *obegin++ = *ibegin++;
    }

    return obegin;
}

//copy_if()
template<typename TInIter, typename TOutIter, typename F>
F copy_if(TInIter ibegin, TInIter iend, TOutIter obegin, F f)
{
    while (ibegin != iend)
    {
        if (f(*ibegin))
        {
            *obegin++ = *ibegin;
        }
        ibegin++;
    }

    return f;
}

//begin()
template<typename T, std::size_t N>
auto begin(T(&array)[N])
{
    return array;
}

template<typename T>
auto begin(T& container)
{
    return container.begin();
}

//end()
template<typename T, std::size_t N>
auto end(T(&array)[N])
{
    return array + N;
} 

template<typename T>
auto end(T& container)
{
    return container.end();
}

//-------------------------------------------------------------------------------------
//transform 1
template<typename TInIter, typename TOutIter, typename F>
void transform(TInIter ibegin, TInIter iend, TOutIter obegin, F f)
{
    while (ibegin != iend)
        *obegin++ = f(*ibegin++);
}

//transform 2
template<typename TInIter1, typename TInIter2, typename TOutIter, typename F>
void transform(TInIter1 ibegin1, TInIter1 iend, TInIter2 ibegin2, TOutIter obegin, F f)
{
    while (ibegin1 != iend1)
        *obegin++ = (*ibegin1++, *ibegin2++);
}

//accumulate
template<typename TIter, typename TValue>
auto accumulate(TIter begin, TIter end, TValue value)
{
    while (begin != end)
    {
        value += *begin++;
    }
    return value;
}

//-------------------------------------------------------------------------------------
//Type erasure pattern

class Weapon
{
    struct IConcept
    {
        virtual void attack() = 0;
        virtual ~IConcept() = default;
    };

    template<typename T>
    class Model : IConcept
    {
        T _instance;
    
    public:

        template<typename... Args>
        Model(Args&&... args);

        virtual void attack() override;
    };

    std::unique_ptr<IConcept>_concept;
    Weapon(std::unique_ptr<IConcept> concept);

public:

    template<typename T, typename... Args>
    static Weapon create(Args&&... args);

    void attack();
};

template<typename T> template<typename... Args>
Weapon::Model<T>::Model(Args&&... args) :_instance{ std::forward<Args>(args)... } {}

void Weapon::Model<T>::attack()
{
    _instance.attack();
}

Weapon::Weapon(std::unique_ptr<Weapon::IConcept> concept) : _concept{ std::move(concept) } {}

void Weapon::attack()
{
    _concept->attack();
}

template<typename T, template Args...args>
Weapon Weapon::create(Args&&... args)
{
    return weapon(std::make_unique<Model<T, Args...>>(std::forward<Args>(args)...));
}

//-------------------------------------------------------------------------------------
//Bit Manipulation

value | mask    // set
value & ~mask   // clear
value ^ mask    //negate 

//-------------------------------------------------------------------------------------
//bind
#include <functional>

double divide(double x, double y)
{
    return x / y;
}

struct MyPair 
{
    double a, b;
    
    double multiply()
    {
        return a * b; 
    }
};

using std::placeholders::_1;

auto fn_five = std::bind(divide, 10, 2);  //return 10/2
std::cout << fn_five() << std::endl;        //5

auto fn_half = std::bind(divide, _1, 2);    //return x/2
std::cout << fn_half(10) << std::endl;      //5

auto fn_invert = std::bind(divide, _2, _1); // return y/x;
std::_Ptr_cout << fn_invert(10,2) << std::endl  //return 0.2

auto fn_rounding = std::bind<int>(divide, _1, _2);  //return x/y
std::wcout << fn_rounding(10,3) << std::endl    //3

MyPair ten_two{ 10,2 };

// binding members:
auto bound_member_fn = std::bind(&MyPair::multiply, _1); // returns x.multiply()
std::cout << bound_member_fn(ten_two) << '\n';           // 20

auto bound_member_data = std::bind(&MyPair::a, ten_two); // returns ten_two.a
std::cout << bound_member_data() << '\n';                // 10

//-------------------------------------------------------------------------------------
//Unique Pointer
#include <iostream>
#include <memory>

class myClass
{
public:
    void print() { std::cout << "Hello World!" << std::endl; }
};

//by lvalue ref of an object 
void print1(myClass& obj) { obj.print(); }

//by universal ref of a pointer 
template <typename T>
void print2(T&& obj_ptr) { obj_ptr->print(); }

//by-value of a pointer
void print3(std::unique_ptr<myClass>ptr) { ptr->print(); }

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

//--------------------------------------------------------------------------------
//Shared Pointer 
#include <iostream>
#include <memory>

class myClass
{
public:
    void print() { std::cout << "Hello World!" << std::endl; }
};

//by lvalue ref of an object 
void print1(myClass& obj) { obj.print(); }

//by universal ref of a pointer 
template <typename T>
void print2(T&& obj_ptr) { obj_ptr->print(); }

//by-value of a pointer
void print3(std::shared_ptr<myClass> ptr) { ptr->print(); }

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

//--------------------------------------------------------------------------------
//Weak Pointer
#include <iostream>
#include <memory>

class myClass
{
public:
    void print() { std::cout << "Hello World!" << std::endl; }
};

//by lvalue ref of an object 
void print1(myClass& obj) { obj.print(); }

//by universal ref of a pointer 
template <typename T>
void print2(T&& obj_ptr) { obj_ptr->print(); }

//by-value of a pointer
void print3(std::weak_ptr<myClass> ptr)
{
    if (std::shared_ptr<myClass> sp = ptr.lock())
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

//--------------------------------------------------------------------------------
Weak pointer - lock
------------------ -
#include <iostream>
#include <memory>

class myClass
{
public:
    void print() { std::cout << "Hello World!" << std::endl; }
};

//by lvalue ref of an object 
void print1(myClass& obj) { obj.print(); }

//by universal ref of a pointer 
template <typename T>
void print2(T&& obj_ptr) { obj_ptr->print(); }

//by-value of a pointer
void print3(std::weak_ptr<myClass> ptr)
{
    if (std::shared_ptr<myClass> sp = ptr.lock())
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

    std::weak_ptr<myClass> wp{ sp };
    sp.reset();

    print3(wp);        //lock unsuccessful!
}

//--------------------------------------------------------------------------------