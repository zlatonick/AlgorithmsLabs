#pragma once
#include "globals.h"
#include "Object.h"
#include "Snakes.h"

class GameField {
	
	int boardSize;

	int firstPlayer;

	Object* field;
	Object* playerWon;
	Object* computerWon;
	
	Snakes* snakes;

	std::vector<Object*>* objects;

	int gameIsOver;		// 0 - game is not finished, 1 - player won, 2 - comp won

	void updateObjects();

public:
	
	GameField(int _boardSize, int _firstPlayer, int _level);
	~GameField();

	void mouseClick(float x, float y);

	void display(sf::RenderWindow *window, unsigned int time);
};