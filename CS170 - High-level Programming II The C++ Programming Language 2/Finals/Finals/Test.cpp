#include <iostream>

/*
class Test
{
private:
    int d;

public: 
    int a, b;

    Test() :a(1),b(2),d(4){};

    explicit Test(int D)
    {
        this->d = D;
    }

    Test(int A, int B)
    {
        this->a = A;
        this->b = B;
        d = 8;
    }

    ~Test(){}

    int get_d()const {return d;}

    void set_d(int D){this->d = D;}

    friend Test operator+(const Test& lhs, const Test& rhs);
    friend Test operator-(const Test& lhs, const Test& rhs);
    friend Test operator*(const Test& lhs, const Test& rhs);
    friend std::ostream& operator <<(std::ostream& os, const Test& rhs);

    int ToInt(void) const;
    operator int(void) const;

};

Test operator+(const Test& lhs, const Test& rhs)
{
    Test e(lhs.d + rhs.d);
    return e;
}

Test operator-(const Test& lhs, const Test& rhs)
{
    Test f(lhs.d - rhs.d);
    return f;
}

Test operator*(const Test& lhs, const Test& rhs)
{
    Test g(lhs.d * rhs.d);
    return g;
}

std::ostream& operator <<(std::ostream& os, const Test& rhs)
{
    os << rhs.d << std::endl;
    return os;
}

void Test2(const Test& H)
{
    std::cout << "Went through" << std::endl;
}

int Test::ToInt(void) const
{
    return d;
}

Test::operator int(void) const
{
    return d;
}

int main(void)
{
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    Test A;         
    Test B(5, 6);

    std::cout << A.a << "," << A.b <<"," << A.get_d() << std::endl;     //1,2,4
    std::cout << B.a << "," << B.b <<"," << B.get_d() << std::endl;     //5,6,8

    B.set_d(10);

    std::cout << B.a << "," << B.b << "," << B.get_d() << std::endl;    //5,6,10

    Test C(30);                                                         
    Test D(20);

    Test E = operator+(C, D);
    std::cout << E.get_d() << std::endl;                                //50
    std::cout << C.get_d() + D.get_d() << std::endl;                    //50

    Test F = operator-(C, D);
    std::cout << F.get_d() << std::endl;                                //10
    std::cout << C.get_d() - D.get_d() << std::endl;                    //10

    Test G = operator*(C, D);
    std::cout << G.get_d() << std::endl;                                //600
    std::cout << C.get_d() * D.get_d() << std::endl;                    //600

    std::cout << E;
    std::cout << F;
    std::cout << G;

    Test H(100);
    std::cout << H;                                                     //100
    
    Test2(H);                                                           //Went through
    //Test2(100); 
    Test2(Test(100));                                                   //Went through

    int I = H.ToInt();                                                  //convert class to int

    std::cout << "I :" << I << std::endl;                               //I:100

    int J = H;                                                         //implicit conversion
    std::cout << H;                                                    //100

    return 0;
}
*/