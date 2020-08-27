#include "TextureLoader.h"

TextureLoader* TextureLoader::thisObject = nullptr;

TextureLoader::TextureLoader()
{
	if (thisObject != nullptr)
		exit(EXIT_FAILURE);

	thisObject = this;

	lastID = -1;
}


TextureLoader::~TextureLoader()
{
	std::map<int, sf::Texture*>::iterator it;

	for (it = this->textures.begin(); it != this->textures.end(); it++)
	{
		delete it->second;
	}
}


bool TextureLoader::addTexture(char *fileName)
{
	sf::Texture *texture = new sf::Texture();

	if (!texture->loadFromFile(fileName))
	{
		return false;
	}

	this->lastID++;

	this->textures.insert(std::pair<int, sf::Texture*>(lastID, texture));

	return true;
}

sf::Texture* TextureLoader::getTexture(unsigned int id)
{
	if (this->textures.count(id) == 0)
	{
		// Texture not found
		exit(EXIT_FAILURE);
	}
	else
	{
		return this->textures[id];
	}
}