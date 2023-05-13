#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <utility>

#define UNUSED(x) (void)x;

namespace AI
{

    // Struct that defines a one-dimensional map of cells, 
    // when each cell is defined as an integers.
    struct MapInt1D
    {
        int* base;
        int size;

        friend std::ostream& operator<<(std::ostream& os, const MapInt1D& rhs)
        {
            for (int i = 0; i < rhs.size; ++i)
                os << rhs.base[i] << (i != rhs.size-1 ? "," : "");
            return os;
        }
    };

    // Struct that defines a two-dimensional map of cells, 
    // when each cell is defined as an integers.
    struct MapInt2D
    {
        int* base;
        int width;
        int height;
 
        friend std::ostream& operator<<(std::ostream& os, const MapInt2D& rhs)
        {
            for (int j = 0; j < rhs.height; ++j)
            {
                for (int i = 0; i < rhs.width; ++i)
                    os << rhs.base[j * rhs.width + i] 
                            << (i * j != (rhs.width-1)*(rhs.height-1) ? "," : "");
                os << std::endl;
            }
            return os;
        }
    };

    // Template class for a location on a map used by Backtracking algorithm.
    // Parameter T defines the type of a cell on the map.
    // base member  - an address of the first cell of the map.
    // index member - 0-based index of a cell on the map.
    template<typename T = int>
    class Location
    {
        T* base;
        int index;

    public:
        Location(T* base, int index)
            : base{ base }, index{ index }
        {
        }

        bool notFound()
        {
            return base == nullptr;
        }

        void clearValue()
        {
            base[index] = 0;
        }

        void setValue(T value)
        {
            base[index] = value;
        }

        T getValue() const
        {
            return base[index];
        }

        int getIndex() const
        {
            return index;
        }
    };


    // Functor that returns next location which is not occupied yet on the map.
    // If no location found, returns Location<>{ nullptr, 0 }.
    // This is the simplest NextLocation functor is given as an example.
    class NextLocation
    {
        void* map;

    public:

        NextLocation(void* map = nullptr)
            : map{ map }
        {
        }

        Location<> operator()() const
        {
            return Location<>{ nullptr, 0 };
        }
    };


    // Functor that returns next candidate for a specified location on the map.
    // If no candidate found, returns 0.
    // This is the simplest NextCandidate functor is given as an example.
    class NextCandidate
    {
        void* map;

    public:

        NextCandidate(void* map = nullptr)
            : map{ map }
        {
        }

        int operator()(Location<> location)
        {
            UNUSED(location)
            return 0;
        }
    };

} // end namespace

#endif