#include "Puzzle.h"


/**
Default constructor
only set the parent to null
its probably best we don't initalize the board to prevent
the board from being seen as a valid puzzle
*/
Puzzle::Puzzle() {
	_parent = NULL;
}

/**
The full constructor that sets the board and points it to 
the parent node

@param the board to set for this puzzle
@param a pointer to the parent puzzle this will point to
*/
Puzzle::Puzzle(const array<array<char, 3>, 3> board, const Puzzle * puzzle)
{
	_board = board;
	_parent = puzzle;
}

/**
not every puzzle needs a parent node so just
initialize the board

@param the board to set for this puzzle
*/
Puzzle::Puzzle(array<array<char, 3>, 3> board)
{
	_board = board;
	_parent = NULL;
}


/**
In the case we don't construct from a pointer we want a
reference not a copy. The parent should stay in memory then

@param the board to set for this puzzle
@param a reference to the parent puzzle this will point to
*/
Puzzle::Puzzle(array<array<char, 3>, 3> board, Puzzle & parent) {
	_board = board;
	_parent = &parent;
}

/**
Get the current node this puzzle points to

@return The current puzzles parent pointer
which can be null
*/
const Puzzle * Puzzle::getParent() const {
	return _parent;
}

/**
Make the parent pointer point to a new node

@param the new puzzle to point at
*/
void Puzzle::setParent(const Puzzle * parent)
{
	_parent = parent;
}

/**
Set the current puzzle to a new board state

@param a 3x3 array to replace the current board
*/
void Puzzle::setBoard(array<array<char, 3>, 3> board)
{
	_board = board;
}

/**
gets the puzzles board as a 3x3 array

@return the puzzles board
*/
const array<array<char, 3>, 3> Puzzle::getBoard() const {
	return _board;
}


/**
Even though just comparing the arrays of each puzzle
would work for clarity and sanity sake lets test
each slot

@return if the puzzle board is in the goal state
*/
const bool Puzzle::isGoalState() const
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (_board[i][j] != GOAL[i][j]) {
				return false;
			}
		}
	}
	return true;
}

/**
From the current board state a possible 4 children can be spawned
this works by finding the slidable tile (0) location and checking
what possible children exist. It ignores previously spawned children
that are contained in the set that is passed in

@param A set with all previously visited nodes
@param The order in which to spawn successors
@return a vector which contains the children
*/
vector<Puzzle> Puzzle::getSuccesors(unordered_set<unsigned __int32> prevStates, SuccessorOrder order) const
{
	vector<Puzzle> possibleBoards;
	int i = 0;
	int j = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (_board[i][j] == 0) {
				goto stop;
			}
		}
	}
stop:
	for (auto dir : order.getOrderList()) {
		switch (dir) {
		case 0:
			if (i != 0) { //up
				Puzzle mutatedBoard = Puzzle(_board, this);
				mutatedBoard[i][j] = mutatedBoard[i - 1][j];
				mutatedBoard[i - 1][j] = 0;
				if (prevStates.find(mutatedBoard.toInt()) == prevStates.end()) {
					possibleBoards.push_back(mutatedBoard);
				}
			}
			break;
		case 1:
			if (j != 0) { //left
				Puzzle mutatedBoard = Puzzle(_board, this);
				mutatedBoard[i][j] = mutatedBoard[i][j - 1];
				mutatedBoard[i][j - 1] = 0;
				if (prevStates.find(mutatedBoard.toInt()) == prevStates.end()) {
					possibleBoards.push_back(mutatedBoard);
				}
			}
			break;
		case 2:
			if (i != 2) { //down
				Puzzle mutatedBoard = Puzzle(_board, this);
				mutatedBoard[i][j] = mutatedBoard[i + 1][j];
				mutatedBoard[i + 1][j] = 0;
				if (prevStates.find(mutatedBoard.toInt()) == prevStates.end()) {
					possibleBoards.push_back(mutatedBoard);
				}
			}
			break;
		case 3:
			if (j != 2) { //right
				Puzzle mutatedBoard = Puzzle(_board, this);
				mutatedBoard[i][j] = mutatedBoard[i][j + 1];
				mutatedBoard[i][j + 1] = 0;
				if (prevStates.find(mutatedBoard.toInt()) == prevStates.end()) {
					possibleBoards.push_back(mutatedBoard);
				}
			}
			break;
		}
	}
	return possibleBoards;
}

/**
From the current board state a possible 4 children can be spawned
this works by finding the slidable tile (0) location and checking
what possible children exist. It ignores previously spawned children
that are contained in the set that is passed in
returns a pair that includes the cost of moving the tile

@param A set with all previously visited nodes
@param The order in which to spawn successors
@return a vector of pairs which contain the cost and
the children
*/
vector<pair<unsigned char, Puzzle>> Puzzle::getSuccesorsWithCost(unordered_set<unsigned __int32> prevStates, SuccessorOrder order) const
{
	vector<pair<unsigned char, Puzzle>> possibleBoards;
	int i = 0;
	int j = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (_board[i][j] == 0) {
				goto stop;
			}
		}
	}
stop:
	for (auto dir : order.getOrderList()) {
		switch (dir) {
		case 0:
			if (i != 0) { //up
				Puzzle mutatedBoard = Puzzle(_board, this);
				mutatedBoard[i][j] = mutatedBoard[i - 1][j];
				mutatedBoard[i - 1][j] = 0;
				if (prevStates.find(mutatedBoard.toInt()) == prevStates.end()) {
					possibleBoards.push_back(pair<unsigned char, Puzzle>(mutatedBoard[i][j], mutatedBoard));
				}
			}
			break;
		case 1:
			if (j != 0) { //left
				Puzzle mutatedBoard = Puzzle(_board, this);
				mutatedBoard[i][j] = mutatedBoard[i][j - 1];
				mutatedBoard[i][j - 1] = 0;
				if (prevStates.find(mutatedBoard.toInt()) == prevStates.end()) {
					possibleBoards.push_back(pair<unsigned char, Puzzle>(mutatedBoard[i][j], mutatedBoard));
				}
			}
			break;
		case 2:
			if (i != 2) { //down
				Puzzle mutatedBoard = Puzzle(_board, this);
				mutatedBoard[i][j] = mutatedBoard[i + 1][j];
				mutatedBoard[i + 1][j] = 0;
				if (prevStates.find(mutatedBoard.toInt()) == prevStates.end()) {
					possibleBoards.push_back(pair<unsigned char, Puzzle>(mutatedBoard[i][j], mutatedBoard));
				}
			}
			break;
		case 3:
			if (j != 2) { //right
				Puzzle mutatedBoard = Puzzle(_board, this);
				mutatedBoard[i][j] = mutatedBoard[i][j + 1];
				mutatedBoard[i][j + 1] = 0;
				if (prevStates.find(mutatedBoard.toInt()) == prevStates.end()) {
					possibleBoards.push_back(pair<unsigned char, Puzzle>(mutatedBoard[i][j], mutatedBoard));
				}
			}
			break;
		}
	}
	return possibleBoards;
}

/**
Overload the operator to access the underlying multi
dimensional array

@param position in the array
@return the array at the position given
*/
array<char, 3>& Puzzle::operator[](int pos) {
	return _board[pos];
}

/**
Overload the operator to access the underlying multi
dimensional array

@param position in the array
@return the array at the position given
*/
const array<char, 3>& Puzzle::operator[](int pos) const {
	return _board[pos];
}

/**
Overload the operator to handle the assignment of
pointers 

@param the puzzle we are assigning
@return the new puzzle, a pointer to this
*/
Puzzle& Puzzle::operator=(Puzzle const &puzzle) {
	if (this != &puzzle) {
		//delete _parent; // free resource;
		if (_parent != puzzle.getParent()) {
			this->setParent(puzzle.getParent());
		}
		this->setBoard(puzzle.getBoard());
	}
	return *this;
}

/**
Map the underlying puzzle into an integer.
minimum size is 32 bits since there are
362880 total states and the board is saved as
"flattened" int so [0][0] is the first digit

@return 32bit integer that represents the array
*/
unsigned __int32 Puzzle::toInt() {
	string convert = "";
	convert.append(to_string(_board[0][0]));
	convert.append(to_string(_board[0][1]));
	convert.append(to_string(_board[0][2]));
	convert.append(to_string(_board[1][0]));
	convert.append(to_string(_board[1][1]));
	convert.append(to_string(_board[1][2]));
	convert.append(to_string(_board[2][0]));
	convert.append(to_string(_board[2][1]));
	convert.append(to_string(_board[2][2]));
	return stoi(convert);
}

/**
Map the underlying puzzle into an integer.
minimum size is 32 bits since there are
362880 total states and the board is saved as
"flattened" int so [0][0] is the first digit

@return 32bit integer that represents the array
*/
const unsigned __int32 Puzzle::toInt() const
{
	string convert = "";
	convert.append(to_string(_board[0][0]));
	convert.append(to_string(_board[0][1]));
	convert.append(to_string(_board[0][2]));
	convert.append(to_string(_board[1][0]));
	convert.append(to_string(_board[1][1]));
	convert.append(to_string(_board[1][2]));
	convert.append(to_string(_board[2][0]));
	convert.append(to_string(_board[2][1]));
	convert.append(to_string(_board[2][2]));
	return stoi(convert);
}

