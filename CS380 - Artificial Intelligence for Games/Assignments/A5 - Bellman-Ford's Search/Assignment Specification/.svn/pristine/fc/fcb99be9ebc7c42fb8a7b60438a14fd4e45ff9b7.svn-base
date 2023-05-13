#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <array>
#include <climits>
#include <algorithm>

#include "data.h"

#define UNUSED(x) (void)x;

namespace AI 
{
    const int null = -1;
    const int inf = INT_MAX;

    // An implementation of the Bellman-Ford algorithm. 
    // The algorithm finds the shortest path between a
    // starting node and all other nodes in the graph. 
    // The algorithm also detects negative cycles.
    template<int SIZE = 0>
     class BellmanFord
    {
        int* matrix; // the cost adjacency matrix
        int* distance;
        int* predecessor;

    public:

        BellmanFord(int* matrix = nullptr)
            : matrix{ matrix }, distance{ nullptr }, predecessor{ nullptr }
        {

            //...

         }

        ~BellmanFord()
        {

            //...
 
        }

        // Runs the algorithm. Returns false when negative 
        // cycles are found, and true otherwise.
        bool run(int starting = 0)
        {
            UNUSED(starting)

            //...
 
            return true;
        }

        // Interpretation of the result that reconstructs the shortest path
        // from starting point to target
        std::vector<int> getPath(int target)
        {
            UNUSED(target)

            std::vector<int> path{};

            //...
 
            return path;
        }

        // Interpretation of the result that create a route (step-by-step 
        // description) of the shortest path from start to end with cost
        std::vector<std::array<int, 3>> getRoute(int target)
        {
            UNUSED(target)

            std::vector<std::array<int, 3>> route{};

            //...

            return route;
        }

        friend std::ostream& operator<<(std::ostream& os, const BellmanFord& rhs)
        {

            UNUSED(rhs)
 
            //...

            return os;
        }
    };

} // end namespace

#endif