#include "11_Inheritance.h"

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
protected:
    Node<T>* head;

public:
    List() :head(nullptr) {}

    void push_front(T value)
    {
        Node<T>* newNode = new Node<T>;

        newNode->data = value;
        newNode->next = head;

        head = newNode;
    }
};

template <typename T>
class Stack : protected List<T>
{
public:
    void push(T value)
    {
        List<T>::push_front(value);
    }

    T pop() noexcept (false)
    {
        if (isEmpty() == true)
        {
            throw "Error";
        }

        T deleteValue = 0;

        deleteValue = List<T>::head->data;

        Node<T>* deleteNode = nullptr;

        Node<T>* current = List<T>::head;

        deleteNode = current;

        List<T>::head = current->next;

        delete deleteNode;
        return deleteValue;
    }

    bool isEmpty() noexcept
    {
        if (List<T>::head != nullptr)
            return false;
        else
            return true;
    }
};

int main()
{
    Stack<int> s;
    std::cout << s.isEmpty();

    s.push(1);
    s.push(2);
    s.push(3);
    try {
        std::cout << s.pop();
        std::cout << s.pop();
        std::cout << s.pop();
        std::cout << s.pop();
    }
    catch (const char* msg)
    {
        std::cout << msg;
    }
    return 0;
}
*/