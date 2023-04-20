#include "Label.h"
#include "Motion.h"

Label::Label(Node* node) {
	this->node = node;
}

void Label::draw(sf::RenderWindow* window, sf::Text* text, sf::Color* color, std::string name, float dummy, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	text->setFillColor(*color);
	text->setString(name);
	text->setOrigin(text->getLocalBounds().left + text->getLocalBounds().width / 2, 0);
	text->setPosition(node->position + sf::Vector2f(30, 60));
	window->draw(*text);
}

void Label::drawFadeIn(sf::RenderWindow* window, sf::Text* text, sf::Color* color, std::string name, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	sf::Color newColor = (*color) * sf::Color(255, 255, 255, std::round(Motion::Bezier(ratio) * 255));
	draw(window, text, &newColor, name);
}

void Label::drawFadeOut(sf::RenderWindow* window, sf::Text* text, sf::Color* color, std::string name, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	drawFadeIn(window, text, color, name, 1 - ratio, isDrawn);
}

void Label::drawChange(sf::RenderWindow* window, sf::Text* text, sf::Color* color, std::string fromName, std::string toName, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	if (ratio <= 0.5f) {
		drawFadeOut(window, text, color, fromName, 2 * ratio, isDrawn);
	} else {
		drawFadeIn(window, text, color, toName, 2 * (ratio - 0.5f), isDrawn);
	}
}