#include "Board.h"

/**
Default Constructor
initialize the board, both player scores, and the turn order
*/
Board::Board()
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			_board[i][j] = 0;
		}
	}
	whiteScore = 0;
	blackScore = 0;
	currentPlayer = false;
	recalcScore = false;
}

/**
Calculates the score from the current board state

starts from the bottom left and checks to the top right
each location is checked for a similar color tile to the right,
up and right, up, and up and left. This is to prevent double
counting the score.
*/
void Board::calculateScore()
{
	whiteScore = 0;
	blackScore = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (_board[j][i] == 1) {
				whiteScore += (i < 8 && _board[j][i] == _board[j][i + 1]) ? 2 : 0;
				whiteScore += (j < 8 && _board[j][i] == _board[j + 1][i]) ? 2 : 0;
				whiteScore += (j < 8 && i < 8 && _board[j][i] == _board[j + 1][i + 1]) ? 1 : 0;
				whiteScore += (j > 0 && i < 8 && _board[j][i] == _board[j - 1][i + 1]) ? 1 : 0;
			}
			else if (_board[j][i] == 2) {
				blackScore += (i < 8 && _board[j][i] == _board[j][i + 1]) ? 2 : 0;
				blackScore += (j < 8 && _board[j][i] == _board[j + 1][i]) ? 2 : 0;
				blackScore += (j < 8 && i < 8 && _board[j][i] == _board[j + 1][i + 1]) ? 1 : 0;
				blackScore += (j > 0 && i < 8 && _board[j][i] == _board[j - 1][i + 1]) ? 1 : 0;
			}
		}
	}
}

/**
Check if the board has any empty spaces

Since the tiles slide down we only need to check the top
row for an open slot.

@return boolean whether or not there is an open space
*/
bool Board::isFull()
{
	for (int i = 0; i < 9; i++) {
		if (_board[i][8] == 0) {
			return false;
		}
	}
	return true;
}

/**
Get the value at the current location on the board

@param The column of the space
@param The row of the space
@return value at the location from the parameters
*/
unsigned __int8 Board::getSpace(unsigned __int8 col, unsigned __int8 row)
{
	return _board[col][row];
}


/**
Get the scores of both players together as a pair

recalcualtes the score if the board state has changed since the last time
it was requested

@return a pair of the black and white scores respectively
*/
pair<unsigned __int16, unsigned __int16> Board::getScore()
{
	if (recalcScore) {
		calculateScore();
		recalcScore = false;
	}
	return pair<unsigned __int16, unsigned __int16>(blackScore, whiteScore);
}

/**
Places a tile into the column and calculates what row to place it into
changes the active player on success and flags a score recalculation

@param The column to place the tile in
@return a boolean of whether the operation completed successfully,
		fails if the column is full
*/
bool Board::placeInColumn(unsigned __int8 column)
{
	if (column > 8 || _board[column][8] != 0) {
		return false;
	}

	recalcScore = true;

	for (int i = 0; i < 9; i++) {
		if (_board[column][i] == 0) {
			_board[column][i] = currentPlayer ? 2 : 1;
			currentPlayer = !currentPlayer;
			return true;
		}
	}

	return false;
}

/**
Clear the board back to an inital phase
resets the scores and the player turns
*/
void Board::clearBoard()
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			_board[i][j] = 0;
		}
	}
	whiteScore = 0;
	blackScore = 0;
	currentPlayer = false;
	recalcScore = false;
}

/**
Overloaded operator
assigns the values of the passed in board to this one

@param the board to assign from
@return the pointer to this board
*/
Board & Board::operator=(Board const & puzzle)
{
	this->_board = puzzle.getBoard();
	return *this;
}
