#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "data.h"

#define UNUSED(x) (void)x;

namespace AI 
{

    // Domain specific functor that returns adjacent nodes
    class GetMapAdjacents : public GetAdjacents
    {
        int* map; // the map with integers where 0 means an empty cell
        int size; // width and hight of the map in elements

    public:

        GetMapAdjacents(int* map = nullptr, int size = 0)
            : GetAdjacents(), map{ map }, size{ size }
        {
        }

        std::vector<AI::Node*> operator()(Key key)
        {
            UNUSED(key)

            std::vector<AI::Node*> list = {};

            // Find and return all empty adjacent cells

            return list;
        }
    };

    class Dijkstras
    {
        GetAdjacents* pGetAdjacents;

    public:

        Dijkstras(GetAdjacents* pGetAdjacents)
            : pGetAdjacents(pGetAdjacents)
        {
        }

        // starting and target are arrays of 2 elements [j, i] that define positions on the map
        std::vector<char> run(Key starting, Key target)
        {
            UNUSED(starting)
            UNUSED(target)

            Node * pCurrent = nullptr;
            
            // Implement the search

            return getPath(pCurrent);
        }

    private:

        std::vector<char> getPath(Node* pNode)
        {
            UNUSED(pNode)

            std::vector<char> a{};
            
            // Trace back to return a vector of moves (.info)

            return a;
        }
    };

} // end namespace

#endif