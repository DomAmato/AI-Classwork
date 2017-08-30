#pragma once
#include "Puzzle.h"

///class to handle all our heuristics
class Heuristics {
public:
	/**
	Heuristic which gives us the total number of misplaced tiles

	@param the puzzle to evaluate
	@return an 8 bit integer of the number of misplaced tiles, the max number can be 9
	*/
	static unsigned __int8 heurtisticMisplacedTiles(const Puzzle board) {
		__int8 misplacedTiles = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (board[i][j] != GOAL[i][j]) {
					misplacedTiles++;
				}
			}
		}
		return misplacedTiles;
	}

	/**
	Heuristic which gives us the total number of moves required for every tile to reach its destination

	@param the puzzle to evaluate
	@return an 8 bit integer of the number of misplaced tiles, the max number has to be less than 9*4 (9 tiles, max distance of 4)
	*/
	static unsigned __int8 heurtisticManhatten(const Puzzle board) {
		unsigned __int8 totalManhatten = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (board[i][j] != GOAL[i][j]) {
					__int8 totalDist = 0;
					for (int x = 0; x < 3; x++) {
						for (int y = 0; y < 3; y++) {
							if (board[i][j] == GOAL[x][y]) {
								totalDist = abs(i - x) + abs(j - y);
								goto outter;
							}
						}
					}
					outter:
					totalManhatten+=totalDist;
				}
			}
		}
		return totalManhatten;
	}

	/**
	Heuristic which is just multiplying misplaced tiles by manhatten distance.
	By multiplying we have a greater variation in the values returned and thus
	the search algorithm should be able to be more directed

	@param the puzzle to evaluate
	@return an 16 bit integer of the number of misplaced tiles, the max number can be 9
	*/
	static unsigned __int16 heurtisticHybrid(const Puzzle board) {
		return heurtisticMisplacedTiles(board) * heurtisticManhatten(board);
	}
};