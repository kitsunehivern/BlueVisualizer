#include "Cell.h"
#include "Motion.h"

Cell::Cell(int value, sf::Vector2f position) {
	this->value = value;
	this->position = position;
}

void Cell::draw(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* squareColor, sf::Text* valueText, sf::Color* valueColor, float dummy, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	squareSprite->setColor(*squareColor);
	squareSprite->setPosition(position);
	window->draw(*squareSprite);

	valueText->setCharacterSize(25);
	valueText->setFillColor(*valueColor);
	valueText->setString(std::to_string(value));
	valueText->setOrigin(valueText->getLocalBounds().left + valueText->getLocalBounds().width / 2, valueText->getLocalBounds().top + valueText->getLocalBounds().height / 2);
	valueText->setPosition(position + sf::Vector2f(30, 30));
	window->draw(*valueText);
}

void Cell::drawFadeIn(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* squareColor, sf::Text* valueText, sf::Color* valueColor, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	sf::Color newSquareColor = (*squareColor) * sf::Color(255, 255, 255, std::round(Motion::Bezier(ratio) * 255));
	sf::Color newValueColor = (*valueColor) * sf::Color(255, 255, 255, std::round(Motion::Bezier(ratio) * 255));

	draw(window, squareSprite, &newSquareColor, valueText, &newValueColor);
}

void Cell::drawFadeOut(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* squareColor, sf::Text* valueText, sf::Color* valueColor, float ratio, bool isDrawn) {
	if (!isDrawn) {
		return;
	}

	drawFadeIn(window, squareSprite, squareColor, valueText, valueColor, 1 - ratio, isDrawn);
}

void Cell::drawMove(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* squareColor, sf::Text* valueText, sf::Color* valueColor, sf::Vector2f fromPosition, sf::Vector2f toPosition, float ratio, bool isDrawn) {
	position.x = fromPosition.x + Motion::Bezier(ratio) * (toPosition.x - fromPosition.x);
	position.y = fromPosition.y + Motion::Bezier(ratio) * (toPosition.y - fromPosition.y);

	if (!isDrawn) {
		return;
	}

	draw(window, squareSprite, squareColor, valueText, valueColor);
}

void Cell::drawFadeInAndUpdate(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* squareColor, sf::Text* valueText, sf::Color* valueColor, int fromValue, int toValue, float ratio, bool isDrawn) {
	sf::Color newSquareColor = (*squareColor) * sf::Color(255, 255, 255, std::round(Motion::Bezier(ratio) * 255));
	sf::Color newValueColor = (*valueColor) * sf::Color(255, 255, 255, std::round(Motion::Bezier(ratio) * 255));

	if (ratio <= 0.5f) {
		this->value = fromValue;
		newValueColor *= sf::Color(255, 255, 255, std::round(Motion::Bezier(1.0f - 2.0f * ratio) * 255));
		draw(window, squareSprite, &newSquareColor, valueText, &newValueColor, ratio, isDrawn);
	} else {
		this->value = toValue;
		newValueColor *= sf::Color(255, 255, 255, std::round(Motion::Bezier(2.0f * (ratio - 0.5f)) * 255));
		draw(window, squareSprite, &newSquareColor, valueText, &newValueColor, ratio, isDrawn);
	}
}

void Cell::drawChangeAndUpdate(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Text* valueText, sf::Color* fromValueColor, sf::Color* toValueColor, int fromValue, int toValue, float ratio, bool isDrawn) {
	sf::Color newSquareColor = *fromSquareColor;
	newSquareColor.r += std::round(Motion::Bezier(ratio) * (toSquareColor->r - (int)fromSquareColor->r));
	newSquareColor.g += std::round(Motion::Bezier(ratio) * (toSquareColor->g - (int)fromSquareColor->g));
	newSquareColor.b += std::round(Motion::Bezier(ratio) * (toSquareColor->b - (int)fromSquareColor->b));
	newSquareColor.a += std::round(Motion::Bezier(ratio) * (toSquareColor->a - (int)fromSquareColor->a));

	sf::Color newValueColor = *fromValueColor;
	newValueColor.r += std::round(Motion::Bezier(ratio) * (toValueColor->r - (int)fromValueColor->r));
	newValueColor.g += std::round(Motion::Bezier(ratio) * (toValueColor->g - (int)fromValueColor->g));
	newValueColor.b += std::round(Motion::Bezier(ratio) * (toValueColor->b - (int)fromValueColor->b));
	newValueColor.a += std::round(Motion::Bezier(ratio) * (toValueColor->a - (int)fromValueColor->a));
	
	if (ratio <= 0.5f) {
		this->value = fromValue;
		newValueColor *= sf::Color(255, 255, 255, std::round(Motion::Bezier(1.0f - 2.0f * ratio) * 255));
		draw(window, squareSprite, &newSquareColor, valueText, &newValueColor, ratio, isDrawn);
	} else {
		this->value = toValue;
		newValueColor *= sf::Color(255, 255, 255, std::round(Motion::Bezier(2.0f * (ratio - 0.5f)) * 255));
		draw(window, squareSprite, &newSquareColor, valueText, &newValueColor, ratio, isDrawn);
	}
}

void Cell::drawChange(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Text* valueText, sf::Color* fromValueColor, sf::Color* toValueColor, float ratio, bool isDrawn) {	
	if (!isDrawn) {
		return;
	}

	sf::Color newSquareColor = *fromSquareColor;
	newSquareColor.r += std::round(Motion::Bezier(ratio) * (toSquareColor->r - (int)fromSquareColor->r));
	newSquareColor.g += std::round(Motion::Bezier(ratio) * (toSquareColor->g - (int)fromSquareColor->g));
	newSquareColor.b += std::round(Motion::Bezier(ratio) * (toSquareColor->b - (int)fromSquareColor->b));
	newSquareColor.a += std::round(Motion::Bezier(ratio) * (toSquareColor->a - (int)fromSquareColor->a));

	sf::Color newValueColor = *fromValueColor;
	newValueColor.r += std::round(Motion::Bezier(ratio) * (toValueColor->r - (int)fromValueColor->r));
	newValueColor.g += std::round(Motion::Bezier(ratio) * (toValueColor->g - (int)fromValueColor->g));
	newValueColor.b += std::round(Motion::Bezier(ratio) * (toValueColor->b - (int)fromValueColor->b));
	newValueColor.a += std::round(Motion::Bezier(ratio) * (toValueColor->a - (int)fromValueColor->a));

	draw(window, squareSprite, &newSquareColor, valueText, &newValueColor);
}