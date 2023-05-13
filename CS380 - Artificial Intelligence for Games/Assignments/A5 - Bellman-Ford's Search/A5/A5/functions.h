/*!*****************************************************************************
\file functions.h
\author Vadim Surov, Goh Wei Zhe
\par DP email: vsurov\@digipen.edu, weizhe.goh\@digipen.edu
\par Course: CS380
\par Section: B
\par Programming Assignment 5
\date 06-13-2021
\brief
This file has declarations and definitions that are required for submission
*******************************************************************************/

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

        /*!*********************************************************************
        \brief
        Constructor for class BellmanFord

        \param matrix
        A matrix array of integers

        \return
        None.
        ***********************************************************************/
        BellmanFord(int* matrix = nullptr)
            : matrix{ matrix }, distance{ nullptr }, predecessor{ nullptr }
        {
           this->distance = new int[SIZE];
           this->predecessor = new int[SIZE];
         }

        /*!*********************************************************************
        \brief
        Destructor for class BellmanFord

        \param 
        None. 

        \return
        None.
        ***********************************************************************/
        ~BellmanFord()
        {
           delete[] distance;
           delete[] predecessor;
        }

        /*!*********************************************************************
        \brief
        Function to run Bellman-Ford's Algorithm 

        \param starting
        The starting position of the array

        \return
        Returns tue if cycles are found, else return false if negative cycles
        ***********************************************************************/
        bool run(int starting = 0)
        {
            for (int i = 0; i < SIZE; ++i)
            {
                //set distance all to infinite
                //set predecessor all to null
                this->distance[i] = inf;
                this->predecessor[i] = null;
                
                //set starting node distance to 0
                if(starting == i)
                    this->distance[starting] = 0;

            }
            
            // For each node, apply relaxation for all the edges
            for (int k = 0; k < SIZE-1; k++)
            {
                //number of relaxation
                int counter = 0;
                
                for (int i = 0; i < SIZE; i++)
                {
                    for (int j = 0; j < SIZE; j++)
                    {
                        if ((i != j) && (this->distance[i] != inf) &&
                            (this->matrix[(i*SIZE) + j] != inf))
                        {
                            int new_distance = 
                                this->distance[i] + this->matrix[(i*SIZE) + j];

                            if (new_distance < this->distance[j])
                            {
                                //Relaxation 
                                this->distance[j] = new_distance;
                                this->predecessor[j] = i;
                                counter++;
                            }
                        }
                    }
                }

                // Stop when no more relaxation
                // There is no negative cycles 
                if (counter == 0)
                    return true;
            }

             // Run algorithm a second time to detect which nodes are part
             // of a negative cycle. A negative cycle has occurred if we
             // can find a better path beyond the optimal solution.
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    if ((i != j) && (this->distance[i] != inf) && 
                        (this->matrix[(i*SIZE)+ j] != inf)
                        && ((this->distance[i] + this->matrix[(i*SIZE) + j])
                            < this->distance[j]))
                        return false;
                }
            }
            
            // There is no negative cycles
            return true;
        }

        /*!*********************************************************************
        \brief
        Function to reconstruct the shortest path from starting point to target

        \param target
        Target to get the path from

        \return
        Returns an array of int values from starting point to target.
        ***********************************************************************/
        std::vector<int> getPath(int target)
        {
            std::vector<int> path{};

            for (int i = target; predecessor[i] != null; i = predecessor[i])
                path.push_back(i);

            std::reverse(path.begin(), path.end());

            return path;
        }

        /*!*********************************************************************
        \brief
        Function to create a route (step-by-step description) of the shortest 
        path from start to end with cost

        \param target
        Target to get the path from

        \return
        Returns a vector of int array values from starting point to target.
        ***********************************************************************/
        std::vector<std::array<int, 3>> getRoute(int target)
        {
            std::vector<std::array<int, 3>> route{};
            
            for (int i = target; predecessor[i] != null; i = predecessor[i])
                route.push_back({ this->predecessor[i], i, this->distance[i] });

            for (unsigned j = 0; j < route.size() - 1; ++j)
                route[j][2] -= route[j + 1][2];

            std::reverse(route.begin(), route.end());
          
            return route;
        }

        /*!*********************************************************************
        \brief
        Serialization. An overloading insertion operator function that takes
        and return a stream object.

        \param os
        Output stream to perform output.

        \param rhs
        Right hand side object.

        \return
        Returns the output through ostream.
        ***********************************************************************/
        friend std::ostream& operator<<(std::ostream& os,const BellmanFord& rhs)
        { 
            Print(os, &rhs);
            return os;
        }

        /*!*********************************************************************
        \brief
        Function to print output.

        \param os
        Output stream to perform output.

        \param rhs
        Right hand side object.

        \return
        None.
        ***********************************************************************/
        static void Print(std::ostream& os, const BellmanFord* rhs)
        {
            os << "[";

            for (int i = 0; i < SIZE; i++)
            {
                if ((rhs->distance[i] == inf) && (i != SIZE - 1))
                    os << "inf,";
                else if ((rhs->distance[i] == inf) && (i == SIZE - 1))
                    os << "inf";
                else if (i != SIZE-1)
                    os << rhs->distance[i] << ",";
                else
                    os << rhs->distance[i];
            }

            os << "] [";
            
            for (int i = 0; i < SIZE; i++)
            {
                if ((rhs->predecessor[i] == null) && (i != SIZE - 1))
                    os << "null,";
                else if ((rhs->predecessor[i] == null) && (i == SIZE - 1))
                    os << "null";
                else if (i != SIZE-1)
                    os << rhs->predecessor[i] << ",";
                else
                    os << rhs->predecessor[i];
            }

            os << "]";
        }
    };

} // end namespace

#endif