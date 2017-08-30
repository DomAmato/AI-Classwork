#pragma once
#include "Board.h"

class Assignment2 {
private:
	//the frequency of the cpu for timing purposes
	double pcFrequency;

	/**
	Generic Minimax Recursive funciton

	@param Whether we are maximizing or minimizing at this depth
	@param the current depth of the search
	@param the "0" depth or the root node
	@param the board to mutate from (also could be previously mutated but higher recursive calls)
	@return the column which to place the tile into
	*/
	__int8 miniMax(bool maxi, __int8 depth, __int8 topDepth, Board mutatedBoard) {
		//did we reach the depth or if the board is full, return
		if (depth == 0 || mutatedBoard.isFull()) {
			auto score = mutatedBoard.getScore();
			return score.first - score.second;
		}

		__int8 bestVal = maxi ? MINCHAR : MAXCHAR; //highest signed value and the lowest signed value
		__int8 optimalLoc = -1; //set the optimal location to an invalid location to start
		__int8 prevVal = bestVal; //check if things changed
		for (int i = 0; i < 9; i++) {
			Board newBoard = mutatedBoard;
			if (newBoard.placeInColumn(i)) { //we can't place into that column if its full
				//ternary operation, are we minimizing or maximizing
				bestVal = maxi ?
					max(bestVal, miniMax(!maxi, depth - 1, topDepth, newBoard)) :
					min(bestVal, miniMax(!maxi, depth - 1, topDepth, newBoard));
				//this only exists to make sure the value returned at the root is not the
				//minimax value but the column that the best value came from
				if (depth == topDepth) {
					if (prevVal != bestVal) {
						optimalLoc = i;
						prevVal = bestVal;
					}
				}
			}
		}

		//are we at the root level, return the column
		if (depth == topDepth) {
			return optimalLoc;
		}
		//return the best value
		return bestVal;
	}

	/**
	Naive Alpha Beta Pruning Recursive funciton

	@param Whether we are maximizing or minimizing at this depth
	@param the current depth of the search
	@param the "0" depth or the root node
	@param the alpha value
	@param the beta value
	@param the board to mutate from (also could be previously mutated but higher recursive calls)
	@return the column which to place the tile into
	*/
	__int8 miniMaxAB(bool maxi, __int8 depth, __int8 topDepth, __int8 alpha, __int8 beta, Board mutatedBoard) {
		//did we reach the depth or if the board is full, return
		if (depth == 0 || mutatedBoard.isFull()) {
			auto score = mutatedBoard.getScore();
			return score.first - score.second;
		}

		__int8 bestVal = maxi ? MINCHAR : MAXCHAR; //highest signed value and the lowest signed value
		__int8 optimalLoc = -1; //set the optimal location to an invalid location to start
		__int8 prevVal = bestVal; //check if things changed
		for (int i = 0; i < 9; i++) {
			Board newBoard = mutatedBoard;
			if (newBoard.placeInColumn(i)) { //we can't place into that column if its full
				if (maxi) { //instead of a ternary operator we need to do a few operations
					bestVal = max(bestVal, miniMaxAB(!maxi, depth - 1, topDepth, alpha, beta, newBoard));
					//check if the alpha value needs to change
					alpha = max(alpha, bestVal);
					if (beta <= alpha) {
						//is the beta less than the alpha, exit the loop
						goto endLoop;
					}
				}
				else {
					bestVal = min(bestVal, miniMaxAB(!maxi, depth - 1, topDepth, alpha, beta, newBoard));
					//does the beta need to change
					beta = min(beta, bestVal);
					if (beta <= alpha) {
						//is the beta less than the alpha, exit the loop
						goto endLoop;
					}
				}
				//this only exists to make sure the value returned at the root is not the
				//minimax value but the column that the best value came from
				if (depth == topDepth) {
					if (prevVal != bestVal) {
						optimalLoc = i;
						prevVal = bestVal;
					}
				}
			}
		}
	endLoop:

		//are we at the root level, return the column
		if (depth == topDepth) {
			return optimalLoc;
		}
		//return the best value
		return bestVal;
	}
public:
	/**
	Default Constructor
	Gathers the PC frequency for speed tests
	*/
	Assignment2() {
		LARGE_INTEGER frq;
		QueryPerformanceFrequency(&frq);
		pcFrequency = double(frq.QuadPart) / 1000.0;
	}

	/**
	Public function to perform the minimax search. Passes in a reference of the
	game board to be altered once completed

	@param Reference to the main game board
	@param Whether to start the search maximising or minimising the player
	@param the number of plys to search
	*/
	void performMiniMax(Board & board, bool maxOrMin, int plys) {
		if (!board.isFull()) {
			LARGE_INTEGER startTimer;
			LARGE_INTEGER endTimer;
			QueryPerformanceCounter(&startTimer);
			//begin recursive minimax search
			auto col = miniMax(maxOrMin, plys, plys, board);
			//place tile in the returned optimal location
			board.placeInColumn(col);
			QueryPerformanceCounter(&endTimer);
			cout << "Minimax of " << plys << " plys completed in: " << (endTimer.QuadPart - startTimer.QuadPart) / pcFrequency << " placing in column " << to_string(col) << endl;
		}
	}

	/**
	Public function to perform the minimax search with alpha beta pruning.
	Passes in a reference of the game board to be altered once completed

	@param Reference to the main game board
	@param Whether to start the search maximising or minimising the player
	@param the number of plys to search
	*/
	void performMiniMaxAB(Board & board, bool maxOrMin, int plys) {
		if (!board.isFull()) {
			LARGE_INTEGER startTimer;
			LARGE_INTEGER endTimer;
			QueryPerformanceCounter(&startTimer);
			//begin recursive minimax search with alpha beta pruning
			auto col = miniMaxAB(maxOrMin, plys, plys, MINCHAR, MAXCHAR, board);
			board.placeInColumn(col);
			//place tile in the returned optimal location
			QueryPerformanceCounter(&endTimer);
			cout << "Minimax of " << plys << " plys completed in: " << (endTimer.QuadPart - startTimer.QuadPart) / pcFrequency << " placing in column " << to_string(col) << endl;
		}
	}


};