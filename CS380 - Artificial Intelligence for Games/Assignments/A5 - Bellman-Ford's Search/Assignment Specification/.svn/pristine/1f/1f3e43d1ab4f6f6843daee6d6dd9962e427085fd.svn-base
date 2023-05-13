#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <climits>
#include "functions.h"

void test0();
void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
void test8();
void test9();
void test10();

int main(int argc, char* argv[])
{
    void (*f[])() = { test1, test2, test3, test4, test5, test6, test7, test8, test9, test10 };
    const int SIZE = sizeof(f) / sizeof(f[0]);
    int id = -1;

    if (argc == 2)
    {
        if (argv[1][0] == 'i')
        {
            std::cout << "Enter the test number or 0 to run all tests:" << std::endl;
            std::cin >> id;
        }
        else
            id = atoi(argv[1]);
    }
    else
        std::cin >> id;

    test0();

    if (id == 0)
        for (int i = 0; i < SIZE; ++i)
            f[i]();
    else if (0 < id && id <= SIZE)
        f[id - 1]();
    else
        std::cout << "Test " << id << " not found." << std::endl;

    return 0;
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& rhs)
{
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
        os << *it << (it + 1 != rhs.end() ? "," : "");
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::array<int, 3>>& rhs)
{
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
        os << "[" << (*it)[0] << "," << (*it)[1] << "," << (*it)[2] << "]";
    return os;
}


// Non evaluated selftest
void test0()
{

}

void test1()
{
    AI::BellmanFord<0> bellmanFord;

    bellmanFord.run();

    std::ostringstream os;
    os << bellmanFord;

    std::string actual = os.str();
    std::string expected = "[] []";

    std::cout << "Test 1 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test2()
{
    const int inf = AI::inf;

    int matrix[] = {
        0,   inf, inf, inf, inf,
        inf, 0,   inf, inf, inf,
        inf, inf, 0,   inf, inf,
        inf, inf, inf, 0,   inf,
        inf, inf, inf, inf, 0
    };
 
    AI::BellmanFord<5> bellmanFord(matrix);

    bellmanFord.run(0);

    std::ostringstream os;
    os << bellmanFord;

    std::string actual = os.str();
    std::string expected = "[0,inf,inf,inf,inf] [null,null,null,null,null]";

    std::cout << "Test 2 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test3()
{
    const int inf = AI::inf;

    int matrix[] = {
        0,   10,  inf, inf, inf,
        inf, 0,   20,  inf, inf,
        inf, inf, 0,   30,  inf,
        inf, inf, inf, 0,   40,
        inf, inf, inf, inf, 0
    };

    AI::BellmanFord<5> bellmanFord(matrix);

    bellmanFord.run(0);

    std::ostringstream os;
    os << bellmanFord;

    std::string actual = os.str();
    std::string expected = "[0,10,30,60,100] [null,0,1,2,3]";

    std::cout << "Test 3 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test4()
{
    const int inf = AI::inf;

    int matrix[] = {
        0,   10,  inf, inf, inf,
        10 , 0,   20,  inf, inf,
        inf, 20,  0,   30,  inf,
        inf, inf, 30,  0,   40,
        inf, inf, inf, 40,  0
    };

    AI::BellmanFord<5> bellmanFord(matrix);

    bellmanFord.run(2);

    std::ostringstream os;
    os << bellmanFord;

    std::string actual = os.str();
    std::string expected = "[30,20,0,30,70] [1,2,null,2,3]";

    std::cout << "Test 4 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test5()
{
    const int inf = AI::inf;

    int matrix[] = {
        0,   1,   1,   1,   1,
        inf, 0,   1,   1,   1,
        inf, inf, 0,   1,   1,
        inf, inf, inf, 0,   1,
        inf, inf, inf, inf, 0
    };

    AI::BellmanFord<5> bellmanFord(matrix);

    bellmanFord.run(0);

    std::ostringstream os;
    os << bellmanFord;

    std::string actual = os.str();
    std::string expected = "[0,1,1,1,1] [null,0,0,0,0]";

    std::cout << "Test 5 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test6()
{
    const int inf = AI::inf;

    int matrix[] = {
        0,   10,  inf, inf, inf,
        inf, 0,   20,  inf, inf,
        inf, inf, 0,   30,  inf,
        inf, -90, inf, 0,   40,
        inf, inf, inf, inf, 0
    };

    AI::BellmanFord<5> bellmanFord(matrix);

    bool actual = bellmanFord.run(0);
    bool expected = false;

    std::cout << "Test 6 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test7()
{
    const int inf = AI::inf;

    int matrix[] = {
        0,   10,  inf, inf, inf,
        inf, 0,   20,  inf, inf,
        inf, inf, 0,   30,  inf,
        inf, inf, inf, 0,   40,
        inf, inf, inf, inf, 0
    };

    AI::BellmanFord<5> bellmanFord(matrix);
    bellmanFord.run(0);
    std::vector<int> path = bellmanFord.getPath(4);

    std::ostringstream os;
    os << path;

    std::string actual = os.str();
    std::string expected = "1,2,3,4";

    std::cout << "Test 7 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test8()
{
    const int inf = AI::inf;

    int matrix[] = {
        0,   10,  inf, inf, inf,
        inf, 0,   20,  inf, inf,
        inf, inf, 0,   30,  inf,
        inf, inf, inf, 0,   40,
        inf, inf, inf, inf, 0
    };

    AI::BellmanFord<5> bellmanFord(matrix);
    bellmanFord.run(0);
    std::vector<std::array<int, 3>> route = bellmanFord.getRoute(4);

    std::ostringstream os;
    os << route;

    std::string actual = os.str();
    std::string expected = "[0,1,10][1,2,20][2,3,30][3,4,40]";

    std::cout << "Test 8 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}


void test9()
{
    const int inf = AI::inf;

    const int SIZE = 100;

    //    0,   10,  inf, inf, inf, ..., inf
    //    inf, 0,   20,  inf, inf, ..., inf
    //    inf, inf, 0,   30,  inf, ..., inf
    //    inf, inf, inf, 0,   40,  ..., inf
    //    ...
    //    inf, inf, inf, inf, inf, ..., 990
    //    inf, inf, inf, inf, inf, ..., 0

    int matrix[SIZE * SIZE];

    for (int j = 0; j < SIZE; ++j)
        for (int i = 0; i < SIZE; ++i)
            matrix[j * SIZE + i] = inf;

    for (int i = 0; i < SIZE; ++i)
         matrix[i * SIZE + i] = 0;

    for (int i = 0; i < SIZE-1; ++i)
        matrix[i * SIZE + i+1] = (i+1)*10;

    AI::BellmanFord<SIZE> bellmanFord(matrix);
    bellmanFord.run(0);
    std::vector<int> path = bellmanFord.getPath(SIZE - 1);

    std::ostringstream os;
    os << path;

    std::string actual = os.str();
    std::string expected; // = "1,2,3,4,...,99";
    for (int i = 1; i < SIZE; ++i)
        expected += std::to_string(i) + (i<SIZE-1?",":"");

    std::cout << "Test 9 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test10()
{
    const int inf = AI::inf;

    const int SIZE = 100;

    //    0,   10,  inf, inf, inf, ..., inf
    //    inf, 0,   20,  inf, inf, ..., inf
    //    inf, inf, 0,   30,  inf, ..., inf
    //    inf, inf, inf, 0,   40,  ..., inf
    //    ...
    //    inf, inf, inf, inf, inf, ..., 990
    //    inf, inf, inf, inf, inf, ..., 0

    int matrix[SIZE * SIZE];

    for (int j = 0; j < SIZE; ++j)
        for (int i = 0; i < SIZE; ++i)
            matrix[j * SIZE + i] = inf;

    for (int i = 0; i < SIZE; ++i)
        matrix[i * SIZE + i] = 0;

    for (int i = 0; i < SIZE - 1; ++i)
        matrix[i * SIZE + i + 1] = (i + 1) * 10;

    AI::BellmanFord<SIZE> bellmanFord(matrix);
    bellmanFord.run(0);
    std::vector<std::array<int, 3>> route = bellmanFord.getRoute(SIZE - 1);

    std::ostringstream os;
    os << route;

    std::string actual = os.str();
    std::string expected; // = "[0,1,10][1,2,20],...,[98,99,990]";
    for (int i = 0; i < SIZE-1; ++i)
        expected += "[" + std::to_string(i) + "," + std::to_string(i+1) + "," + std::to_string((i+1) * 10) + "]";

    std::cout << "Test 10 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}
