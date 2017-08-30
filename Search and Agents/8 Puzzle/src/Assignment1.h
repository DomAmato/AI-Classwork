#pragma once
#include "Heuristics.h"

class Assignment1 {

public:
	//default constructor, just sets the pc frequency
	Assignment1();

	//breadth first algorithm
	tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> breadthFirst(const Puzzle initialState, SuccessorOrder order);
	//depth first search using iteration
	tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> depthFirst(const Puzzle initialState, SuccessorOrder order);

	//depth first search using recursion, causes stack overflows...
	tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> depthFirstR(Puzzle initialState, SuccessorOrder order);
	Puzzle RecursiveDepthSearch(Puzzle currentBoard, unsigned __int32 level, unsigned __int32 & nodes, unordered_set<unsigned __int32>& prevStates, SuccessorOrder order);

	//uses recursion to perform limited depth first searches
	tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> iterativeDeepening(Puzzle initialState, SuccessorOrder order);
	Puzzle * RecursiveDepthLimitedSearch(Puzzle * currentBoard, unsigned __int32 limit, unsigned __int32 & nodes, unsigned __int32 & queueSize, unordered_set<unsigned __int32>& prevStates, SuccessorOrder order);

	//Priority queue based on the cost of each move
	tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> uniformCost(Puzzle initialState, SuccessorOrder order);
	//Follows the Misplaced Tile heuristic with no cost checking
	tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> bestFirst(Puzzle initialState, SuccessorOrder order);
	//Misplaced Tiles Heuristic
	tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> aStar1(Puzzle initialState, SuccessorOrder order);
	//Manhatten Distance Heuristic with A*
	tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> aStar2(Puzzle initialState, SuccessorOrder order);
	//Grad Requirement to make our own heuristic
	tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> aStar3(Puzzle initialState, SuccessorOrder order);

	unsigned __int32 calculateCost(const Puzzle * completedPuzzle);

private:
	//the frequency and timers are only for efficency measurement 
	double pcFrequncy;
	LARGE_INTEGER startTimer;
	LARGE_INTEGER endTimer;

protected:

};

///we have to create a comparison object for the priority queue because by default it will order things
///from greatest to least and we want the opposite, i.e. things with the least cost at the front
class pairComparison
{
public:
	pairComparison() {}
	//we want the smallest value at the top of the queue because that means we are closer to the answer (ideally)
	bool operator() (const pair<int, Puzzle> & lhs, const pair<int, Puzzle>&rhs) const
	{
		return (lhs.first > rhs.first);
	}
};

