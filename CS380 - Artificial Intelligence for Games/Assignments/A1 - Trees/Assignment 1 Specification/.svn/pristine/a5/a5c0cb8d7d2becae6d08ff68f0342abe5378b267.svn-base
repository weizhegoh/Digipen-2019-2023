#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

#include "data.h"

namespace AI 
{
    #define UNUSED(expr) (void)expr;

    // A simple graph node definition with serialization functions
    template<typename T>
    struct Node
    {
        // Member data

        T value;
        Node* parent;
        std::list<Node*> children;

        Node(T value = {}, Node* parent = nullptr, 
                    const std::list<Node*>& children = {})
            : value{ value }, parent{ parent }, children{ children }
        {

        }

        ~Node()
        {
            for (auto child : children)
                delete child;
        }

        // Serialization
        friend std::ostream& operator<<(std::ostream& os, Node const& rhs)
        {
            os << rhs.value; // Just for test, you can change it
            // ...
            return os;
        }

        // Deserialization
        friend std::istream& operator>>(std::istream& is, Node& rhs)
        {
            is >> rhs.value; // Just for test, you can change it
            // ...
            return is;
        }

        // Returns values from root to this node as an array
        std::vector<T> getPath() const
        {
            std::vector<T> r;
            // ...
            return r;
        }
    };

    // Implementation of the Breadth-First Search algorithm
    template<typename T>
    Node<T>* BFS(Node<T> & node, const T & lookingfor)
    {
        UNUSED(node)
        UNUSED(lookingfor)
        // ...
        return nullptr;
    }

    // Implementation of the Depth-First Search algorithm
    template<typename T>
    Node<T>* DFS(Node<T> & node, const T & lookingfor)
    {
        UNUSED(node)
        UNUSED(lookingfor)
        // ...
        return nullptr;
    }

} // end namespace

#endif