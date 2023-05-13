#include <iostream>

/*
class A
{
    int n;

public:
    A(int n = 1) :n(n) {}

    int getn()const { return n; }

    A operator+(const A& rhs)
    {
        return n + rhs.n;
    } 

    A operator-(const A& rhs)
    {
        return n - rhs.n;
    }

    A operator++()
    {
        ++n;
        return *this;
    }

    A operator++(int)
    {
        n++;
        return *this;
    }

    A operator+=(const A& rhs)
    {
        n += rhs.n;
        return *this;
    }

    A& operator=(const A& a1)
    {
        n = a1.n;
        return *this;
    }
};

int main(void)
{
    A a1(3), a2, a3 = a1 + a2;
    std::cout << "a3 = a1+a2: " << a3.getn() << std::endl;

    a3 = a1 - a2;
    std::cout << "a3 = a1-a2: " << a3.getn() << std::endl;

    a3 += a1;
    std::cout << "a3 += a1: " << a3.getn() << std::endl;

    a3 = ++a1;
    std::cout << "a3 = ++a1: " << a3.getn() << std::endl;

    a3 = a1++;
    std::cout << "a3 = a1++: " << a3.getn() << std::endl;

    A a4 = a1;
    std::cout << "a4 == a1: " << a4.getn() << std::endl;


    return 0;
}
*/