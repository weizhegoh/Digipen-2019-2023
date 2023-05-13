#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <deque>
#include <list>
#include <vector>
#include <initializer_list>

namespace AI
{
    // Key part from key-value pairs that are used in hash tables
    class Key : public std::vector<int>
    {
    public:
        Key(const std::initializer_list<int>& list)
        {
            for (auto it = list.begin(); it != list.end(); ++it)
                push_back(*it);
        }

        int pop() 
        {
            int n = front();
            erase(begin());
            return n;
        }

        bool operator==(const Key& rhs) const
        {
            if (size() != rhs.size())
                return false;

            for (auto ia = begin(), ib = rhs.begin(); ia != end(); ++ia, ++ib)
                if (*ia != *ib)
                    return false;

            return true;
        }

        friend std::ostream& operator<<(std::ostream& os, const Key& rhs)
        {
            for (auto it = rhs.begin(); it != rhs.end(); ++it)
                os << *it << (it + 1 != rhs.end() ? "," : "");
            return os;
        }
    };

    // Node is used to keep track of visited nodes or nodes to be visited 
    // during search
    class Node
    {
    public:
        Key key;
        int g;
        char info;
        Node* parent;

        Node(Key key = {}, int g = 0, char info = ' ', Node* parent = nullptr)
            : key(key), g{ g }, info{ info }, parent{ parent }
        {
        }

        static bool less(const Node* a, const Node* b) 
        { 
            return a->g < b->g; 
        }

        friend std::ostream& operator<<(std::ostream& os, const Node& rhs)
        {
            os << rhs.key << ' ' << rhs.g << ' ' << rhs.info;
            if (rhs.parent)
                os << ' ' << rhs.parent->info;
            return os;
        }
    };


    // Abstract base class for domain specific functors that return adjacent nodes
    class GetAdjacents
    {

    public:

        virtual ~GetAdjacents()
        {
        }

        virtual std::vector<Node*> operator()(Key key) = 0;
    };


    // Hash table that uses fixed size key of integers to 
    // locate value in memory in constant time 
    class HashTable
    {
        Node* value;
        HashTable* children[10];

    public:
        HashTable()
            : value{ nullptr }
            , children{ }
        {
            for (int i = 0; i < 10; ++i)
                children[i] = nullptr;
        }

        ~HashTable()
        {
            for (auto child : children)
            {
                if (child)
                {
                    delete child->value;
                    delete child;
                }
            }
        }

        void add(Key key, Node* v)
        {
            HashTable* pNode = this;
            while (key.size())
            {
                int a = key.pop();
                if (pNode->children[a] == nullptr)
                    pNode->children[a] = new HashTable();
                pNode = pNode->children[a];
            }
            if (pNode->value)
              delete pNode->value;
            pNode->value = v;
        }

        Node* find(Key key)
        {
            HashTable* pNode = this;
            while (key.size())
            {
                int a = key.pop();
                if (pNode->children[a] == nullptr)
                    return nullptr;
                pNode = pNode->children[a];
            }
            return pNode->value;
        }
    };

    // Custom made priority queue that is similar to std::priority_queue but 
    // in addition provide access to elements of the queue
    class PriorityQueue : public std::list<Node*>
    {
    public:
        ~PriorityQueue()
        {
            for (auto e : *this)
                delete e;
        }

        Node* pop()
        {
            Node* node = front();
            pop_front();
            return node;
        }

        void push(Node* node)
        {
            push_back(node);
            sort(Node::less);
        }

        Node* find(Key key)
        {
            for (auto it = begin(); it != end(); ++it)
                if ((*it)->key == key)
                    return *it;
            return nullptr;
        }

        friend std::ostream& operator<<(std::ostream& os, const PriorityQueue& rhs)
        {
            for (auto it = rhs.begin(); it != rhs.end(); ++it)
                os << **it << "  ";
            return os;
        }
    };

} // end namespace

#endif