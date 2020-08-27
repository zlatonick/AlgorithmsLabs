#include "Menu.h"
#include "World.h"


Menu::Menu() {

	// Background of menu
	this->backGround = new Object(0);
	
	// Board sizes
	this->boardSizes = new std::vector<ObjectHovered*>;

	this->boardSizes->push_back(new ObjectHovered(1, 2));
	this->boardSizes->push_back(new ObjectHovered(3, 4));
	this->boardSizes->push_back(new ObjectHovered(5, 6));
	this->boardSizes->push_back(new ObjectHovered(7, 8));

	(*this->boardSizes)[0]->setPosition(450, 200);
	(*this->boardSizes)[1]->setPosition(660, 200);
	(*this->boardSizes)[2]->setPosition(870, 200);
	(*this->boardSizes)[3]->setPosition(1080, 200);

	// First moves
	this->firstMoves = new std::vector<ObjectHovered*>;

	this->firstMoves->push_back(new ObjectHovered(9, 10));
	this->firstMoves->push_back(new ObjectHovered(11, 12));

	(*this->firstMoves)[0]->setPosition(450, 310);
	(*this->firstMoves)[1]->setPosition(880, 310);

	// Levels
	this->levels = new std::vector<ObjectHovered*>;

	this->levels->push_back(new ObjectHovered(13, 14));
	this->levels->push_back(new ObjectHovered(15, 16));
	this->levels->push_back(new ObjectHovered(17, 18));

	(*this->levels)[0]->setPosition(450, 440);
	(*this->levels)[1]->setPosition(730, 440);
	(*this->levels)[2]->setPosition(1010, 440);

	// Default settings
	this->currBoardSize = 3;
	this->currFirstMove = PLAYER;
	this->currLevel = EASY;

	(*this->boardSizes)[0]->setIsHovered(true);
	(*this->levels)[0]->setIsHovered(true);
	(*this->firstMoves)[0]->setIsHovered(true);
}


Menu::~Menu() {
	delete this->backGround;

	for (int i = 0; i < this->boardSizes->size(); i++) {
		delete (*this->boardSizes)[i];
	}
	delete this->boardSizes;

	for (int i = 0; i < this->firstMoves->size(); i++) {
		delete (*this->firstMoves)[i];
	}
	delete this->firstMoves;

	for (int i = 0; i < this->levels->size(); i++) {
		delete (*this->levels)[i];
	}
	delete this->levels;
}


void Menu::display(sf::RenderWindow *window, unsigned int time) {
	this->backGround->display(window, time);

	for (int i = 0; i < this->boardSizes->size(); i++) {
		(*this->boardSizes)[i]->display(window, time);
	}

	for (int i = 0; i < this->firstMoves->size(); i++) {
		(*this->firstMoves)[i]->display(window, time);
	}

	for (int i = 0; i < this->levels->size(); i++) {
		(*this->levels)[i]->display(window, time);
	}
}


void Menu::mouseClick(float x, float y) {

	// Checking the exit button
	if (sf::IntRect(1130, 583, 100, 100).contains(x, y)) {
		exit(0);
	}

	// Checking the Play button
	if (sf::IntRect(550, 572, 265, 123).contains(x, y)) {
		int firstPlayer;
		int level;

		switch (this->currFirstMove) {
		case PLAYER:
			firstPlayer = 1;
			break;
		case COMPUTER:
			firstPlayer = -1;
			break;
		}

		switch (this->currLevel) {
		case EASY:
			level = 0;
			break;
		case MEDIUM:
			level = 1;
			break;
		case HARD:
			level = 2;
			break;
		}

		World::getObject()->changeActivityToGame(this->currBoardSize, firstPlayer, level);
		return;
	}
	
	// Checking the board size chose
	for (int i = 0; i < this->boardSizes->size(); i++) {
		if ((*this->boardSizes)[i]->mouseClick(x, y)) {
			this->currBoardSize = i + 3;

			(*this->boardSizes)[i]->setIsHovered(true);

			for (int j = 0; j < this->boardSizes->size(); j++) {
				if (j != i) {
					(*this->boardSizes)[j]->setIsHovered(false);
				}
			}
			break;
		}
	}

	// Checking the first move chose
	if ((*this->firstMoves)[0]->mouseClick(x, y)) {

		this->currFirstMove = PLAYER;
		(*this->firstMoves)[0]->setIsHovered(true);
		(*this->firstMoves)[1]->setIsHovered(false);
	}

	if ((*this->firstMoves)[1]->mouseClick(x, y)) {

		this->currFirstMove = COMPUTER;
		(*this->firstMoves)[1]->setIsHovered(true);
		(*this->firstMoves)[0]->setIsHovered(false);
	}

	// Checking the level chose
	if ((*this->levels)[0]->mouseClick(x, y)) {

		this->currLevel = EASY;
		(*this->levels)[0]->setIsHovered(true);
		(*this->levels)[1]->setIsHovered(false);
		(*this->levels)[2]->setIsHovered(false);
	}

	if ((*this->levels)[1]->mouseClick(x, y)) {

		this->currLevel = MEDIUM;
		(*this->levels)[1]->setIsHovered(true);
		(*this->levels)[0]->setIsHovered(false);
		(*this->levels)[2]->setIsHovered(false);
	}

	if ((*this->levels)[2]->mouseClick(x, y)) {

		this->currLevel = HARD;
		(*this->levels)[2]->setIsHovered(true);
		(*this->levels)[0]->setIsHovered(false);
		(*this->levels)[1]->setIsHovered(false);
	}

}