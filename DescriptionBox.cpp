#include "DescriptionBox.h"
#include "Motion.h"

DescriptionBox::DescriptionBox(sf::Vector2f position) {
	this->position = position;
}

void DescriptionBox::newOperation(std::string operation) {
	this->operation = operation;
	this->description.clear();
}

void DescriptionBox::addDescription(std::vector <std::string> description) {
	this->description.push_back(description);
}

int DescriptionBox::size() {
	return description.size();
}

void DescriptionBox::draw(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Text* operationText, sf::Text* descriptionText, int line, float dummy, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	boxSprite->setColor(BOX_COLOR_2);
	boxSprite->setPosition(position);
	window->draw(*boxSprite);

	sf::RectangleShape seperatorLine(sf::Vector2f(580, 2));
	seperatorLine.setFillColor(SEPERATOR_LINE_COLOR_2);
	seperatorLine.setOrigin(seperatorLine.getSize().x / 2, seperatorLine.getSize().y / 2);
	seperatorLine.setPosition(position + sf::Vector2f(340, 50));
	window->draw(seperatorLine);

	if (line == -1) {
		return;
	}

	operationText->setString(operation);
	operationText->setCharacterSize(30);
	operationText->setFillColor(BOX_TEXT_COLOR);
	operationText->setOrigin(operationText->getLocalBounds().left + operationText->getLocalBounds().width / 2, 0);
	operationText->setPosition(position + sf::Vector2f(340, 4));
	window->draw(*operationText);

	for (int i = 0; i < description[line].size(); i++) {
		descriptionText->setString(description[line][i]);
		descriptionText->setCharacterSize(25);
		descriptionText->setFillColor(BOX_TEXT_COLOR);
		descriptionText->setOrigin(0, 0);
		descriptionText->setPosition(position + sf::Vector2f(15, 60 + 40 * i));
		window->draw(*descriptionText);
	}
}

void DescriptionBox::drawFadeIn(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Text* operationText, sf::Text* descriptionText, int line, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	boxSprite->setColor(BOX_COLOR_2);
	boxSprite->setPosition(position);
	window->draw(*boxSprite);

	operationText->setString(operation);
	operationText->setCharacterSize(30);
	operationText->setFillColor(BOX_TEXT_COLOR);
	operationText->setOrigin(operationText->getLocalBounds().left + operationText->getLocalBounds().width / 2, 0);
	operationText->setPosition(position + sf::Vector2f(340, 4));
	window->draw(*operationText);

	sf::RectangleShape seperatorLine(sf::Vector2f(580, 2));
	seperatorLine.setFillColor(SEPERATOR_LINE_COLOR_2);
	seperatorLine.setOrigin(seperatorLine.getSize().x / 2, seperatorLine.getSize().y / 2);
	seperatorLine.setPosition(position + sf::Vector2f(340, 50));
	window->draw(seperatorLine);

	for (int i = 0; i < description[line].size(); i++) {
		descriptionText->setString(description[line][i]);
		descriptionText->setCharacterSize(25);
		descriptionText->setFillColor(BOX_TEXT_COLOR * sf::Color(255, 255, 255, std::round(Motion::Bezier(ratio) * 255)));
		descriptionText->setOrigin(0, 0);
		descriptionText->setPosition(position + sf::Vector2f(15, 60 + 40 * i));
		window->draw(*descriptionText);
	}
}

void DescriptionBox::drawFadeOut(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Text* operationText, sf::Text* descriptionText, int line, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	drawFadeIn(window, boxSprite, operationText, descriptionText, line, 1.0f - ratio, isDrawn);
}

void DescriptionBox::drawChange(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Text* operationText, sf::Text* descriptinText, int fromLine, int toLine, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	if (ratio <= 0.25f) {
		draw(window, boxSprite, operationText, descriptinText, fromLine, ratio, isDrawn);
	} else if (ratio <= 0.5f) {
		drawFadeOut(window, boxSprite, operationText, descriptinText, fromLine, 4.0f * (ratio - 0.25), isDrawn);
	} else if (ratio <= 0.75f) {
		drawFadeIn(window, boxSprite, operationText, descriptinText, toLine, 4.0f * (ratio - 0.5f), isDrawn);
	} else {
		draw(window, boxSprite, operationText, descriptinText, toLine, ratio, isDrawn);
	}
}