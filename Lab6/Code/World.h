#pragma once
#include "globals.h"
#include "Object.h"
#include "Menu.h"
#include "Game.h"

enum Activity {
	MENU,
	GAME
};

class World {

	static World* thisObject;			// Singletone
	
	Activity currActivity;

	Menu* menu;
	Game* game;

public:
	World();
	~World();

	static World* getObject() { return thisObject; }

	void changeActivityToMenu();
	void changeActivityToGame(int _boardSize, int _firstPlayer, int _level);

	void mouseClick(float x, float y);

	void display(sf::RenderWindow *window, unsigned int time);
};