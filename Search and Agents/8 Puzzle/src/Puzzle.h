#pragma once
#include "Dependencies.h"

#ifndef PUZZLE_H
#define PUZZLE_H

///this is all the possible successor orders 
/// 0 = up, 1 = left, 2 = down, and 3 = right
enum Orders {
	//we have to omit the leading 0 for the first mutations as it causes a problem
	A = 123,
	B = 132,
	C = 213,
	D = 231,
	E = 321,
	F = 312,
	G = 1023,
	H = 1032,
	I = 1203,
	J = 1230,
	K = 1302,
	L = 1320,
	M = 2013,
	N = 2031,
	O = 2103,
	P = 2130,
	Q = 2301,
	R = 2310,
	S = 3012,
	T = 3021,
	U = 3102,
	V = 3120,
	W = 3201,
	X = 3210
};

///Just a simple struct to keep track of the order to
///create successor nodes from, uses the enum with a helper function
struct SuccessorOrder {
	//order E seemed to execute quickest for DFS on average
	//coincidently its also clockwise
	Orders order = Orders::E;

	//a vector of the order from the enum
	vector<__int8> getOrderList() const {
		vector<__int8> theOrder;
		string digit = to_string(order);
		if (digit.size() < 4) {
			theOrder.push_back(0);
		}
		for (auto c : digit) {
			//to convert a char to an int we have to convert the ascii code
			theOrder.push_back(int(c - '0'));
		}
		return theOrder;
	}

	void setOrder(Orders o) {
		order = o;
	}
};

const static SuccessorOrder successorOrder;

///Puzzle class that has a pointer to the parent node
///that spawned this board. Needed for reverse tree
///traversal since the final goal node is the leaf
class Puzzle {
private:
	const Puzzle * _parent;
	array<array<char, 3>, 3> _board;
public:
	//default constructor
	Puzzle();
	//full constructor with pointer to parent node
	Puzzle(const array<array<char, 3>, 3> board, const Puzzle * puzzle);
	//similar only we pass a reference instead of a pointer
	Puzzle(array<array<char, 3>, 3> board, Puzzle & parent);
	//create a puzzle with null parent
	Puzzle(array<array<char, 3>, 3> board);

	//return the pointer of the parent node
	const Puzzle * getParent() const;
	//set the parent to the pointer supplied
	void setParent(const Puzzle * parent);
	//sets the board to a new one
	void setBoard(array<array<char, 3>, 3> board);
	//get the current board
	const array<array<char, 3>, 3> getBoard() const;

	//is this puzzle the same as the goal state
	const bool isGoalState() const;

	//convert the puzzle board to an integer
	unsigned __int32 toInt();
	const unsigned __int32 toInt() const;

	//get the successors, the default order is the default successor order value E 
	vector<Puzzle> getSuccesors(unordered_set<unsigned __int32> prevStates, SuccessorOrder order = successorOrder) const;
	vector<pair<unsigned char, Puzzle>> getSuccesorsWithCost(unordered_set<unsigned __int32> prevStates, SuccessorOrder order = successorOrder) const;

	//operator overloader to access the multi-dim array in the typical way
	array<char, 3>& operator[](int pos);
	const array<char, 3>& operator[](int pos) const;

	//overlaod the assignment operator because we have a pointer
	Puzzle& operator=(Puzzle const &puzzle);
};

//overload the operator for debugging and printing purposes
inline ostream& operator<<(ostream& os, const Puzzle & puzzle) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			os << std::to_string(puzzle[i][j]);
		}
		os << endl;
	}
	return os << endl;
};

inline ostream& operator<<(ostream& os, const Puzzle * puzzle) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			os << std::to_string(puzzle->getBoard()[i][j]);
		}
		os << endl;
	}
	return os << endl;
};

//we dont want to compare pointers so compare the boards
//should probably compare the parents to see if they point to the same address
inline bool operator==(const Puzzle & lval, const Puzzle & rval) {
	return lval.getBoard() == rval.getBoard();
};

inline bool operator!=(const Puzzle & lval, const Puzzle & rval) {
	return lval.getBoard() != rval.getBoard();
};

#endif

//Our puzzle states
static const Puzzle GOAL = Puzzle({ { { 1, 2, 3 },{ 8, 0, 4 },{ 7, 6, 5 } } });
static const Puzzle EASY = Puzzle({ { { 1, 3, 4 },{ 8, 6, 2 },{ 7, 0, 5 } } });
static const Puzzle MEDIUM = Puzzle({ { { 2, 8, 1 },{ 0, 4, 3 },{ 7, 6, 5 } } });
static const Puzzle HARD = Puzzle({ { { 5, 6, 7 },{ 4, 0, 8 },{ 3, 2, 1 } } });