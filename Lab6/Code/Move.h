#pragma once

struct Move {
	int colour;

	int oldX;
	int oldY;

	int newX;
	int newY;

	Move(int _colour, int _oldX, int _oldY, int _newX, int _newY) :
		colour(_colour), oldX(_oldX), oldY(_oldY), newX(_newX), newY(_newY) {}
};