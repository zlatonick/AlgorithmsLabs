#include "Snakes.h"


Snakes::Snakes(int _size, int _firstMove, int _level) {
	
	this->size = _size + 2;		// With board ends

	this->firstMove = _firstMove;

	// Choosing the depth of recursion
	if (_level == 0) {
		// Easy
		this->depth = 10;
	}
	else if (_level == 1) {
		// Medium
		this->depth = 15;
	}
	else {
		// Hard
		this->depth = INF;
	}

	this->board = new int*[this->size];
	for (int i = 0; i < this->size; i++) {
		this->board[i] = new int[this->size];

		if (i == 0 || i == this->size - 1) {
			for (int j = 0; j < this->size; j++) {
				this->board[i][j] = BOARD_END;
			}
		}
		else {
			for (int j = 0; j < this->size; j++) {
				if (j == 0 || j == this->size - 1) {
					this->board[i][j] = BOARD_END;
				}
				else {
					this->board[i][j] = 0;
				}
			}
		}
	}

	// Setting the last dots
	if (_size >= 5) {
		this->whiteHeadX = 2;
		this->whiteHeadY = 2;
		this->blackHeadX = this->size - 3;
		this->blackHeadY = this->size - 3;
	}
	else {
		this->whiteHeadX = 1;
		this->whiteHeadY = 1;
		this->blackHeadX = this->size - 2;
		this->blackHeadY = this->size - 2;
	}

	// Starting position
	this->board[this->whiteHeadX][this->whiteHeadY] = 1;
	this->board[this->blackHeadX][this->blackHeadY] = -1;

	// Initializing the routes
	this->whiteRoute = new std::vector<std::pair<int, int>>;
	this->blackRoute = new std::vector<std::pair<int, int>>;

	this->whiteRoute->push_back(
		std::pair<int, int>(this->whiteHeadX - 1, this->whiteHeadY - 1));
	this->blackRoute->push_back(
		std::pair<int, int>(this->blackHeadX - 1, this->blackHeadY - 1));

	// Making the first move
	if (_firstMove == -1) {
		int startResult = this->findBestMove(1, -1, 1, this->depth);
		this->applyMove(this->bestMove);

		this->whiteRoute->push_back(
			std::pair<int, int>(this->whiteHeadX - 1, this->whiteHeadY - 1));
	}
}


Snakes::~Snakes() {

	delete this->whiteRoute;
	delete this->blackRoute;

	for (int i = 0; i < this->size; i++) {
		delete[] this->board[i];
	}
	delete[] this->board;
}


void Snakes::applyMove(Move* move) {
	this->board[move->newX][move->newY] = move->colour;

	if (move->colour == 1) {
		this->whiteHeadX = move->newX;
		this->whiteHeadY = move->newY;
	}
	else {
		this->blackHeadX = move->newX;
		this->blackHeadY = move->newY;
	}
}


void Snakes::unApplyMove(Move* move) {
	this->board[move->newX][move->newY] = 0;

	if (move->colour == 1) {
		this->whiteHeadX = move->oldX;
		this->whiteHeadY = move->oldY;
	}
	else {
		this->blackHeadX = move->oldX;
		this->blackHeadY = move->oldY;
	}
}


std::vector<Move*> Snakes::findAllMoves(int colour) {
	std::vector<Move*> result;

	if (colour == 1) {
		// Checking upper
		if (this->board[this->whiteHeadX][this->whiteHeadY - 1] == 0) {
			result.push_back(new Move(colour, this->whiteHeadX, this->whiteHeadY,
				this->whiteHeadX, this->whiteHeadY - 1));
		}

		// Checking lower
		if (this->board[this->whiteHeadX][this->whiteHeadY + 1] == 0) {
			result.push_back(new Move(colour, this->whiteHeadX, this->whiteHeadY,
				this->whiteHeadX, this->whiteHeadY + 1));
		}

		// Checking lefts
		if (this->board[this->whiteHeadX - 1][this->whiteHeadY] == 0) {
			result.push_back(new Move(colour, this->whiteHeadX, this->whiteHeadY,
				this->whiteHeadX - 1, this->whiteHeadY));
		}

		// Checking rights
		if (this->board[this->whiteHeadX + 1][this->whiteHeadY] == 0) {
			result.push_back(new Move(colour, this->whiteHeadX, this->whiteHeadY,
				this->whiteHeadX + 1, this->whiteHeadY));
		}
	}
	else {
		// Checking upper
		if (this->board[this->blackHeadX][this->blackHeadY - 1] == 0) {
			result.push_back(new Move(colour, this->blackHeadX, this->blackHeadY,
				this->blackHeadX, this->blackHeadY - 1));
		}

		// Checking lower
		if (this->board[this->blackHeadX][this->blackHeadY + 1] == 0) {
			result.push_back(new Move(colour, this->blackHeadX, this->blackHeadY,
				this->blackHeadX, this->blackHeadY + 1));
		}

		// Checking lefts
		if (this->board[this->blackHeadX - 1][this->blackHeadY] == 0) {
			result.push_back(new Move(colour, this->blackHeadX, this->blackHeadY,
				this->blackHeadX - 1, this->blackHeadY));
		}

		// Checking rights
		if (this->board[this->blackHeadX + 1][this->blackHeadY] == 0) {
			result.push_back(new Move(colour, this->blackHeadX, this->blackHeadY,
				this->blackHeadX + 1, this->blackHeadY));
		}
	}

	return result;
}


int Snakes::findBestMove(int colour, int alpha, int beta, int currDepth) {

	if (currDepth == 0) {
		return 0;
	}

	std::vector<Move*> moves = this->findAllMoves(colour);

	if (moves.empty()) {
		// End of the game
		// Opponent wins
		this->bestMove = nullptr;
		return -1;
	}

	Move* maxMove = moves[0];

	int index = 0;
	while (index < moves.size() && alpha < beta) {
		Move* currMove = moves[index];

		this->applyMove(currMove);

		int currValue = -this->findBestMove(-colour, -beta, -alpha, currDepth - 1);

		this->unApplyMove(currMove);

		if (currValue > alpha) {
			maxMove = currMove;
			alpha = currValue;
		}

		index++;
	}

	// Clearing the memory
	for (int i = 0; i < moves.size(); i++) {
		if (moves[i] != maxMove) {
			delete moves[i];
		}
	}

	this->bestMove = maxMove;

	return alpha;
}


int Snakes::findBestMoveNoAB(int colour, int currDepth) {

	if (currDepth == 0) {
		return 0;
	}

	std::vector<Move*> moves = this->findAllMoves(colour);

	if (moves.empty()) {
		// End of the game
		// Opponent wins
		this->bestMove = nullptr;
		return -1;
	}

	int maxValue = -INF;
	Move* maxMove = moves[0];

	int index = 0;
	while (index < moves.size()) {
		Move* currMove = moves[index];

		this->applyMove(currMove);

		int currValue = -this->findBestMoveNoAB(-colour, currDepth - 1);

		this->unApplyMove(currMove);

		if (currValue > maxValue) {
			maxMove = currMove;
			maxValue = currValue;
		}

		index++;
	}

	// Clearing the memory
	for (int i = 0; i < moves.size(); i++) {
		if (moves[i] != maxMove) {
			delete moves[i];
		}
	}

	this->bestMove = maxMove;

	return maxValue;
}


bool Snakes::makePlayerMove(int x, int y) {
	if (this->firstMove == 1) {
		// Checking if the move is correct
		if (this->whiteHeadX == x) {
			if (abs(this->whiteHeadY - y) != 1) {
				return false;
			}
		}
		else if (this->whiteHeadY == y) {
			if (abs(this->whiteHeadX - x) != 1) {
				return false;
			}
		}
		else {
			return false;
		}

		this->applyMove(new Move(this->firstMove,
			this->whiteHeadX, this->whiteHeadY, x, y));

		this->whiteRoute->push_back(
			std::pair<int, int>(x - 1, y - 1));
	}
	else {
		// Checking if the move is correct
		if (this->blackHeadX == x) {
			if (abs(this->blackHeadY - y) != 1) {
				return false;
			}
		}
		else if (this->blackHeadY == y) {
			if (abs(this->blackHeadX - x) != 1) {
				return false;
			}
		}
		else {
			return false;
		}

		this->applyMove(new Move(this->firstMove,
			this->blackHeadX, this->blackHeadY, x, y));

		this->blackRoute->push_back(
			std::pair<int, int>(x - 1, y - 1));
	}

	return true;
}


void Snakes::makeComputerMove() {
	this->findBestMove(-this->firstMove, -1, 1, this->depth);
	this->applyMove(this->bestMove);

	if (this->firstMove == -1) {
		this->whiteRoute->push_back(
			std::pair<int, int>(this->whiteHeadX - 1, this->whiteHeadY - 1));
	}
	else {
		this->blackRoute->push_back(
			std::pair<int, int>(this->blackHeadX - 1, this->blackHeadY - 1));
	}
}


bool Snakes::gameIsOver(int currColour) {
	std::vector<Move*> moves = this->findAllMoves(currColour);
	if (moves.empty()) {
		return true;
	}
	else {
		for (int i = 0; i < moves.size(); i++) {
			delete moves[i];
		}
	}
	return false;
}


std::vector<std::pair<int, int>>* Snakes::getWhiteRoute() {
	return this->whiteRoute;
}


std::vector<std::pair<int, int>>* Snakes::getBlackRoute() {
	return this->blackRoute;
}