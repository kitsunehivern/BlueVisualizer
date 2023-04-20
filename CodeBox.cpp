#include "CodeBox.h"
#include "Motion.h"

CodeBox::CodeBox(sf::Vector2f position) {
	this->position = position;
}

void CodeBox::update(std::vector <std::string> code) {
	this->code = code;
}

void CodeBox::draw(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Sprite* barSprite, sf::Text* codeText, bool withoutBar, float dummy, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	boxSprite->setPosition(position);
	boxSprite->setColor(BOX_COLOR_2);
	window->draw(*boxSprite);

	if (!withoutBar) {
		barSprite->setColor(CODE_BAR_COLOR);
		window->draw(*barSprite);
	}

	for (int i = 0; i < code.size(); i++) {
		codeText->setString(code[i]);
		codeText->setCharacterSize(25);
		codeText->setFillColor(CODE_TEXT_COLOR);
		codeText->setPosition(position + sf::Vector2f(15, 10 + 40 * i + 4));
		window->draw(*codeText);
	}
}

void CodeBox::drawFadeIn(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Sprite* barSprite, sf::Text* codeText, int line, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	boxSprite->setPosition(position);
	boxSprite->setColor(BOX_COLOR_2);
	window->draw(*boxSprite);

	barSprite->setColor(CODE_BAR_COLOR * sf::Color(255, 255, 255, std::round(Motion::Bezier(ratio) * 255)));
	barSprite->setPosition(position + sf::Vector2f(0, 10 + 40 * line));
	window->draw(*barSprite);

	for (int i = 0; i < code.size(); i++) {
		codeText->setString(code[i]);
		codeText->setCharacterSize(25);
		codeText->setFillColor(CODE_TEXT_COLOR);
		codeText->setPosition(position + sf::Vector2f(15, 10 + 40 * i + 4));
		window->draw(*codeText);
	}
}

void CodeBox::drawFadeOut(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Sprite* barSprite, sf::Text* codeText, int line, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	drawFadeIn(window, boxSprite, barSprite, codeText, line, 1.0f - ratio, isDrawn);
}

void CodeBox::drawMove(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Sprite* barSprite, sf::Text* codeText, int fromLine, int toLine, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	if (ratio <= 0.25f) {
		barSprite->setPosition(position + sf::Vector2f(0, 10 + 40 * fromLine));
		draw(window, boxSprite, barSprite, codeText, false);
	} else if (ratio <= 0.75) {
		barSprite->setPosition(position + sf::Vector2f(0, 10 + 40 * (fromLine + Motion::Bezier(2 * (ratio - 0.25f)) * (toLine - fromLine))));
		draw(window, boxSprite, barSprite, codeText, false);
	} else {
		barSprite->setPosition(position + sf::Vector2f(0, 10 + 40 * toLine));
		draw(window, boxSprite, barSprite, codeText, false);
	}
}