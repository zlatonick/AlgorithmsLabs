#include "Object.h"
#include "TextureLoader.h"

Object::Object(int id) {
	TextureLoader *textureLoader = TextureLoader::getObject();
	sf::Texture *texture = textureLoader->getTexture(id);

	this->sprite = new sf::Sprite();
	this->sprite->setTexture(*texture);
}

Object::~Object() {
	delete this->sprite;
}

void Object::setPosition(float _x, float _y) {
	this->sprite->setPosition(_x, _y);
}

void Object::display(sf::RenderWindow *window, unsigned int time) {
	window->draw(*(this->sprite));
}

bool Object::mouseClick(float x, float y) {
	if (this->sprite->getGlobalBounds().contains(x, y)) {
		return true;
	}
	return false;
}