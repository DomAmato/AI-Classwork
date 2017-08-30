#include "Assignment1.h"

/**
Just a default constructor to get the frequency of the CPU to use for timing
*/
Assignment1::Assignment1()
{
	LARGE_INTEGER frq;
	QueryPerformanceFrequency(&frq);
	pcFrequncy = double(frq.QuadPart) / 1000.0;
}

/**
Simple Breadth-First search using a STL queue

@param the starting puzzle state to perform the search on
@return a tuple consisting of the cost of the completed puzzle, the depth
which the goal state was found, how many nodes were visited, and
the time it took to perform the search
*/
tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> Assignment1::breadthFirst(const Puzzle initialState, SuccessorOrder order)
{
	//all the previous states visited by the search
	unordered_set<unsigned __int32> prevStates;
	//STL queue container of the puzzles to check
	queue<Puzzle> successors;
	//32bit int to measure nodes visited
	unsigned __int32 node = 0;
	//32bit integer to track max queue size
	unsigned __int32 queueSize = 0;
	//A pointer to the puzzle state, we need a pointer otherwise
	//the parent link breaks
	const Puzzle * boardState = new Puzzle(initialState);
	//add the inital state to the set
	prevStates.emplace(boardState->toInt());

	//get the first tick the search begins
	QueryPerformanceCounter(&startTimer);
	//If we look at more nodes than possible states something is wrong...
	while (!boardState->isGoalState() && node <= 362880) {
		//increment our visited nodes
		node++;
		//add all of our successors to the queue
		for (auto entry : boardState->getSuccesors(prevStates)) {
			prevStates.emplace(entry.toInt());//dont feed in duplicates
			successors.push(entry);
		}
		//set the next puzzle to create successors from, could be the goal 
		boardState = new Puzzle(successors.front().getBoard(), successors.front().getParent());
		//remove the item from teh queue
		if (successors.size() > queueSize) {
			queueSize = successors.size();
		}
		successors.pop();
	}
	//the tick in which the search completes
	QueryPerformanceCounter(&endTimer);

	//iterate through the board to find the depth, but lets create a new pointer to interate with
	const Puzzle * depth = boardState;
	//Only keep an int this large because of DFS, other searches should never have a depth this large
	unsigned __int32 level = 0;
	//the pointer wont be the same so lets check the board to see if we reached the parent
	//alternatively we can check if the parent is null
	while (depth->getBoard() != initialState.getBoard()) {
		depth = depth->getParent();
		level++;
	}

	//return the tuple of all the information we need 
	return make_tuple(boardState, calculateCost(boardState), level, node, (endTimer.QuadPart - startTimer.QuadPart) / pcFrequncy, queueSize);
}

/**
Simple Depth-First search using a STL stack which uses iteration

@param the starting puzzle state to perform the search on
@return a tuple consisting of the cost of the completed puzzle, the depth
which the goal state was found, how many nodes were visited, and
the time it took to perform the search
*/
tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> Assignment1::depthFirst(const Puzzle initialState, SuccessorOrder order)
{
	//all the previous states visited by the search
	unordered_set<unsigned __int32> prevStates;
	//STL stack container of the possible puzzles
	stack<Puzzle> successors;
	//32bit int to measure nodes visited
	unsigned __int32 node = 0;
	//32bit integer to track max queue size
	unsigned __int32 queueSize = 0;
	//A pointer to the puzzle state, we need a pointer otherwise
	//the parent link breaks
	const Puzzle * boardState = new Puzzle(initialState);

	//get the first tick the search begins
	QueryPerformanceCounter(&startTimer);
	//If we look at more nodes than possible states something is wrong...
	while (!boardState->isGoalState() && node <= 362880) {
		//increment our visited nodes
		node++;
		//put our current board into the visited set
		prevStates.emplace(boardState->toInt());
		//add all of our successors to the queue
		for (auto entry : boardState->getSuccesors(prevStates)) {
			successors.push(entry);
		}
		//set our next board state from the top of the stack
		boardState = new Puzzle(successors.top().getBoard(), successors.top().getParent());
		//check if our queue size is larger
		if (successors.size() > queueSize) {
			queueSize = successors.size();
		}
		//pop the top of the stack
		successors.pop();
	}
	//the tick in which the search completes
	QueryPerformanceCounter(&endTimer);

	//iterate through the board to find the depth, but lets create a new pointer to interate with
	const Puzzle * depth = boardState;
	//Only keep an int this large because of DFS, other searches should never have a depth this large
	unsigned __int32 level = 0;
	//the pointer wont be the same so lets check the board to see if we reached the parent
	//alternatively we can check if the parent is null
	while (depth->getBoard() != initialState.getBoard()) {
		depth = depth->getParent();
		level++;
	}

	//return the tuple of all the information we need 
	return make_tuple(boardState, calculateCost(boardState), level, node, (endTimer.QuadPart - startTimer.QuadPart) / pcFrequncy, queueSize);
}

/**
Simple Depth-First search using a STL stack which uses recursion
currently throws stack overflows, not safe to use.

@param the starting puzzle state to perform the search on
@return a tuple consisting of the cost of the completed puzzle, the depth
which the goal state was found, how many nodes were visited, and
the time it took to perform the search
*/
tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> Assignment1::depthFirstR(Puzzle initialState, SuccessorOrder order)
{
	unordered_set<unsigned __int32> prevStates;

	//variable for number of nodes visited
	unsigned __int32 node = 0;
	//variable for number of levels visited
	unsigned __int32 level = 0;
	//32bit integer to track max queue size
	unsigned __int32 queueSize = 0;

	//get the first tick the search begins
	QueryPerformanceCounter(&startTimer);
	Puzzle board = initialState;
	board = RecursiveDepthSearch(board, level, node, prevStates, order);
	//the tick in which the search completes
	QueryPerformanceCounter(&endTimer);

	//return the tuple of all the information we need 
	return make_tuple(&board, calculateCost(&board), level, node, (endTimer.QuadPart - startTimer.QuadPart) / pcFrequncy, queueSize);
}

/**
Recursive DFS helper function

@param The current puzzle state
@param current level
@param reference to the nodes visited
@param reference to the set of previously visited nodes
@return the puzzle that has found the goal state
*/
Puzzle Assignment1::RecursiveDepthSearch(Puzzle currentBoard, unsigned __int32 level, unsigned __int32 & nodes, unordered_set<unsigned __int32>& prevStates, SuccessorOrder order)
{
	//return the current board if its a goal state
	nodes++;
	if (currentBoard.isGoalState())
		return currentBoard;

	prevStates.emplace(currentBoard.toInt());

	for (auto entry : currentBoard.getSuccesors(prevStates)) {
		Puzzle board = RecursiveDepthSearch(entry, level, nodes, prevStates, order);
		if (board.isGoalState()) {
			return board;
		}
	}

	//its possible all the states from the successorsr has been previously visited so return
	//the board even if its not a goal or we are at our depth
	return currentBoard;
}

/**
Iterative Deepening Search, a recursive depth limited search that searches the entire
depth before moving on to the next level

@param the starting puzzle state to perform the search on
@return a tuple consisting of the cost of the completed puzzle, the depth
which the goal state was found, how many nodes were visited, and
the time it took to perform the search
*/
tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> Assignment1::iterativeDeepening(Puzzle initialState, SuccessorOrder order)
{
	//variable for number of nodes visited
	unsigned __int32 node = 0;
	//variable for number of levels visited
	unsigned __int32 level = 0;
	//32bit integer to track max queue size
	unsigned __int32 queueSize = 0;

	//get the first tick the search begins
	QueryPerformanceCounter(&startTimer);
	//make a new puzzle, so we dont alter the inital state
	Puzzle * board = &initialState;
	while (!board->isGoalState()) {
		//reset the queue size
		queueSize = 0;
		//since we start from square 1 each time the depth returns we need to either
		//clear the set or make a new one, since STLs in c++11 have dynamic memory it
		//shouldn't really matter which we choose
		board = RecursiveDepthLimitedSearch(board, level++, node, queueSize, unordered_set<unsigned __int32>(), order);
	}
	//the tick in which the search completes
	QueryPerformanceCounter(&endTimer);

	//return the tuple of all the information we need 
	return make_tuple(board, calculateCost(board), level, node, (endTimer.QuadPart - startTimer.QuadPart) / pcFrequncy, queueSize);
}

/**
Recursive IDS helper function

@param The current puzzle state
@param current level
@param reference to the nodes visited
@param reference to the set of previously visited nodes
@return the puzzle that has found the goal state
*/
Puzzle * Assignment1::RecursiveDepthLimitedSearch(Puzzle * currentBoard, unsigned __int32 limit, unsigned __int32 & nodes, unsigned __int32 & queueSize, unordered_set<unsigned __int32> & prevStates, SuccessorOrder order)
{
	//increment the number of nodes visited
	nodes++;
	//return the current board if we have reached the limit or if its a goal state
	if (currentBoard->isGoalState() || limit <= 0) {
		if (!currentBoard->isGoalState()) {
			//it isnt the goal state so we are going up a level, decrement the queue size
			queueSize--;
		}
		return currentBoard;
	}

	//add the voard to the set of previously visited
	prevStates.emplace(currentBoard->toInt());

	//get the sucessors and recursively search until the max depth is reached
	for (auto entry : currentBoard->getSuccesors(prevStates)) {
		//we could start at limit zero and add but its easier to increment it outside the recrsive loop and
		//report that value
		queueSize++;
		Puzzle * board = RecursiveDepthLimitedSearch(new Puzzle(entry.getBoard(), entry.getParent()), limit - 1, nodes, queueSize, prevStates, order);
		if (board->isGoalState()) {
			//we found the goal, now we return all the way back
			return board;
		}
	}

	//decrement the queue size since we reached a dead end
	queueSize--;
	//its possible all the states from the successor has been previously visited so return
	//the board even if its not a goal or we are at our depth
	return currentBoard;
}

/**
Uniform Cost Search, Dijkstra's algorithm - Computes the next step based on cost of moving the tile
utilizes a priority queue to search for the path of least cost

@param the starting puzzle state to perform the search on
@return a tuple consisting of the cost of the completed puzzle, the depth
which the goal state was found, how many nodes were visited, and
the time it took to perform the search
*/
tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> Assignment1::uniformCost(Puzzle initialState, SuccessorOrder order)
{
	//all the previous states visited by the search
	unordered_set<unsigned __int32> prevStates;
	//create a custom priority queue that is a vector of pairs which the hueristic is the first value and the associated board is the second
	priority_queue<pair<int, Puzzle>, vector<pair<int, Puzzle>>, pairComparison> successors;
	//A pointer to the puzzle state, we need a pointer otherwise
	//the parent link breaks
	const Puzzle * boardState = new Puzzle(initialState);
	//variable for number of nodes visited
	unsigned __int32 node = 0;
	//32bit integer to track max queue size
	unsigned __int32 queueSize = 0;
	//variable to keep track of the current lowest cost for computing the ordering of the priority queue
	unsigned __int32 bestVal = 0;

	//get the first tick the search begins
	QueryPerformanceCounter(&startTimer);
	while (!boardState->isGoalState()) {
		//increment our visited nodes
		node++;
		//put our current board into the visited set
		prevStates.emplace(boardState->toInt());
		//add all of our successors to the queue
		for (auto entry : boardState->getSuccesorsWithCost(prevStates)) {
			//make a pair with the cost and the puzzle state
			//cost is determined from the current nodes cost + the cost to move the tile
			successors.emplace(bestVal + entry.first, entry.second);
		}
		//set our next board state from the front of the queue
		boardState = new Puzzle(successors.top().second.getBoard(), successors.top().second.getParent());
		//the current value of the top node to be popped
		bestVal = successors.top().first;
		//check if our queue size is larger
		if (successors.size() > queueSize) {
			queueSize = successors.size();
		}
		//pop the front of the queue
		successors.pop();
	}
	//the tick in which the search completes
	QueryPerformanceCounter(&endTimer);

	//iterate through the board to find the depth, but lets create a new pointer to interate with
	const Puzzle * depth = boardState;
	//Only keep an int this large because of DFS, other searches should never have a depth this large
	unsigned __int32 level = 0;
	//the pointer wont be the same so lets check the board to see if we reached the parent
	//alternatively we can check if the parent is null
	while (depth->getBoard() != initialState.getBoard()) {
		depth = depth->getParent();
		level++;
	}

	//return the tuple of all the information we need 
	return make_tuple(boardState, calculateCost(boardState), level, node, (endTimer.QuadPart - startTimer.QuadPart) / pcFrequncy, queueSize);
}

/**
Greedy Best First Search
Explores based on the most promising misplaced tile heuristic

@param the starting puzzle state to perform the search on
@return a tuple consisting of the cost of the completed puzzle, the depth
which the goal state was found, how many nodes were visited, and
the time it took to perform the search
*/
tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> Assignment1::bestFirst(Puzzle initialState, SuccessorOrder order)
{
	//all the previous states visited by the search
	unordered_set<unsigned __int32> prevStates;
	//create a custom priority queue that is a vector of pairs which the hueristic is the first value and the associated board is the second
	priority_queue<pair<int, Puzzle>, vector<pair<int, Puzzle>>, pairComparison> successors;
	//A pointer to the puzzle state, we need a pointer otherwise
	//the parent link breaks
	const Puzzle * boardState = new Puzzle(initialState);
	unsigned __int32 node = 0;
	//32bit integer to track max queue size
	unsigned __int32 queueSize = 0;

	//get the first tick the search begins
	QueryPerformanceCounter(&startTimer);
	while (!boardState->isGoalState()) {
		//increment our visited nodes
		node++;
		//put our current board into the visited set
		prevStates.emplace(boardState->toInt());
		//add all of our successors to the queue
		for (auto entry : boardState->getSuccesors(prevStates)) {
			//make a pair with the heuristic and the puzzle state
			successors.emplace(Heuristics::heurtisticMisplacedTiles(entry), entry);
		}
		boardState = new Puzzle(successors.top().second.getBoard(), successors.top().second.getParent());
		//check if our queue size is larger
		if (successors.size() > queueSize) {
			queueSize = successors.size();
		}
		//pop the top of the stack
		successors.pop();
	}
	//the tick in which the search completes
	QueryPerformanceCounter(&endTimer);

	//iterate through the board to find the depth, but lets create a new pointer to interate with
	const Puzzle * depth = boardState;
	//Only keep an int this large because of DFS, other searches should never have a depth this large
	unsigned __int32 level = 0;
	//the pointer wont be the same so lets check the board to see if we reached the parent
	//alternatively we can check if the parent is null
	while (depth->getBoard() != initialState.getBoard()) {
		depth = depth->getParent();
		level++;
	}

	//return the tuple of all the information we need 
	return make_tuple(boardState, calculateCost(boardState), level, node, (endTimer.QuadPart - startTimer.QuadPart) / pcFrequncy, queueSize);
}

/**
A* Search using Misplaced Tiles Heuristic

@param the starting puzzle state to perform the search on
@return a tuple consisting of the cost of the completed puzzle, the depth
which the goal state was found, how many nodes were visited, and
the time it took to perform the search
*/
tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> Assignment1::aStar1(Puzzle initialState, SuccessorOrder order)
{
	//all the previous states visited by the search
	unordered_set<unsigned __int32> prevStates;
	//create a custom priority queue that is a vector of pairs which the hueristic is the first value and the associated board is the second
	priority_queue<pair<int, Puzzle>, vector<pair<int, Puzzle>>, pairComparison> successors;
	//A pointer to the puzzle state, we need a pointer otherwise
	//the parent link breaks
	const Puzzle * boardState = new Puzzle(initialState);
	//variable for number of nodes visited
	unsigned __int32 node = 0;
	//variable to keep track of the current lowest cost for computing the ordering of the priority queue
	unsigned __int32 bestVal = 0;
	//32bit integer to track max queue size
	unsigned __int32 queueSize = 0;

	//get the first tick the search begins
	QueryPerformanceCounter(&startTimer);
	while (!boardState->isGoalState()) {
		//increment our visited nodes
		node++;
		//put our current board into the visited set
		prevStates.emplace(boardState->toInt());
		//add all of our successors to the queue
		for (auto entry : boardState->getSuccesorsWithCost(prevStates)) {
			//make a pair with the cost and the puzzle state
			//cost is determined from the current nodes cost + the cost to move the tile + heuristic
			successors.emplace(bestVal + Heuristics::heurtisticMisplacedTiles(entry.second) + entry.first, entry.second);
		}
		//set our next board state from the front of the queue
		boardState = new Puzzle(successors.top().second.getBoard(), successors.top().second.getParent());
		//the current value of the top node to be popped
		bestVal = successors.top().first;
		//check if our queue size is larger
		if (successors.size() > queueSize) {
			queueSize = successors.size();
		}
		//pop the front of the queue
		successors.pop();
	}
	//the tick in which the search completes
	QueryPerformanceCounter(&endTimer);

	//iterate through the board to find the depth, but lets create a new pointer to interate with
	const Puzzle * depth = boardState;
	//Only keep an int this large because of DFS, other searches should never have a depth this large
	unsigned __int32 level = 0;
	//the pointer wont be the same so lets check the board to see if we reached the parent
	//alternatively we can check if the parent is null
	while (depth->getBoard() != initialState.getBoard()) {
		depth = depth->getParent();
		level++;
	}

	//return the tuple of all the information we need 
	return make_tuple(boardState, calculateCost(boardState), level, node, (endTimer.QuadPart - startTimer.QuadPart) / pcFrequncy, queueSize);
}

/**
A* Search using the Manhatten Distance Heuristic

@param the starting puzzle state to perform the search on
@return a tuple consisting of the cost of the completed puzzle, the depth
which the goal state was found, how many nodes were visited, and
the time it took to perform the search
*/
tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> Assignment1::aStar2(Puzzle initialState, SuccessorOrder order)
{
	//all the previous states visited by the search
	unordered_set<unsigned __int32> prevStates;
	//create a custom priority queue that is a vector of pairs which the hueristic is the first value and the associated board is the second
	priority_queue<pair<int, Puzzle>, vector<pair<int, Puzzle>>, pairComparison> successors;
	//A pointer to the puzzle state, we need a pointer otherwise
	//the parent link breaks
	const Puzzle * boardState = new Puzzle(initialState);
	//variable for number of nodes visited
	unsigned __int32 node = 0;
	//variable to keep track of the current lowest cost for computing the ordering of the priority queue
	unsigned __int32 bestVal = 0;
	//32bit integer to track max queue size
	unsigned __int32 queueSize = 0;

	//get the first tick the search begins
	QueryPerformanceCounter(&startTimer);
	while (!boardState->isGoalState()) {
		//increment our visited nodes
		node++;
		//put our current board into the visited set
		prevStates.emplace(boardState->toInt());
		//add all of our successors to the queue
		for (auto entry : boardState->getSuccesorsWithCost(prevStates)) {
			//make a pair with the cost and the puzzle state
			//cost is determined from the current nodes cost + the cost to move the tile + heuristic
			successors.emplace(bestVal + Heuristics::heurtisticManhatten(entry.second) + entry.first, entry.second);
		}
		//set our next board state from the front of the queue
		boardState = new Puzzle(successors.top().second.getBoard(), successors.top().second.getParent());
		//the current value of the top node to be popped
		bestVal = successors.top().first;
		//check if our queue size is larger
		if (successors.size() > queueSize) {
			queueSize = successors.size();
		}
		//pop the front of the queue
		successors.pop();
	}
	//the tick in which the search completes
	QueryPerformanceCounter(&endTimer);

	//iterate through the board to find the depth, but lets create a new pointer to interate with
	const Puzzle * depth = boardState;
	//Only keep an int this large because of DFS, other searches should never have a depth this large
	unsigned __int32 level = 0;
	//the pointer wont be the same so lets check the board to see if we reached the parent
	//alternatively we can check if the parent is null
	while (depth->getBoard() != initialState.getBoard()) {
		depth = depth->getParent();
		level++;
	}

	//return the tuple of all the information we need 
	return make_tuple(boardState, calculateCost(boardState), level, node, (endTimer.QuadPart - startTimer.QuadPart) / pcFrequncy, queueSize);
}

/**
A* Search with a hybrid of Misplaced Tiles and Manhatten Distance Heuristics

@param the starting puzzle state to perform the search on
@return a tuple consisting of the cost of the completed puzzle, the depth
which the goal state was found, how many nodes were visited, and
the time it took to perform the search
*/
tuple<const Puzzle *, unsigned __int32, unsigned __int32, unsigned __int32, double, unsigned __int32> Assignment1::aStar3(Puzzle initialState, SuccessorOrder order)
{
	//all the previous states visited by the search
	unordered_set<unsigned __int32> prevStates;
	//create a custom priority queue that is a vector of pairs which the hueristic is the first value and the associated board is the second
	priority_queue<pair<int, Puzzle>, vector<pair<int, Puzzle>>, pairComparison> successors;
	//A pointer to the puzzle state, we need a pointer otherwise
	//the parent link breaks
	const Puzzle * boardState = new Puzzle(initialState);
	//variable for number of nodes visited
	unsigned __int32 node = 0;
	//variable to keep track of the current lowest cost for computing the ordering of the priority queue
	unsigned __int32 bestVal = 0;
	//32bit integer to track max queue size
	unsigned __int32 queueSize = 0;

	//get the first tick the search begins
	QueryPerformanceCounter(&startTimer);
	while (!boardState->isGoalState()) {
		//increment our visited nodes
		node++;
		//put our current board into the visited set
		prevStates.emplace(boardState->toInt());
		//add all of our successors to the queue
		for (auto entry : boardState->getSuccesorsWithCost(prevStates)) {
			//make a pair with the cost and the puzzle state
			//cost is determined from the current nodes cost + the cost to move the tile + heuristic
			successors.emplace(bestVal + Heuristics::heurtisticHybrid(entry.second) + entry.first, entry.second);
		}
		//set our next board state from the front of the queue
		boardState = new Puzzle(successors.top().second.getBoard(), successors.top().second.getParent());
		//the current value of the top node to be popped
		bestVal = successors.top().first;
		//check if our queue size is larger
		if (successors.size() > queueSize) {
			queueSize = successors.size();
		}
		//pop the front of the queue
		successors.pop();
	}
	//the tick in which the search completes
	QueryPerformanceCounter(&endTimer);

	//iterate through the board to find the depth, but lets create a new pointer to interate with
	const Puzzle * depth = boardState;
	//Only keep an int this large because of DFS, other searches should never have a depth this large
	unsigned __int32 level = 0;
	//the pointer wont be the same so lets check the board to see if we reached the parent
	//alternatively we can check if the parent is null
	while (depth->getBoard() != initialState.getBoard()) {
		depth = depth->getParent();
		level++;
	}

	//return the tuple of all the information we need 
	return make_tuple(boardState, calculateCost(boardState), level, node, (endTimer.QuadPart - startTimer.QuadPart) / pcFrequncy, queueSize);
}

/**
Traverse the completed puzzles path and calculate the cost

@param the completed puzzle state to traverse
@return a 32 bit integer containing the cost of the path
*/
unsigned __int32 Assignment1::calculateCost(const Puzzle * completedPuzzle)
{
	//similar to the depth test create a new pointer and traverse it
	const Puzzle * depth = completedPuzzle;

	unsigned __int32 cost = 0;
	//the pointer wont be the same as the inital board so lets check if the parent is null
	while (depth->getParent() != NULL) {
		//save the old board state
		auto prevBoard = depth->getBoard();
		//get the parent
		depth = depth->getParent();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				//once we find where the sliding tile is we can look at the 
				//old board to find what value moved
				if ((*depth)[i][j] == 0) {
					cost += prevBoard[i][j];
					goto next;
				}
			}
		}
	next: {} //we get a syntax error if nothing comes after next so just give it an empty brace
	}
	return cost;
}
