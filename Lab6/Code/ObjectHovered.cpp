#include "ObjectHovered.h"
#include "TextureLoader.h"


ObjectHovered::ObjectHovered(int id, int idHovered) : Object(id) {

	sf::Texture *texture = TextureLoader::getObject()->getTexture(idHovered);

	this->spriteHovered = new sf::Sprite();
	this->spriteHovered->setTexture(*texture);

	this->isHovered = false;
}


ObjectHovered::~ObjectHovered() {
	delete this->spriteHovered;
}


void ObjectHovered::setPosition(float _x, float _y) {
	this->sprite->setPosition(_x, _y);
	this->spriteHovered->setPosition(_x, _y);
}


void ObjectHovered::display(sf::RenderWindow *window, unsigned int _time) {
	
	if (this->isHovered) {
		window->draw(*(this->spriteHovered));
	}
	else {
		window->draw(*(this->sprite));
	}
}


bool ObjectHovered::mouseClick(float x, float y) {
	
	if (this->sprite->getGlobalBounds().contains(x, y)) {
		return true;
	}
	return false;
}


void ObjectHovered::setIsHovered(bool _isHovered) {
	this->isHovered = _isHovered;
}