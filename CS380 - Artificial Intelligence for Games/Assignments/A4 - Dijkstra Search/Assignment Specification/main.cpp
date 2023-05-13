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

std::ostream& operator<<(std::ostream& os, const std::vector<char>& rhs)
{
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
        os << *it << (it + 1 != rhs.end() ? "," : "");
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<AI::Node*>& rhs)
{
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
        os << **it << (it + 1 != rhs.end() ? "  " : "");
    return os;
}


// Non evaluated selftest
void test0()
{
    AI::Node n1{{ }, 0, '1' };
    AI::Node n2{{ }, 0, '2' };
    AI::Node n3{{ }, 0, '3' };

    {
        AI::Key v{ 1, 2, 3, 4, 5, 0, 7, 8, 6 };
        AI::HashTable root;
        AI::Node* actual = root.find(v);
        AI::Node* expected = nullptr;

        if (actual != expected)
            std::cout << "Test 0_1 : Failed (" << std::endl << *actual << ')' << std::endl;
    }
    {
        AI::Key v{ 1, 3, 2, 4, 5, 0, 7, 6, 8 };
        AI::HashTable root;
        root.add(v, new AI::Node{{1, 2}, 3, '4' });
        AI::Node* result1 = root.find(v);
        AI::Node* result2 = root.find(v); // Repeat

        std::ostringstream os;
        os << *result1 << "  " << *result2;
        std::string actual = os.str();
        std::string expected = "1,2 3 4  1,2 3 4";

        if (actual != expected)
            std::cout << "Test 0_2 : Failed (" << std::endl << actual << ')' << std::endl;
    }

    {
        AI::Key v{ 8, 3, 2, 4, 5, 0, 7, 6, 1 };
        AI::HashTable root;
        root.add(v, new AI::Node{{1, 2}, 3, '4' });
        root.add(v, new AI::Node{{5, 6}, 7, '8' }); // Reset
        AI::Node* result = root.find(v);

        std::ostringstream os;
        os << *result;
        std::string actual = os.str();
        std::string expected = "5,6 7 8";

        if (actual != expected)
            std::cout << "Test 0_3 : Failed (" << std::endl << actual << ')' << std::endl;
    }

    {
        AI::PriorityQueue q;
        q.push(new AI::Node{{0, 0}, 3, 'S', &n1 });
        q.push(new AI::Node{{1, 1}, 2, 'N', &n2 });
        q.push(new AI::Node{{2, 2}, 5, 'E', &n3 });

        std::ostringstream os;
        os << q;

        std::string actual = os.str();
        std::string expected = "1,1 2 N 2  0,0 3 S 1  2,2 5 E 3  ";

        if (actual != expected)
            std::cout << "Test 0_4 : Failed (" << std::endl << actual << ')' << std::endl;
    }

    {
        AI::PriorityQueue q;
        q.push(new AI::Node{{0, 0}, 3, 'S', &n1 });
        q.push(new AI::Node{{1, 1}, 2, 'N', &n2 });
        q.push(new AI::Node{{2, 2}, 1, 'E', &n3 });

        std::ostringstream os;
        os << q;

        std::string actual = os.str();
        std::string expected = "2,2 1 E 3  1,1 2 N 2  0,0 3 S 1  ";

        if (actual != expected)
            std::cout << "Test 0_5 : Failed (" << std::endl << actual << ')' << std::endl;
    }
}

void test1()
{
    int map[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0
    };   
    
    AI::GetMapAdjacents getAdjacents{map, 5};

    std::vector<AI::Node*> adjacents = getAdjacents({1, 1});

    std::ostringstream os;
    os << adjacents;

    std::string actual = os.str();
    std::string expected = "1,0 10 W  1,2 10 E  0,1 10 N  2,1 10 S";

    std::cout << "Test 1 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;

    for (auto a : adjacents)
        delete a;
}

void test2()
{
    int map[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    AI::GetMapAdjacents getAdjacents{map, 5};

    std::vector<AI::Node*> adjacents = getAdjacents({4, 4});

    std::ostringstream os;
    os << adjacents;

    std::string actual = os.str();
    std::string expected = "4,3 10 W  3,4 10 N";

    std::cout << "Test 2 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;

    for (auto a : adjacents)
        delete a;
}

void test3()
{
    int map[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    AI::GetMapAdjacents getAdjacents{map, 5};

    std::vector<AI::Node*> adjacents = getAdjacents({3, 2});

    std::ostringstream os;
    os << adjacents;

    std::string actual = os.str();
    std::string expected = "3,3 10 E  4,2 10 S";

    std::cout << "Test 3 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;

    for (auto a : adjacents)
        delete a;
}

void test4()
{
    AI::GetMapAdjacents getMapAdjacents;

    AI::Dijkstras dijkstras(&getMapAdjacents);

    std::vector<char> path = dijkstras.run({1, 1}, {1, 1});

    std::ostringstream os;
    os << path;

    std::string actual = os.str();
    std::string expected = "";

    std::cout << "Test 4 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test5()
{
    int map[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    AI::GetMapAdjacents getAdjacents{map, 5};

    AI::Dijkstras dijkstras(&getAdjacents);

    std::vector<char> path = dijkstras.run({1, 1}, {1, 1});

    std::ostringstream os;
    os << path;

    std::string actual = os.str();
    std::string expected = "";

    std::cout << "Test 5 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test6()
{
    int map[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    AI::GetMapAdjacents getAdjacents{map, 5};

    AI::Dijkstras dijkstras(&getAdjacents);

    std::vector<char> path = dijkstras.run({1, 1}, {1, 3});

    std::ostringstream os;
    os << path;

    std::string actual = os.str();
    std::string expected = "E,E";

    std::cout << "Test 6 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test7()
{
    int map[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    AI::GetMapAdjacents getAdjacents{map, 5};

    AI::Dijkstras dijkstras(&getAdjacents);

    std::vector<char> path = dijkstras.run({0, 4}, {4, 4});

    std::ostringstream os;
    os << path;

    std::string actual = os.str();
    std::string expected = "S,S,S,S";

    std::cout << "Test 7 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test8()
{
    int map[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    AI::GetMapAdjacents getAdjacents{map, 5};

    AI::Dijkstras dijkstras(&getAdjacents);

    std::vector<char> path = dijkstras.run({1, 1}, {3, 3});

    std::ostringstream os;
    os << path;

    std::string actual = os.str();
    std::string expected = "E,E,S,S";

    std::cout << "Test 8 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test9()
{
    int map[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    AI::GetMapAdjacents getAdjacents{map, 5};

    AI::Dijkstras dijkstras(&getAdjacents);

    std::vector<char> path = dijkstras.run({1, 1}, {30, 30});

    std::ostringstream os;
    os << path;

    std::string actual = os.str();
    std::string expected = "";

    std::cout << "Test 9 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test10()
{
    int map[] = {
        0, 1, 0, 0, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        0, 0, 0, 1, 0
    };

    AI::GetMapAdjacents getAdjacents{map, 5};

    AI::Dijkstras dijkstras(&getAdjacents);

    std::vector<char> path = dijkstras.run({0, 0}, {4, 4});

    std::ostringstream os;
    os << path;

    std::string actual = os.str();
    std::string expected = "S,S,S,S,E,E,N,N,N,N,E,E,S,S,S,S";

    std::cout << "Test 10 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}
