#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <cstring> // memcpy
#include <limits.h>

#include "data.h"

#define UNUSED(x) (void)x;

// Class that defines the game specific data and code
class Grid
{
	static const int width = 3;
	static const int height = 3;

	char squares[width * height];

public:
	// The game marks/pieces
	static const char x = 'x';
	static const char o = 'o';
	static const char _ = ' ';


	Grid(char* squares = nullptr)
		: squares{ _, _, _, _, _, _, _, _, _ }
	{
		if (squares)
			for (int i = 0; i < height * width; ++i)
				this->squares[i] = squares[i];
	}

	Grid(const Grid& rhs)
	{
		this->operator=(rhs);
	}

	void operator=(const Grid& rhs)
	{
		std::memcpy(squares, rhs.squares, height * width * sizeof(char));
	}

	void set(int i, char c)
	{
		squares[i] = c;
	}

	void clear(int i)
	{
		squares[i] = _;
	}

	// Returns a list of indices of all empty squares in the grid.
	// For example for grid [' ',' ','o',' ','x',' ',' ',' ',' ']
	// the function returns [0,1,3,5,6,7,8]
	std::vector<int> emptyIndices() const
	{
		std::vector<int> array = {};

		// Your code ...

		return array;
	}

	// Returns true if the grid has a winning configuration for the player
	bool winning(char player)
	{
		UNUSED(player);

		// Your code ...

		return false;
	}

	friend std::ostream& operator<<(std::ostream& os, const Grid& rhs)
	{
		UNUSED(rhs);

		// Your code ...

		return os;
	}
};

namespace AI
{
	// A node of the game tree
	template<typename T>
	class Move 
	{
		T grid;			// Result of a move: new state of the game grid			
		int score;		// Score of the move			
		std::vector<Move*>* next;  // All possible next moves
		int bestMove;   // Index of the first move in member next that has the best score 
		int spotIndex;  // Index of the move's spot (used for a visualization)

	public:
		Move(T grid = {}, int score = 0, std::vector<Move*>* next = new std::vector<Move*>{}, int bestMove = -1)
			: grid{ grid }, score{ score }, next{ next }, bestMove{ bestMove }, spotIndex{ -1 }
		{
		}

		~Move() 
		{

			// Your code ...

		}

		Move& at(int i)
		{
			UNUSED(i);

			// Your code ...

			return *(new Move{}); // Just for test
		}

		int getScore() const
		{
			return score;
		}

		void setSpotIndex(int i)
		{
			spotIndex = i;
		}

		friend std::ostream& operator<<(std::ostream& os, const Move& rhs)
		{
			os << rhs.grid << std::endl;
			os << rhs.score << std::endl;
			os << rhs.next->size() << std::endl;
			os << rhs.bestMove << std::endl;
			return os;
		}
	};


	// Find the best next move for maximizer.
	// For the initial call, set the player parameter as maximizer. 
	// Warning! Found solution (sequence of moves) is not necessary shortest.
	template<typename T>
	Move<T>* minimax(T grid, char player, char maximizer, char minimizer)
	{
		UNUSED(grid);
		UNUSED(player);
		UNUSED(maximizer);
		UNUSED(minimizer);

		// Your code ...

		// Return the move
		return new Move<T>{ };
	}

} // end namespace

#endif