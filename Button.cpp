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

void Button::setFillColor(sf::Color* spriteColor, sf::Color* textColor) {
	this->spriteColor = spriteColor;
	this->textColor = textColor;
}

void Button::setPosition(sf::Vector2f position) {
	this->position = position;
}

sf::FloatRect Button::getGlobalBounds() {
	sprite->setPosition(position);

	return sprite->getGlobalBounds();
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	sprite->setColor(*spriteColor);
	sprite->setPosition(position);
	target.draw(*sprite, state);

	text->setString(name);
	text->setCharacterSize(characterSize);
	text->setFillColor(*textColor);
	text->setOrigin(text->getLocalBounds().left + text->getLocalBounds().width / 2, 0);
	text->setPosition(sprite->getGlobalBounds().left + sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().top + 10);
	target.draw(*text, state);
}