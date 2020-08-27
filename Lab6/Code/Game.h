#pragma once
#include "globals.h"
#include "Object.h"
#include "GameField.h"

class Game {

	int boardSize;
	int firstPlayer;
	int level;

	Object* background;
	Object* backButton;
	GameField* field;

public:

	Game(int _boardSize, int _firstPlayer, int _level);
	~Game();

	void mouseClick(float x, float y);

	void display(sf::RenderWindow *window, unsigned int time);
};