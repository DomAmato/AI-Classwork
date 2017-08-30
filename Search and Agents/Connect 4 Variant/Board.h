#pragma once
#include "Dependencies.h"


///this implmentation is pretty sparce with no pointers or links to
///other board states
class Board {
private:
	//lets use a char, a boolean doesnt cover the 3 states (empty, white, black)
	array<array<char, 9>, 9> _board;

	//it might be possible to score over 255 and its not like the memory loss is
	//significant here
	unsigned __int16 whiteScore;
	unsigned __int16 blackScore;

	//which player is placing a tile
	bool currentPlayer;
	//only recalculate the score if the score has changed
	bool recalcScore;

	//funciton to calculate the score when we need it
	void calculateScore();
public:
	//empty constructor
	Board();
	//is every spot on the board taken
	bool isFull();
	//get the value of the board coordinates given
	unsigned __int8 getSpace(unsigned __int8 col, unsigned __int8 row);
	//get the score of both players in a pair
	pair<unsigned __int16, unsigned __int16> getScore();
	//place a tile in the column and at the lowest empty row
	bool placeInColumn(unsigned __int8 column);
	//set everything back to 0
	void clearBoard();
	
	/**
	Get the multidimensional array used by the board to store the state

	@return 2d array of the current board state
	*/
	const array<array<char, 9>, 9> getBoard() const { return _board; }

	/**
	The heuristic for the minimax which is just the difference of the
	scores

	@return the score differential
	*/
	__int8 getHeuristic() const { return blackScore - whiteScore; }

	//overload the assignment operator
	Board& operator=(Board const &puzzle);
	
};

/**
Overloaded operator
Compares the boards of each for equivilency

@param the board to compare against
@param the board to compare to
@return boolean of whether the boards are equal
*/
inline bool operator==(const Board & lval, const Board & rval) {
	return lval.getBoard() == rval.getBoard();
};