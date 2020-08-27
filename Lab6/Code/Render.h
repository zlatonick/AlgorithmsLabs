#pragma once
#include "globals.h"
#include "TextureLoader.h"
#include "World.h"

// Singletone
// Loading and displaying objects on the screen

class Render {
	static Render *thisObject;

	sf::RenderWindow *window;
	TextureLoader *textureLoader;
	World *world;

	void initTextures();

public:
	Render();
	~Render();

	static Render* getObject() { return thisObject; }

	void rendering();		// The main cycle
};