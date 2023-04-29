#include "HeaderBox.h"

HeaderBox::HeaderBox() {
}

HeaderBox::HeaderBox(Assets* assets, std::string name, sf::Vector2f position) {
	this->assets = assets;
	this->name = name;
	this->position = position;
}

bool HeaderBox::handleEvent(sf::RenderWindow* window, sf::Event* event) {
	return false;
}

void HeaderBox::draw(sf::RenderWindow* window) {
	sf::Sprite* boxSprite = &assets->box1580x60Sprite;
	sf::Sprite* backSprite = &assets->backButtonSprite;
	sf::Sprite* modeSprite = &assets->modeButtonSprite;
	sf::Text* text = &assets->hyperspaceItalicText;

	boxSprite->setColor(BOX_COLOR_2);
	boxSprite->setPosition(position);
	window->draw(*boxSprite);

	text->setString(name);
	text->setCharacterSize(40);
	text->setFillColor(BOX_TEXT_COLOR_1);
	text->setOrigin(text->getLocalBounds().left + text->getLocalBounds().width / 2, text->getLocalBounds().top + text->getLocalBounds().height / 2);
	text->setPosition(boxSprite->getGlobalBounds().left + boxSprite->getGlobalBounds().width / 2, boxSprite->getGlobalBounds().top + boxSprite->getGlobalBounds().height / 2);
	window->draw(*text);
	
	backSprite->setPosition(position + sf::Vector2f(20, 10));
	if (positionInRect(sf::Mouse::getPosition(*window), backSprite->getGlobalBounds())) {
		backSprite->setColor(BOX_COLOR_4);
	} else {
		backSprite->setColor(BOX_COLOR_3);
	}

	window->draw(*backSprite);
	
	modeSprite->setPosition(position + sf::Vector2f(1520, 10));
	backSprite->setPosition(position + sf::Vector2f(20, 10));
	if (positionInRect(sf::Mouse::getPosition(*window), modeSprite->getGlobalBounds())) {
		modeSprite->setColor(BOX_COLOR_4);
	} else {
		modeSprite->setColor(BOX_COLOR_3);
	}

	window->draw(*modeSprite);
}