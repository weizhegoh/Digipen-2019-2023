#include "10_Exceptions.h"

//g++ -Wall -Wextra -Werror -Wconversion -pedantic -std=c++11 -o Test.exe Test.cpp

/*
#include <iostream>

template <typename T>
struct Node
{
    T data;
    Node* next;
};

template <typename T>
class List
{
private:
    Node<T>* head;

public:
    List() :head(nullptr) {}

    Node<T>* getHead()
    {
        return this->head;
    }

    Node<T>* setHead(Node <T>* node)
    {
        return this->head = node;
    }

    void push_front(T value)
    {
        Node<T>* newNode = new Node<T>;

        newNode->data = value;
        newNode->next = head;

        head = newNode;
    }
};

template <typename T>
class Stack
{
    List<T> list;

public:
    void push(T value)
    {
        list.push_front(value);
    }

    T pop() noexcept (false)
    {
        if (isEmpty() == true)
        {
            throw "Empty Stack";
        }

        T deleteValue = 0;

        deleteValue = list.getHead()->data;

        Node<T>* deleteNode = nullptr;
        Node<T>* current = list.getHead();

        deleteNode = current;

        list.setHead(current->next);

        delete deleteNode;
        return deleteValue;
    }

    bool isEmpty() noexcept
    {
        if (list.getHead() != nullptr)
            return false;
        else
            return true;
    }
};

int main(void)
{
    Stack<int> s1;

    s1.push(3);
    s1.push(2);
    s1.push(1);

    try
    {
        std::cout << s1.pop() << "->";
        std::cout << s1.pop() << "->";
        std::cout << s1.pop() << "->";
        std::cout << s1.pop() << "->";
    }
    catch (const char* str)
    {
        std::cout << str << std::endl;
    }

    Stack<float> s2;

    s2.push(10.3f);
    s2.push(10.2f);
    s2.push(10.1f);

    try
    {
        std::cout << s2.pop() << "->";
        std::cout << s2.pop() << "->";
        std::cout << s2.pop() << "->";
        std::cout << s2.pop() << "->";
    }
    catch (const char* str)
    {
        std::cout << str << std::endl;
    }

    return 0;
}
*/