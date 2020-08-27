#include "GameField.h"


GameField::GameField(int _boardSize, int _firstPlayer, int _level) {

	this->boardSize = _boardSize;
	this->firstPlayer = _firstPlayer;

	this->field = new Object(this->boardSize + 18);
	this->field->setPosition(413, 110);

	this->playerWon = new Object(49);
	this->playerWon->setPosition(441, 331);

	this->computerWon = new Object(50);
	this->computerWon->setPosition(441, 331);

	this->snakes = new Snakes(this->boardSize, _firstPlayer, _level);

	this->objects = new std::vector<Object*>;

	this->gameIsOver = 0;

	this->updateObjects();
}


GameField::~GameField() {
	delete this->field;
	delete this->snakes;

	for (int i = 0; i < this->objects->size(); i++) {
		delete (*this->objects)[i];
	}
	delete this->objects;
}


void GameField::mouseClick(float x, float y) {
	if (this->gameIsOver != 0) {
		return;
	}

	if (this->field->mouseClick(x, y)) {
		const int fieldUp = 110;
		const int fieldLeft = 413;
		const int cellSize = 540 / this->boardSize;

		bool exitFlag = false;
		for (int i = 0; i < this->boardSize; i++) {
			for (int j = 0; j < this->boardSize; j++) {
				if (sf::IntRect(fieldLeft + (cellSize * j),
					fieldUp + (cellSize * i),
					cellSize, cellSize).contains(x, y)) {

					if (this->snakes->makePlayerMove(j + 1, i + 1)) {

						if (this->snakes->gameIsOver(-this->firstPlayer)) {
							// Player has won
							this->gameIsOver = 1;
							this->updateObjects();
							return;
						}

						this->snakes->makeComputerMove();

						if (this->snakes->gameIsOver(this->firstPlayer)) {
							// Computer has won
							this->gameIsOver = 2;
						}

						this->updateObjects();
					}
					else {
						// DEBUG
						std::cout << "Incorrect move" << std::endl;
					}

					exitFlag = true;
					break;
				}
			}
			if (exitFlag) {
				break;
			}
		}
	}
}


void GameField::updateObjects() {

	const int fieldUp = 110;
	const int fieldLeft = 413;
	const int cellSize = 540 / this->boardSize;
	const int edgeLeftIndent = ((double)cellSize) / 2.8125;
	const int edgeUpIndent = ((double)cellSize) / 1.36363636;
	
	// Removing old objects
	for (int i = 0; i < this->objects->size(); i++) {
		delete (*this->objects)[i];
	}
	this->objects->clear();

	std::vector<std::pair<int, int>>* whiteRoute = this->snakes->getWhiteRoute();
	std::vector<std::pair<int, int>>* blackRoute = this->snakes->getBlackRoute();

	int whiteRouteSize = whiteRoute->size();
	for (int i = 0; i < whiteRouteSize; i++) {
		// Adding the white dot
		Object* dot;

		switch (this->boardSize) {
		case 3:
			dot = new Object(25);
			break;
		case 4:
			dot = new Object(26);
			break;
		case 5:
			dot = new Object(27);
			break;
		case 6:
			dot = new Object(28);
			break;
		}

		std::pair<int, int> coords = (*whiteRoute)[i];
		dot->setPosition(fieldLeft + (cellSize * coords.first),
			fieldUp + (cellSize * coords.second));

		this->objects->push_back(dot);

		// If not last lot - adding the edge
		if (i != whiteRouteSize - 1) {
			std::pair<int, int> nextCoords = (*whiteRoute)[i + 1];

			bool isVertical = nextCoords.first == coords.first;

			if (isVertical) {
				// Vertical edge
				Object* edge;

				switch (this->boardSize) {
				case 3:
					edge = new Object(33);
					break;
				case 4:
					edge = new Object(34);
					break;
				case 5:
					edge = new Object(35);
					break;
				case 6:
					edge = new Object(36);
					break;
				}

				// To upper or to lower
				if (nextCoords.second < coords.second) {
					// Edge to upper
					edge->setPosition(fieldLeft + (cellSize * coords.first) + edgeLeftIndent,
						fieldUp + (cellSize * nextCoords.second) + edgeUpIndent);
				}
				else {
					// Edge to lower
					edge->setPosition(fieldLeft + (cellSize * coords.first) + edgeLeftIndent,
						fieldUp + (cellSize * coords.second) + edgeUpIndent);
				}

				this->objects->push_back(edge);
			}
			else {
				// Horizontal edge
				Object* edge;

				switch (this->boardSize) {
				case 3:
					edge = new Object(29);
					break;
				case 4:
					edge = new Object(30);
					break;
				case 5:
					edge = new Object(31);
					break;
				case 6:
					edge = new Object(32);
					break;
				}

				// To lefts or to rights
				if (nextCoords.first < coords.first) {
					// Edge to lefts
					edge->setPosition(fieldLeft + (cellSize * nextCoords.first) + edgeUpIndent,
						fieldUp + (cellSize * coords.second) + edgeLeftIndent);
				}
				else {
					// Edge to rights
					edge->setPosition(fieldLeft + (cellSize * coords.first) + edgeUpIndent,
						fieldUp + (cellSize * coords.second) + edgeLeftIndent);
				}

				this->objects->push_back(edge);
			}
		}
	}

	// Black dots and edges
	int blackRouteSize = blackRoute->size();
	for (int i = 0; i < blackRouteSize; i++) {
		// Adding the black dot
		Object* dot;

		switch (this->boardSize) {
		case 3:
			dot = new Object(37);
			break;
		case 4:
			dot = new Object(38);
			break;
		case 5:
			dot = new Object(39);
			break;
		case 6:
			dot = new Object(40);
			break;
		}

		std::pair<int, int> coords = (*blackRoute)[i];
		dot->setPosition(fieldLeft + (cellSize * coords.first),
			fieldUp + (cellSize * coords.second));

		this->objects->push_back(dot);

		// If not last lot - adding the edge
		if (i != blackRouteSize - 1) {
			std::pair<int, int> nextCoords = (*blackRoute)[i + 1];

			bool isVertical = nextCoords.first == coords.first;

			if (isVertical) {
				// Vertical edge
				Object* edge;

				switch (this->boardSize) {
				case 3:
					edge = new Object(45);
					break;
				case 4:
					edge = new Object(46);
					break;
				case 5:
					edge = new Object(47);
					break;
				case 6:
					edge = new Object(48);
					break;
				}

				// To upper or to lower
				if (nextCoords.second < coords.second) {
					// Edge to upper
					edge->setPosition(fieldLeft + (cellSize * coords.first) + edgeLeftIndent,
						fieldUp + (cellSize * nextCoords.second) + edgeUpIndent);
				}
				else {
					// Edge to lower
					edge->setPosition(fieldLeft + (cellSize * coords.first) + edgeLeftIndent,
						fieldUp + (cellSize * coords.second) + edgeUpIndent);
				}

				this->objects->push_back(edge);
			}
			else {
				// Horizontal edge
				Object* edge;

				switch (this->boardSize) {
				case 3:
					edge = new Object(41);
					break;
				case 4:
					edge = new Object(42);
					break;
				case 5:
					edge = new Object(43);
					break;
				case 6:
					edge = new Object(44);
					break;
				}

				// To lefts or to rights
				if (nextCoords.first < coords.first) {
					// Edge to lefts
					edge->setPosition(fieldLeft + (cellSize * nextCoords.first) + edgeUpIndent,
						fieldUp + (cellSize * coords.second) + edgeLeftIndent);
				}
				else {
					// Edge to rights
					edge->setPosition(fieldLeft + (cellSize * coords.first) + edgeUpIndent,
						fieldUp + (cellSize * coords.second) + edgeLeftIndent);
				}

				this->objects->push_back(edge);
			}
		}
	}
}


void GameField::display(sf::RenderWindow *window, unsigned int time) {
	this->field->display(window, time);

	for (int i = 0; i < this->objects->size(); i++) {
		(*this->objects)[i]->display(window, time);
	}

	if (this->gameIsOver == 1) {
		this->playerWon->display(window, time);
	}
	if (this->gameIsOver == 2) {
		this->computerWon->display(window, time);
	}
}