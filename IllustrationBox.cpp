#include "IllustrationBox.h"

IllustrationBox::IllustrationBox() {
}

IllustrationBox::IllustrationBox(sf::Sprite* sprite, std::string name, sf::Vector2f position) {
	this->sprite = sprite;
	this->name = name;
	this->position = position;
}

void IllustrationBox::draw(sf::RenderWindow* window, Assets* assets) {
	sf::Sprite* boxSprite = &assets->box320x240Sprite;
	sf::Text* text = &assets->hyperspaceItalicText;
	
	boxSprite->setPosition(position);
	if (positionInRect(sf::Mouse::getPosition(*window), boxSprite->getGlobalBounds())) {
		boxSprite->setColor(assets->boxColor2);
		assets->setCursor(2);
	} else {
		boxSprite->setColor(assets->boxColor1);
	}

	window->draw(*boxSprite);
	
	sprite->setPosition(position + sf::Vector2f(10, 10));
	window->draw(*sprite);

	sf::RectangleShape seperatorLine(sf::Vector2f(240, 2));
	if (positionInRect(sf::Mouse::getPosition(*window), boxSprite->getGlobalBounds())) {
		seperatorLine.setFillColor(assets->seperatorLineColor2);
	} else {
		seperatorLine.setFillColor(assets->seperatorLineColor1);
	}

	seperatorLine.setOrigin(seperatorLine.getSize().x / 2, seperatorLine.getSize().y / 2);
	seperatorLine.setPosition(position + sf::Vector2f(160, 180));
	window->draw(seperatorLine);

	text->setString(name);
	text->setCharacterSize(25);
	text->setOrigin(text->getLocalBounds().left + text->getLocalBounds().width / 2, 0);
	text->setFillColor(assets->boxTextColor1);
	text->setPosition(position + sf::Vector2f(160, 190));
	window->draw(*text);
}

bool IllustrationBox::handleEvent(sf::RenderWindow* window, sf::Event* event) {
	if (event->type == sf::Event::MouseButtonReleased) {
		if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x, position.y, 320, 240))) {
			return true;
		}
	}

	return false;
}