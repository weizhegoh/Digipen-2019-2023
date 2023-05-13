#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <stack>
#include <algorithm>

#include "data.h"

#define UNUSED(x) (void)x;

// This is a domain specific functor class that is used to find a solution 
// for a one-dimensional Sudoku by using Backtracking algorithm.
// Functor returns next location which is not occupied yet on the map.
// If no location found, returns Location<>{ nullptr, 0 }.
class NextLocation_Sudoku1D
{
    AI::MapInt1D* map;

public:

    NextLocation_Sudoku1D(void* map)
        : map{ static_cast<AI::MapInt1D*>(map) }
    {
    }

    AI::Location<> operator()() const
    {
        // Your code ...
        
        return AI::Location<int>{ nullptr, 0 };
    }
};

// This is a domain specific functor class that is used to find a solution 
// for a two-dimensional Sudoku by using Backtracking algorithm.
// Functor returns next location which is not occupied yet on the map.
// If no location found, returns Location<>{ nullptr, 0 }.
class NextLocation_Sudoku2D
{
    AI::MapInt2D* map;

public:

    NextLocation_Sudoku2D(void* map)
        : map{ static_cast<AI::MapInt2D*>(map) }
    {
    }

    AI::Location<> operator()() const
    {
        // Your code ...

        return AI::Location<>{ nullptr, 0 };
    }
};

// This is a domain specific functor class that is used to find a solution 
// for a one-dimensional Sudoku by using Backtracking algorithm.
// Functor returns next candidate for a specified location on the map.
// If no candidate found, returns 0.
class NextCandidate_Sudoku1D
{
    AI::MapInt1D* map;

public:

    NextCandidate_Sudoku1D(void* map)
        : map{ static_cast<AI::MapInt1D*>(map) }
    {
    }

    int operator()(AI::Location<> location)
    {
        UNUSED(location)

        // Your code ...

        return 0;
    }
};

// This is a domain specific functor class that is used to find a solution 
// for a two-dimensional Sudoku by using Backtracking algorithm.
// Functor returns next candidate for a specified location on the map.
// If no candidate found, returns 0.
class NextCandidate_Sudoku2D
{
    AI::MapInt2D* map;

public:

    NextCandidate_Sudoku2D(void* map)
        : map{ static_cast<AI::MapInt2D*>(map) }
    {
    }

    int operator()(AI::Location<> location)
    {
        UNUSED(location)

        // Your code ...

        return 0;
    }
};


namespace AI
{
    // Template class of the Backtracking algorithm.
    // Parameter NL defines domain-specific NextLocation functor. 
    // Parameter NC defines domain-specific NextCandidate functor. 
    template<typename NL, typename NC>
    class Backtracking
    {
        std::stack<Location<>> stack;
        NL next_location;
        NC next_candidate;
 
    public:
        Backtracking(void* map = nullptr)
            : next_location{ map }, next_candidate{ map }
        {
        }

        // Find solution in a blocking mode
        void run()
        {
            // Your code ...
        }

        // One iteration of the search. Used by run() in a blocking running mode
        // or can be called by timer in an non-blocking run
        bool solve()
        {
            // Your code ...

            return false;
        }
    };

} // end namespace

#endif