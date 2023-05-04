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

void Button::draw(sf::RenderWindow* window, Assets* assets) {
	sprite->setPosition(position);
	if (positionInRect(sf::Mouse::getPosition(*window), sprite->getGlobalBounds())) {
		sprite->setColor(assets->boxColor4);	
	} else {
		if (name == "GO") {
			sprite->setColor(assets->boxColor5);
		} else if (name == "Unavailable") {
			sprite->setColor(assets->boxColor6);
		} else {
			sprite->setColor(assets->boxColor3);
		}
	}

	window->draw(*sprite);

	text->setString(name);
	text->setCharacterSize(characterSize);
	text->setOrigin(text->getLocalBounds().left + text->getLocalBounds().width / 2, text->getLocalBounds().top + text->getLocalBounds().height / 2);
	text->setPosition(sprite->getGlobalBounds().left + sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().top + sprite->getGlobalBounds().height / 2);
	if (positionInRect(sf::Mouse::getPosition(*window), sprite->getGlobalBounds())) {
		text->setFillColor(assets->boxTextColor2);	
	} else {
		text->setFillColor(assets->boxTextColor1);
	}

	window->draw(*text);
}