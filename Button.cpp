#include "Button.h"

Button::Button() {
}

void Button::setSprite(sf::Sprite* sprite) {
	this->sprite = sprite;
}

void Button::setText(sf::Text* text, std::string name, int characterSize) {
	this->text = text;
	this->name = name;
	this->characterSize = characterSize;
}

void Button::setPosition(sf::Vector2f position) {
	this->position = position;
}

sf::FloatRect Button::getGlobalBounds() {
	sprite->setPosition(position);

	return sprite->getGlobalBounds();
}

void Button::draw(sf::RenderWindow* window) {
	sprite->setPosition(position);
	if (positionInRect(sf::Mouse::getPosition(*window), sprite->getGlobalBounds())) {
		sprite->setColor(BOX_COLOR_4);	
	} else {
		sprite->setColor(BOX_COLOR_3);
	}

	window->draw(*sprite);

	text->setString(name);
	text->setCharacterSize(characterSize);
	text->setOrigin(text->getLocalBounds().left + text->getLocalBounds().width / 2, 0);
	text->setPosition(sprite->getGlobalBounds().left + sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().top + 10);
	if (positionInRect(sf::Mouse::getPosition(*window), sprite->getGlobalBounds())) {
		text->setFillColor(BOX_TEXT_COLOR_2);	
	} else {
		text->setFillColor(BOX_TEXT_COLOR_1);
	}

	window->draw(*text);
}