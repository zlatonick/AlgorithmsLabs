#pragma once
#include "globals.h"

class TextureLoader {
	static TextureLoader* thisObject;

	std::map<int, sf::Texture*> textures;
	int lastID;

public:
	TextureLoader();
	~TextureLoader();

	static TextureLoader* getObject() { return thisObject; }

	// Add texture to the hash table (false if failed)
	bool addTexture(char *fileName);

	// Get texture by id
	sf::Texture* getTexture(unsigned int id);
};