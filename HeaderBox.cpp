#include "HeaderBox.h"

HeaderBox::HeaderBox() {
}

HeaderBox::HeaderBox(Assets* assets, std::string name, sf::Vector2f position) {
	this->assets = assets;
	this->name = name;
	this->position = position;
}

bool HeaderBox::handleEvent(sf::RenderWindow* window, sf::Event* event) {
	if (event->type == sf::Event::MouseButtonReleased) {
		if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 10, position.y + 10, 40, 40))) {
			return true;
		}

		if (positionInRect(sf::Mouse::getPosition(*window), sf::FloatRect(position.x + 1530, position.y + 10, 40, 40))) {
			assets->switchMode();
		}
	}

	return false;
}

void HeaderBox::draw(sf::RenderWindow* window) {
	sf::Sprite* boxSprite = &assets->box1580x60Sprite;
	sf::Sprite* backSprite = &assets->backButtonSprite;
	sf::Sprite* modeSprite = &assets->modeButtonSprite;
	sf::Text* text = &assets->hyperspaceItalicText;

	boxSprite->setColor(assets->boxColor2);
	boxSprite->setPosition(position);
	window->draw(*boxSprite);

	text->setString(name);
	text->setCharacterSize(40);
	text->setFillColor(assets->boxTextColor1);
	text->setOrigin(text->getLocalBounds().left + text->getLocalBounds().width / 2, text->getLocalBounds().top + text->getLocalBounds().height / 2);
	text->setPosition(boxSprite->getGlobalBounds().left + boxSprite->getGlobalBounds().width / 2, boxSprite->getGlobalBounds().top + boxSprite->getGlobalBounds().height / 2);
	window->draw(*text);
	
	backSprite->setPosition(position + sf::Vector2f(10, 10));
	if (positionInRect(sf::Mouse::getPosition(*window), backSprite->getGlobalBounds())) {
		backSprite->setColor(assets->boxColor4);
	} else {
		backSprite->setColor(assets->boxColor3);
	}

	window->draw(*backSprite);
	
	modeSprite->setPosition(position + sf::Vector2f(1530, 10));
	if (positionInRect(sf::Mouse::getPosition(*window), modeSprite->getGlobalBounds())) {
		modeSprite->setColor(assets->boxColor4);
	} else {
		modeSprite->setColor(assets->boxColor3);
	}

	window->draw(*modeSprite);
}