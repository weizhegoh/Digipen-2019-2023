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



std::ostream& operator<<(std::ostream& os, const std::vector<AI::TreeNode*>& v)
{
    for (std::vector<AI::TreeNode*>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        os << **it;
        if (it != v.end() - 1)
            os << ' ';
    }
    return os;
}


// Non evaluated selftest
void test0()
{

}

void test1()
{
    std::istringstream istream{""};
    
    AI::TreeNode tree;

    istream >> tree;

    AI::GetTreeAdjacents getAdjacents;

    std::vector<AI::TreeNode*> a = getAdjacents(&tree);
 
    std::ostringstream os;
    os << a;

    std::string actual = os.str();
    std::string expected = "";

    std::cout << "Test 1 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test2()
{
    std::istringstream istream{"\
a {3\
 aa {0 }\
 x {2 aba {0 } abc {0 } }\
 ac {0 } } "};

    AI::TreeNode tree;

    istream >> tree;

    AI::GetTreeAdjacents getAdjacents;

    std::vector<AI::TreeNode*> a = getAdjacents(&tree);

    std::ostringstream os;
    os << a;

    std::string actual = os.str();
    std::string expected = "x {2 aba {0 } abc {0 } } ";

    std::cout << "Test 2 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test3()
{
    std::istringstream istream{"\
a {3\
 x {0 }\
 x {1 aba {0 } }\
 x {2 aca {0 } acc {0 } } } "};

    AI::TreeNode tree;

    istream >> tree;

    AI::GetTreeStochasticAdjacents getAdjacents;

    std::vector<AI::TreeNode*> a = getAdjacents(&tree);

    std::ostringstream os;
    os << a;

    std::string actual = os.str();
    std::string expected1 = "x {0 }  x {1 aba {0 } }  x {2 aca {0 } acc {0 } } ";
    std::string expected2 = "x {0 }  x {2 aca {0 } acc {0 } }  x {1 aba {0 } } ";

    std::string expected3 = "x {1 aba {0 } }  x {0 }  x {2 aca {0 } acc {0 } } ";
    std::string expected4 = "x {2 aca {0 } acc {0 } }  x {0 }  x {1 aba {0 } } ";

    std::string expected5 = "x {1 aba {0 } }  x {2 aca {0 } acc {0 } }  x {0 } ";
    std::string expected6 = "x {2 aca {0 } acc {0 } }  x {1 aba {0 } }  x {0 } ";

    std::cout << "Test 3 : ";
    if (actual == expected1 ||
        actual == expected2 ||
        actual == expected3 ||
        actual == expected4 ||
        actual == expected5 ||
        actual == expected6)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

// Next tests replace connected x-s with z different ways

void test4()
{
    std::istringstream istream{"\
a {3\
 aa {0 }\
 ab {2 aba {0 } abb {0 } }\
 ac {0 } } "};

    AI::TreeNode tree;

    istream >> tree;

    AI::GetTreeAdjacents getAdjacents;

    AI::Flood_Fill_Recursive(&getAdjacents).run(&tree, "z");

    std::ostringstream os;
    os << tree;

    std::string actual = os.str();
    std::string expected = "a {3 aa {0 } ab {2 aba {0 } abb {0 } } ac {0 } } ";

    std::cout << "Test 4 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test5()
{
    std::istringstream istream{"\
a {3\
 aa {0 }\
 x {2 aba {0 } x {0 } }\
 ac {0 } } "};

    AI::TreeNode tree;

    istream >> tree;

    AI::GetTreeAdjacents getAdjacents;

    AI::Flood_Fill_Recursive(&getAdjacents).run(&tree, "z");

    std::ostringstream os;
    os << tree;

    std::string actual = os.str();
    std::string expected = "a {3 aa {0 } z {2 aba {0 } z {0 } } ac {0 } } ";

    std::cout << "Test 5 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test6()
{
    std::istringstream istream{"\
a {3\
 aa {0 }\
 x {2 aba {0 } x {0 } }\
 ac {0 } } "};

    AI::TreeNode tree;

    istream >> tree;

    AI::GetTreeAdjacents getAdjacents;

    AI::Flood_Fill_Iterative<AI::Queue>(&getAdjacents).run(&tree, "z");

    std::ostringstream os;
    os << tree;

    std::string actual = os.str();
    std::string expected = "a {3 aa {0 } z {2 aba {0 } z {0 } } ac {0 } } ";

    std::cout << "Test 6 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test7()
{
    std::istringstream istream{"\
a {3\
 aa {0 }\
 x {2 aba {0 } x {0 } }\
 ac {0 } } "};

    AI::TreeNode tree;

    istream >> tree;

    AI::GetTreeAdjacents getAdjacents;

    AI::Flood_Fill_Iterative<AI::Stack>(&getAdjacents).run(&tree, "z");

    std::ostringstream os;
    os << tree;

    std::string actual = os.str();
    std::string expected = "a {3 aa {0 } z {2 aba {0 } z {0 } } ac {0 } } ";

    std::cout << "Test 7 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test8()
{
    std::istringstream istream{"\
a {3\
 aa {0 }\
 x {2 aba {0 } x {0 } }\
 ac {0 } } "};

    AI::TreeNode tree;

    istream >> tree;

    AI::GetTreeStochasticAdjacents getAdjacents;

    AI::Flood_Fill_Recursive(&getAdjacents).run(&tree, "z");

    std::ostringstream os;
    os << tree;

    std::string actual = os.str();
    std::string expected = "a {3 aa {0 } z {2 aba {0 } z {0 } } ac {0 } } ";

    std::cout << "Test 8 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test9()
{
    std::istringstream istream{"\
a {3\
 aa {0 }\
 x {2 aba {0 } x {0 } }\
 ac {0 } } "};

    AI::TreeNode tree;

    istream >> tree;

    AI::GetTreeStochasticAdjacents getAdjacents;

    AI::Flood_Fill_Iterative<AI::Queue>(&getAdjacents).run(&tree, "z");

    std::ostringstream os;
    os << tree;

    std::string actual = os.str();
    std::string expected = "a {3 aa {0 } z {2 aba {0 } z {0 } } ac {0 } } ";

    std::cout << "Test 9 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test10()
{
    std::istringstream istream{"\
a {3\
 aa {0 }\
 x {2 aba {0 } x {0 } }\
 ac {0 } } "};

    AI::TreeNode tree;

    istream >> tree;

    AI::GetTreeStochasticAdjacents getAdjacents;

    AI::Flood_Fill_Iterative<AI::Stack>(&getAdjacents).run(&tree, "z");

    std::ostringstream os;
    os << tree;

    std::string actual = os.str();
    std::string expected = "a {3 aa {0 } z {2 aba {0 } z {0 } } ac {0 } } ";

    std::cout << "Test 10 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}
