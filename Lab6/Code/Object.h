#pragma once
#include "globals.h"

class Object {
protected:
	sf::Sprite *sprite;

public:
	Object(int id);
	~Object();

	// Setting the position of object
	virtual void setPosition(float _x, float _y);

	// If mouse click. Checking the coordinates
	virtual bool mouseClick(float x, float y);

	// Display sprite
	virtual void display(sf::RenderWindow *window, unsigned int _time);
};