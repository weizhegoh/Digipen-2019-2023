#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
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



std::ostream& operator<<(std::ostream& os, const std::vector<AI::Node*>& v)
{
    for (std::vector<AI::Node*>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        os << **it;
        if (it != v.end() - 1)
            os << ',';
    }
    return os;
}

std::string join(const int* map, int size, char c = ',')
{
    std::string s = "";
    for (int i=0; i<size; ++i)
    {
        s += std::to_string(map[i]);
        if (i != size - 1)
            s += c;
    }
    return s;
}

// Non evaluated selftest
void test0()
{

}

void test1()
{
    AI::GetMapAdjacents getAdjacents;

    std::vector<AI::Node*> a1 = getAdjacents(AI::Key{ 10, 10 });
 
    std::ostringstream os;
    os << a1;

    std::string actual = os.str();
    std::string expected = "";

    std::cout << "Test 1 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;

    for (auto a : a1)
        delete a;
}

void test2()
{
    int map[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0
    };

    AI::GetMapAdjacents getAdjacents{ map, 5 };

    std::vector<AI::Node*> a1 = getAdjacents(AI::Key{ 0, 0 });
    std::vector<AI::Node*> a2 = getAdjacents(AI::Key{ 1, 1 });
    std::vector<AI::Node*> a3 = getAdjacents(AI::Key{ 3, 4 });

    std::ostringstream os;
    os << a1 << "  " << a2  << "  " << a3;
      
    std::string actual = os.str();
    std::string expected = "0,1,1,0  1,0,1,2,0,1,2,1  4,4";

    std::cout << "Test 2 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;

    for (auto a : a1)
        delete a;
    for (auto a : a2)
        delete a;
    for (auto a : a3)
        delete a;
}

void test3()
{
    int map[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0
    };

    AI::GetMapStochasticAdjacents getAdjacents{ map, 5 };

    std::vector<AI::Node*> adjacents = getAdjacents(AI::Key{ 1, 1 });

    std::ostringstream os;
    os << adjacents;
    std::string actual = os.str();

    std::string a0 = std::to_string(adjacents[0]->key.j) + ',' + std::to_string(adjacents[0]->key.i);
    std::string a1 = std::to_string(adjacents[1]->key.j) + ',' + std::to_string(adjacents[1]->key.i);
    std::string a2 = std::to_string(adjacents[2]->key.j) + ',' + std::to_string(adjacents[2]->key.i);
    std::string a3 = std::to_string(adjacents[3]->key.j) + ',' + std::to_string(adjacents[3]->key.i);

    bool actual0 = (a0 == "0,1" || a0 == "1,0" || a0 == "1,2" || a0 == "2,1");
    bool actual1 = (a1 != a0) && (a1 == "0,1" || a1 == "1,0" || a1 == "1,2" || a1 == "2,1");
    bool actual2 = (a2 != a0) && (a2 != a1) && (a2 == "0,1" || a2 == "1,0" || a2 == "1,2" || a2 == "2,1");
    bool actual3 = (a3 != a0) && (a3 != a1) && (a3 != a2) && (a3 == "0,1" || a3 == "1,0" || a3 == "1,2" || a3 == "2,1");

    std::cout << "Test 3 : ";
    if (actual0 && actual1 && actual2 && actual3)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;

    for (auto a : adjacents)
        delete a;
}

void test4()
{
    int actual[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0
    };

    AI::GetMapAdjacents getAdjacents{ actual, 5 };

    AI::Flood_Fill_Recursive(&getAdjacents).run(AI::Key{ 0, 0 }, 2);

    int expected[] = {
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 1,
        2, 2, 2, 1, 0,
        2, 2, 1, 0, 0
    };

    std::cout << "Test 4 : ";
    if (std::equal(actual, actual+25, expected))
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << join(actual, 25) << ')' << std::endl;
}

void test5()
{
    int actual[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0
    };

    AI::GetMapAdjacents getAdjacents{ actual, 5 };

    AI::Flood_Fill_Iterative<AI::Queue>(&getAdjacents).run(AI::Key{ 0, 0 }, 2);

    int expected[] = {
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 1,
        2, 2, 2, 1, 0,
        2, 2, 1, 0, 0
    };

    std::cout << "Test 5 : ";
    if (std::equal(actual, actual + 25, expected))
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << join(actual, 25) << ')' << std::endl;
}

void test6()
{
    int actual[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0
    };

    AI::GetMapAdjacents getAdjacents{ actual, 5 };

    AI::Flood_Fill_Iterative<AI::Stack>(&getAdjacents).run(AI::Key{ 0, 0 }, 2);

    int expected[] = {
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 1,
        2, 2, 2, 1, 0,
        2, 2, 1, 0, 0
    };

    std::cout << "Test 6 : ";
    if (std::equal(actual, actual + 25, expected))
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << join(actual, 25) << ')' << std::endl;
}

void test7()
{
    int actual[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0
    };

    AI::GetMapAdjacents getAdjacents{ actual, 5 };

    AI::Flood_Fill_Iterative<AI::Stack>(&getAdjacents).run(AI::Key{ 4, 4 }, 2);

    int expected[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 2,
        0, 0, 1, 2, 2
    };

    std::cout << "Test 7 : ";
    if (std::equal(actual, actual + 25, expected))
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << join(actual, 25) << ')' << std::endl;
}

void test8()
{
    int actual[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0
    };

    AI::GetMapStochasticAdjacents getAdjacents{ actual, 5 };

    AI::Flood_Fill_Iterative<AI::Stack>(&getAdjacents).run(AI::Key{ 0, 0 }, 2);

    int expected[] = {
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 1,
        2, 2, 2, 1, 0,
        2, 2, 1, 0, 0
    };

    std::cout << "Test 8 : ";
    if (std::equal(actual, actual + 25, expected))
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << join(actual, 25) << ')' << std::endl;
}

void test9()
{
    int actual[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0
    };

    AI::GetMapStochasticAdjacents getAdjacents{ actual, 5 };

    AI::Flood_Fill_Iterative<AI::Stack>(&getAdjacents).run(AI::Key{ 4, 4 }, 2);

    int expected[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 2,
        0, 0, 1, 2, 2
    };

    std::cout << "Test 9 : ";
    if (std::equal(actual, actual + 25, expected))
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << join(actual, 25) << ')' << std::endl;
}

void test10()
{
    int actual[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0
    };

    AI::GetMapStochasticAdjacents getAdjacents{ actual, 5 };

    AI::Flood_Fill_Iterative<AI::Stack>(&getAdjacents).run(AI::Key{ 3, 3 }, 2);

    int expected[] = {
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 1,
        2, 2, 2, 1, 2,
        2, 2, 1, 2, 2
    };

    std::cout << "Test 10 : ";
    if (std::equal(actual, actual + 25, expected))
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << join(actual, 25) << ')' << std::endl;
}
