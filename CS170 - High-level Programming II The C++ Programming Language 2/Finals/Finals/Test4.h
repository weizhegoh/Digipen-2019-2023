#include "9_Templates.h"

/*
#include <iostream>

template<typename T>
struct Node
{
    T data;
    Node* next;
};

template<typename T>
class List
{
protected: 
    Node<T>* head;

public:
    List() : head(nullptr) {}

    void push_front(T value)
    {
        Node<T>* newNode = new Node<T>;

        newNode->data = value;
        newNode->next = head;

        head = newNode;
    }
};

template <typename T>
class Stack: protected List<T>
{
public:
    void push(T value)
    {
        List<T>::push_front(value);
    }

    T pop()
    {
        if (isEmpty())
        {
            throw "Empty";
        }
            
        T deleteValue = 0;

        deleteValue = List<T>::head->data;

        Node<T>* deleteNode = nullptr;
        Node<T>* current = this->head;

        deleteNode = current;

        this->head = current->next;

        delete deleteNode;
        return deleteValue;
    }

    bool isEmpty()
    {
        if (List<T>::head == nullptr)
            return true;
        else
            return false;
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