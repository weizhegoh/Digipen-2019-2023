#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include "functions.h"

std::string join(const std::vector<std::string>& v, char c)
{
    std::string s;

    for (std::vector<std::string>::const_iterator p = v.begin();
        p != v.end(); ++p) {
        s += *p;
        if (p != v.end() - 1)
            s += c;
    }
    return s;
}

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
        f[id-1]();
    else
        std::cout << "Test " << id << " not found." << std::endl;

    return 0;
}

void test0()
{

}

void test1()
{
    std::string string{ "a b c d e f g" }; // Incorrect input
    AI::Node<std::string> tree;

    std::istringstream istream{ string };
    std::ostringstream ostream;

    istream >> tree;
    ostream << tree;

    std::string actual = ostream.str();
    std::string expected = "a {0 } ";

    std::cout << "Test 1 : " <<  (actual == expected ? "Pass" : ("Failed (\n" + actual + ")")) << std::endl;
}

void test2()
{
    std::string string{ "" }; 
    AI::Node<std::string> tree;

    std::istringstream istream{ string };
    std::ostringstream ostream;

    istream >> tree;
    ostream << tree;

    std::string actual = ostream.str();
    std::string expected = " {0 } ";

    std::cout << "Test 2 : " << (actual == expected ? "Pass" : ("Failed (\n" + actual + ")")) << std::endl;
}

void test3()
{
    std::string string{ "aaa {0 } " };
    AI::Node<std::string> tree;

    std::istringstream istream{ string };
    std::ostringstream ostream;

    istream >> tree;
    ostream << tree;

    std::string actual = ostream.str();
    std::string expected = string;

    std::cout << "Test 3 : " << (actual == expected ? "Pass" : ("Failed (\n" + actual + ")")) << std::endl;
}

void test4()
{
    std::string string{ "a {1 aa {1 aaa {0 } } } " };
    AI::Node<std::string> tree;

    std::istringstream istream{ string };
    std::ostringstream ostream;

    istream >> tree;
    ostream << tree;

    std::string actual = ostream.str();
    std::string expected = string;

    std::cout << "Test 4 : " << (actual == expected ? "Pass" : ("Failed (\n" + actual + ")")) << std::endl;
}

void test5()
{
    std::string string{ "a {3 aa {0 } bb {0 } cc {0 } } " };
    AI::Node<std::string> tree;

    std::istringstream istream{ string };
    std::ostringstream ostream;

    istream >> tree;
    ostream << tree;

    std::string actual = ostream.str();
    std::string expected = string;

    std::cout << "Test 5 : " << (actual == expected ? "Pass" : ("Failed (\n" + actual + ")")) << std::endl;
}

void test6()
{
    std::string string{ "a {3 aa {3 aaa {0 } aab {0 } ooo {0 } } ooo {3 aba {0 } abb {0 } abc {0 } } ac {3 aca {0 } ooo {0 } acc {0 } } } " };
    AI::Node<std::string> tree;

    std::istringstream istream{ string };
    std::ostringstream ostream;

    istream >> tree;
    ostream << tree;

    std::string actual = ostream.str();
    std::string expected = string;

    std::cout << "Test 6 : " << (actual == expected ? "Pass" : ("Failed (\n" + actual + ")")) << std::endl;
}

void test7()
{
    std::string string{ "a {3 aa {3 aaa {0 } aab {0 } ooo {0 } } ooo {3 aba {0 } abb {0 } abc {0 } } ac {3 aca {0 } ooo {0 } acc {0 } } } " };
    AI::Node<std::string> root;

    std::string lookingfor = "N";

    std::istringstream istream{ string };
    istream >> root;

    AI::Node<std::string> * actual = AI::BFS(root, lookingfor);
    AI::Node<std::string> * expected = nullptr;

    std::cout << "Test 7 : " << (actual == expected ? "Pass" : ("Failed (\n" + actual->value + ")")) << std::endl;
}

void test8()
{
    std::string string{ "a {3 aa {3 aaa {0 } aab {0 } ooo {0 } } ooo {3 aba {0 } abb {0 } abc {0 } } ac {3 aca {0 } ooo {0 } acc {0 } } } " };
    
    std::string lookingfor = "ooo";

    AI::Node<std::string> root;

    std::istringstream istream{ string };
    istream >> root;
     
    std::string actual = join(AI::BFS(root, lookingfor)->getPath(), ',');
    std::string expected = "a,ooo";

    std::cout << "Test 8 : " << (actual == expected ? "Pass" : ("Failed (\n" + actual + ")")) << std::endl;
}

void test9()
{
    std::string string{ "a {3 aa {3 aaa {0 } aab {0 } ooo {0 } } ooo {3 aba {0 } abb {0 } abc {0 } } ac {3 aca {0 } ooo {0 } acc {0 } } } " };
    AI::Node<std::string> root;

    std::string lookingfor = "N";

    std::istringstream istream{ string };
    istream >> root;

    AI::Node<std::string>* actual = AI::DFS(root, lookingfor);
    AI::Node<std::string>* expected = nullptr;

    std::cout << "Test 9 : " << (actual == expected ? "Pass" : ("Failed (\n" + actual->value + ")")) << std::endl;
}

void test10()
{
    std::string string{ "a {3 aa {3 aaa {0 } aab {0 } ooo {0 } } ooo {3 aba {0 } abb {0 } abc {0 } } ac {3 aca {0 } ooo {0 } acc {0 } } } " };

    std::string lookingfor = "ooo";

    AI::Node<std::string> root;

    std::istringstream istream{ string };
    istream >> root;

    std::string actual = join(AI::DFS(root, lookingfor)->getPath(), ',');
    std::string expected = "a,ac,ooo";

    std::cout << "Test 10 : " << (actual == expected ? "Pass" : ("Failed (\n" + actual + ")")) << std::endl;
}
