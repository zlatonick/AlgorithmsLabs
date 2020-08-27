#pragma once
#include "globals.h"
#include "Move.h"

#define BOARD_END 64
#define INF 1000

class Snakes {

	/*
	 * The game field
	 * 0 - emply cell
	 * 1 - white dot
	 * -1 - black dot
	 * BOARD_END - end of board
	 */
	int** board;

	int size;			// The size of board

	int firstMove;		// 1 if player is first

	int depth;			// The depth of recursion

	// Coordinates of the last dot for each colour
	int whiteHeadX;
	int whiteHeadY;
	int blackHeadX;
	int blackHeadY;

	// Red and black routes
	std::vector<std::pair<int, int>>* whiteRoute;
	std::vector<std::pair<int, int>>* blackRoute;

	// The best move in current position.
	// Is initialized in findBestMove function
	Move* bestMove;

	std::vector<Move*> findAllMoves(int colour);

	void applyMove(Move* move);

	void unApplyMove(Move* move);

public:
	Snakes(int _size, int _firstMove, int _level);
	~Snakes();

	int findBestMoveNoAB(int colour, int currDepth);
	int findBestMove(int colour, int alpha, int beta, int currDepth);

	bool makePlayerMove(int x, int y);
	void makeComputerMove();

	bool gameIsOver(int currColour);

	std::vector<std::pair<int, int>>* getWhiteRoute();
	std::vector<std::pair<int, int>>* getBlackRoute();

	//void play();

	// DEBUG
	//void printBoard();
};