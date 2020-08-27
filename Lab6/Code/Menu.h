#pragma once
#include "globals.h"
#include "Object.h"
#include "ObjectHovered.h"

enum FirstMove {
	PLAYER,
	COMPUTER
};

enum Level {
	EASY,
	MEDIUM,
	HARD
};

class Menu {

	Object* backGround;

	std::vector<ObjectHovered*>* boardSizes;
	std::vector<ObjectHovered*>* firstMoves;
	std::vector<ObjectHovered*>* levels;

	int currBoardSize;
	FirstMove currFirstMove;
	Level currLevel;

public:

	Menu();
	~Menu();

	void mouseClick(float x, float y);

	void display(sf::RenderWindow *window, unsigned int time);
};