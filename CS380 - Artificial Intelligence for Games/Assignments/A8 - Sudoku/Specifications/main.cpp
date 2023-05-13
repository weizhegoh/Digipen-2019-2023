#include <iostream>
#include <sstream>
#include <string>
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


// Non evaluated selftest
void test0()
{
    AI::Backtracking<AI::NextLocation, AI::NextCandidate>{}.run();
}

void test1()
{
    int p[] = { 1,1,1,1,1,0,1,1,1 };
    AI::MapInt1D map{ p, sizeof(p) / sizeof(p[0])};

    int actual = NextLocation_Sudoku1D{ &map }().getIndex();
    int expected = 5;

    std::cout << "Test 1 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test2()
{
    int p[] = { 1,1,1,1,1,1,1,1,1 };
    AI::MapInt1D map{ p, sizeof(p) / sizeof(p[0]) };

    bool actual = NextLocation_Sudoku1D{ &map }().notFound();
    bool expected = true;

    std::cout << "Test 2 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test3()
{
    int p[] = { 0,0,0,0,0,0,6,0,0 };
    AI::MapInt1D map{ p, sizeof(p) / sizeof(p[0]) };

    int actual = NextCandidate_Sudoku1D{ &map }(AI::Location<int>{ map.base, 6 });
    int expected = 7;

    std::cout << "Test 3 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test4()
{
    int p[] = { 0,0,0,0,0,9,0,0,0 };
    AI::MapInt1D map{ p, sizeof(p) / sizeof(p[0]) };

    int actual = NextCandidate_Sudoku1D{ &map }(AI::Location<int>{ map.base, 5 });
    int expected = 0;

    std::cout << "Test 4 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test5()
{
    int p[] = { 0,0,0,0,0,0,0,0,0 };
    AI::MapInt1D map{ p, sizeof(p) / sizeof(p[0]) };

    AI::Backtracking<NextLocation_Sudoku1D, NextCandidate_Sudoku1D>{ &map }.run();

    std::ostringstream os;
    os << map;

    std::string actual = os.str();
    std::string expected = "1,2,3,4,5,6,7,8,9";

    std::cout << "Test 5 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test6()
{
    int p[] = { 1,2,3,4,5,6,7,8,9 };
    AI::MapInt1D map{ p, sizeof(p) / sizeof(p[0]) };

    AI::Backtracking<NextLocation_Sudoku1D, NextCandidate_Sudoku1D>{ &map }.run();

    std::ostringstream os;
    os << map;

    std::string actual = os.str();
    std::string expected = "1,2,3,4,5,6,7,8,9";

    std::cout << "Test 6 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test7()
{
    int p[] = { 0,0,0,1,0,0,2,0,3 };
    AI::MapInt1D map{ p, sizeof(p) / sizeof(p[0]) };

    AI::Backtracking<NextLocation_Sudoku1D, NextCandidate_Sudoku1D>{ &map }.run();

    std::ostringstream os;
    os << map;

    std::string actual = os.str();
    std::string expected = "4,5,6,1,7,8,2,9,3";

    std::cout << "Test 7 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}


void test8()
{
    const int size = 9;
    int p[] = {
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,0,1,1,1,1,   // <--
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1
    };
    AI::MapInt2D map{ p, size, size };

    int actual = NextLocation_Sudoku2D{ &map }().getIndex();
    int expected =  5 * map.width + 4;

    std::cout << "Test 8 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test9()
{
    const int size = 9;
    int p[] = {
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,5,1,1,1,
        1,1,1,1,6,1,1,1,1,
        1,4,1,1,1,3,1,2,1,   // <--
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1
    };
    AI::MapInt2D map{ p, size, size };

    int actual = NextCandidate_Sudoku2D{ &map }(AI::Location<>{ map.base, 4 * map.width + 5 });
    int expected = 7;

    std::cout << "Test 9 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}
    
void test10()
{
    const int size = 9;
    int p[] = {
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0
    };
    AI::MapInt2D map{ p, size, size };

    AI::Backtracking<NextLocation_Sudoku2D, NextCandidate_Sudoku2D>{ &map }.run();

    std::ostringstream os;
    os << map;

    std::string actual = os.str();
    std::string expected = "\
1,2,3,4,5,6,7,8,9,\n\
4,5,6,7,8,9,1,2,3,\n\
7,8,9,1,2,3,4,5,6,\n\
2,1,4,3,6,5,8,9,7,\n\
3,6,5,8,9,7,2,1,4,\n\
8,9,7,2,1,4,3,6,5,\n\
5,3,1,6,4,2,9,7,8,\n\
6,4,2,9,7,8,5,3,1,\n\
9,7,8,5,3,1,6,4,2\n";

    std::cout << "Test 10 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}
