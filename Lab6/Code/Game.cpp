#include "Game.h"
#include "World.h"


Game::Game(int _boardSize, int _firstPlayer, int _level) {

	this->boardSize = _boardSize;
	this->firstPlayer = _firstPlayer;
	this->level = _level;

	// Background
	this->background = new Object(19);
	this->background->setPosition(0, 0);

	// Menu button
	this->backButton = new Object(20);
	this->backButton->setPosition(1050, 520);

	// The needed field
	this->field = new GameField(_boardSize, _firstPlayer, _level);

	// TODO
	// Load all the textures


}


Game::~Game() {
	delete this->background;
	delete this->backButton;
	delete this->field;
}


void Game::mouseClick(float x, float y) {

	// Checking the Back To Menu button
	if (this->backButton->mouseClick(x, y)) {
		World::getObject()->changeActivityToMenu();

		return;
	}

	this->field->mouseClick(x, y);
}


void Game::display(sf::RenderWindow *window, unsigned int time) {
	this->background->display(window, time);
	this->backButton->display(window, time);
	this->field->display(window, time);
}