/******************************************************************************/
/*!
\file:		ALGraph.h
\author:    Goh Wei Zhe, weizhe.goh, 440000119
\par email: weizhe.goh\@digipen.edu
\date:		March 28, 2021
\brief		This file contains the declarations needed to construct a graph and 
            implementing Dijkstra's algorithm.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

//---------------------------------------------------------------------------
#ifndef ALGRAPH_H
#define ALGRAPH_H
//---------------------------------------------------------------------------
#include <vector>
#include <algorithm>
#include <queue>
#include <list>
#include <iostream>

struct DijkstraInfo
{
  unsigned cost;
  std::vector<unsigned> path;
};

struct AdjacencyInfo
{
  unsigned id;
  unsigned weight;
};

typedef std::vector<std::vector<AdjacencyInfo> > ALIST;

class ALGraph   
{
  public:
    ALGraph(unsigned size);
    ~ALGraph(void);
    void AddDEdge(unsigned source, unsigned destination, unsigned weight);
    void AddUEdge(unsigned node1, unsigned node2, unsigned weight);

    std::vector<DijkstraInfo> Dijkstra(unsigned start_node) const;
    ALIST GetAList(void) const;
        
  private:
    
    // An EXAMPLE of some other classes you may want to create and 
    // implement in ALGraph.cpp
    class GNode; 
    class GEdge;
    
    struct AdjInfo
    {
     // GNode *node;
      unsigned id;
      unsigned weight;
      
      //unsigned cost;
      //AdjInfo();
      bool operator<(const AdjInfo& rhs) const;
      bool operator>(const AdjInfo& rhs) const;
    };
    
    // Other private fields and methods
    struct NodeInfo
    {
        unsigned id;
        unsigned cost;
        unsigned prev;
        
        bool operator < (const NodeInfo& rhs) const;
        bool operator == (const unsigned& value) const;
    };
    
    unsigned size_;
    std::vector<std::vector<AdjInfo>> graph;
    
    unsigned extract_min(std::vector<AdjacencyInfo>& pq) const;
};

#endif
