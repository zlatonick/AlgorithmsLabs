#pragma once
#include "Object.h"

class ObjectHovered : public Object {

	sf::Sprite *spriteHovered;

	bool isHovered;

public:
	ObjectHovered(int id, int idHovered);
	~ObjectHovered();

	// Setting the position of both images
	void setPosition(float _x, float _y);

	// If mouse click. Checking the coordinates
	bool mouseClick(float x, float y);

	// Enabling the illumination
	void setIsHovered(bool _isHovered);

	// Display sprite
	void display(sf::RenderWindow *window, unsigned int _time);
};