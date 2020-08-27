#include "World.h"


World* World::thisObject = nullptr;


World::World() {

	if (thisObject != nullptr)
		exit(EXIT_FAILURE);

	thisObject = this;
	
	this->currActivity = MENU;

	this->menu = new Menu();
	this->game = nullptr;
}


World::~World() {
	switch (this->currActivity) {
	case MENU:
		delete this->menu;
		break;

	case GAME:
		delete this->game;
		break;
	}
}


void World::changeActivityToMenu() {
	this->currActivity = MENU;
	
	delete this->game;
	this->menu = new Menu();
}


void World::changeActivityToGame(int _boardSize, int _firstPlayer, int _level) {
	this->currActivity = GAME;

	delete this->menu;
	this->game = new Game(_boardSize, _firstPlayer, _level);
}


void World::display(sf::RenderWindow *window, unsigned int time) {
	switch (this->currActivity) {
	case MENU:
		this->menu->display(window, time);
		break;

	case GAME:
		this->game->display(window, time);
		break;
	}
}


void World::mouseClick(float x, float y) {
	switch (this->currActivity) {
	case MENU:
		this->menu->mouseClick(x, y);
		break;

	case GAME:
		this->game->mouseClick(x, y);
		break;
	}
}