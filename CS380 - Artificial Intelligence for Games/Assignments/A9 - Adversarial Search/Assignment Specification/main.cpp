#include <iostream>
#include <sstream>
#include <string>
#include <vector>
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

const char x = Grid::x;
const char o = Grid::o;
const char _ = Grid::_;

// Non evaluated selftest
void test0()
{
    
}

void test1()
{
    AI::Move<Grid> move;

    std::ostringstream os;
    os << move;

    std::string actual = os.str();
    std::string expected = "\
[ , , ,\n\
  , , ,\n\
  , , ]\n\
0\n\
0\n\
-1\n";

    std::cout << "Test 1 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test2()
{
    char grid1[] = {
        _,_,_,
        _,_,_,
        _,_,_
    };
    std::vector<int> result1 = Grid{ grid1 }.emptyIndices();

    char grid2[] = {
        _,_,x,
        _,o,_,
        _,_,_
    };
    std::vector<int> result2 = Grid{ grid2 }.emptyIndices();

    char grid3[] = {
        x,x,x,
        x,x,x,
        x,x,x
    };
    std::vector<int> result3 = Grid{ grid3 }.emptyIndices();

    std::ostringstream os;
    os << result1 << ' ' << result2 << ' ' << result3;

    std::string actual = os.str();
    std::string expected = "0,1,2,3,4,5,6,7,8 0,1,3,5,6,7,8 ";

    std::cout << "Test 2 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}


void test3()
{
    char grid1[] = {
        _,_,x,
        _,o,_,
        _,_,_
    };
    bool actual1 = Grid{ grid1 }.winning(x);
    bool expected1 = false;

    char grid2[] = {
        x,x,x,
        _,o,_,
        _,_,_
    };
    bool actual2 = Grid{ grid2 }.winning(x);
    bool expected2 = true;

    std::cout << "Test 3 : ";
    if (actual1 == expected1 && actual2 == expected2)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl 
                                << std::boolalpha 
                                << actual1 << ' ' 
                                << actual2 << ')' << std::endl;
}

void test4()
{
    char grid[] = {
        _,x,_,
        _,x,_,
        _,x,_
    };

    AI::Move<Grid>* move = AI::minimax(Grid{ grid }, x, x, o);

    std::ostringstream os;
    os << *move;

    std::string actual = os.str();
    std::string expected = "\
[ ,x, ,\n\
  ,x, ,\n\
  ,x, ]\n\
10\n\
0\n\
-1\n";

    delete move;

    std::cout << "Test 4 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test5()
{
    char grid[] = {
        _,_,_,
        o,o,o,
        _,_,_
    };

    AI::Move<Grid>* move = AI::minimax(Grid{ grid }, x, x, o);

    std::ostringstream os;
    os << *move;

    std::string actual = os.str();
    std::string expected = "\
[ , , ,\n\
 o,o,o,\n\
  , , ]\n\
-10\n\
0\n\
-1\n";

    delete move;

    std::cout << "Test 5 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test6()
{
    char grid[] = {
        x,x,o,
        o,o,x,
        x,o,x
    };

    AI::Move<Grid>* move = AI::minimax(Grid{ grid }, x, x, o);

    std::ostringstream os;
    os << *move;

    std::string actual = os.str();
    std::string expected = "\
[x,x,o,\n\
 o,o,x,\n\
 x,o,x]\n\
0\n\
0\n\
-1\n";

    delete move;

    std::cout << "Test 6 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl; 
}

void test7()
{
    char grid[] = {
        _,_,_,
        _,_,_,
        _,_,_
    };

    AI::Move<Grid>* move = AI::minimax(Grid{ grid }, x, x, o);

    std::ostringstream os;
    os << *move 
        << move->at(0) 
        << move->at(1)
        << move->at(2)
        << move->at(3)
        << move->at(4)
        << move->at(5)
        << move->at(6)
        << move->at(7)
        << move->at(8);

    std::string actual = os.str();
    std::string expected = "\
[ , , ,\n\
  , , ,\n\
  , , ]\n\
0\n\
9\n\
0\n\
[x, , ,\n\
  , , ,\n\
  , , ]\n\
0\n\
8\n\
3\n\
[ ,x, ,\n\
  , , ,\n\
  , , ]\n\
0\n\
8\n\
0\n\
[ , ,x,\n\
  , , ,\n\
  , , ]\n\
0\n\
8\n\
3\n\
[ , , ,\n\
 x, , ,\n\
  , , ]\n\
0\n\
8\n\
0\n\
[ , , ,\n\
  ,x, ,\n\
  , , ]\n\
0\n\
8\n\
0\n\
[ , , ,\n\
  , ,x,\n\
  , , ]\n\
0\n\
8\n\
2\n\
[ , , ,\n\
  , , ,\n\
 x, , ]\n\
0\n\
8\n\
4\n\
[ , , ,\n\
  , , ,\n\
  ,x, ]\n\
0\n\
8\n\
1\n\
[ , , ,\n\
  , , ,\n\
  , ,x]\n\
0\n\
8\n\
4\n";

    delete move;

    std::cout << "Test 7 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test8()
{
    char grid[] = {
        x,x,o,
        _,o,_,
        _,_,_
    };

    AI::Move<Grid>* move = AI::minimax(Grid{ grid }, x, x, o);

    std::ostringstream os;
    os << *move 
        << move->at(0) 
        << move->at(1)
        << move->at(2)
        << move->at(3)
        << move->at(4);

    std::string actual = os.str();
    std::string expected = "\
[x,x,o,\n\
  ,o, ,\n\
  , , ]\n\
0\n\
5\n\
2\n\
[x,x,o,\n\
 x,o, ,\n\
  , , ]\n\
-10\n\
4\n\
1\n\
[x,x,o,\n\
  ,o,x,\n\
  , , ]\n\
-10\n\
4\n\
1\n\
[x,x,o,\n\
  ,o, ,\n\
 x, , ]\n\
0\n\
4\n\
0\n\
[x,x,o,\n\
  ,o, ,\n\
  ,x, ]\n\
-10\n\
4\n\
0\n\
[x,x,o,\n\
  ,o, ,\n\
  , ,x]\n\
-10\n\
4\n\
0\n";

    delete move;

    std::cout << "Test 8 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test9()
{
    char grid[] = {
        x,x,o,
        o,o,_,
        x,_,_
    };

    AI::Move<Grid>* move = AI::minimax(Grid{ grid }, x, x, o);

    std::ostringstream os;
    os << *move 
        << move->at(0) 
        << move->at(1) 
        << move->at(2);

    std::string actual = os.str();
    std::string expected = "\
[x,x,o,\n\
 o,o, ,\n\
 x, , ]\n\
0\n\
3\n\
0\n\
[x,x,o,\n\
 o,o,x,\n\
 x, , ]\n\
0\n\
2\n\
0\n\
[x,x,o,\n\
 o,o, ,\n\
 x,x, ]\n\
-10\n\
2\n\
0\n\
[x,x,o,\n\
 o,o, ,\n\
 x, ,x]\n\
-10\n\
2\n\
0\n";

    delete move;

    std::cout << "Test 9 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test10()
{
    char grid[] = {
        x,x,o,
        o,o,x,
        x,o,_
    };

    AI::Move<Grid>* move = AI::minimax(Grid{ grid }, x, x, o);

    std::ostringstream os;
    os << *move
        << move->at(0);

    std::string actual = os.str();
    std::string expected = "\
[x,x,o,\n\
 o,o,x,\n\
 x,o, ]\n\
0\n\
1\n\
0\n\
[x,x,o,\n\
 o,o,x,\n\
 x,o,x]\n\
0\n\
0\n\
-1\n";

    delete move;

    std::cout << "Test 10 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}