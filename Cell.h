#ifndef CELL_H
#define CELL_H

#include "Global.h"

static const float CELL_POSITION_X = 140.0f;
static const float CELL_POSITION_Y = 160.0f;
static const float CELL_DISTANCE_X = 30.0f;
static const float CELL_DISTANCE_Y = 60.0f;

struct Cell {
	int value;
	sf::Vector2f position;

	Cell(int value = 0, sf::Vector2f position = sf::Vector2f(0, 0));

	void draw(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* squareColor, sf::Text* valueText, sf::Color* valueColor, float dummy = 1, bool isDrawn = true);
	void drawFadeIn(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* squareColor, sf::Text* valueText, sf::Color* valueColor, float ratio, bool isDrawn);
	void drawFadeOut(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* squareColor, sf::Text* valueText, sf::Color* valueColor, float ratio, bool isDrawn);
	void drawMove(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* squareColor, sf::Text* valueText, sf::Color* valueColor, sf::Vector2f fromPosition, sf::Vector2f toPosition, float ratio, bool isDrawn);
	void drawFadeInAndUpdate(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* squareColor, sf::Text* valueText, sf::Color* valueColor, int fromValue, int toValue, float ratio, bool isDrawn);
	void drawChangeAndUpdate(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Text* valueText, sf::Color* fromValueColor, sf::Color* toValueColor, int fromValue, int toValue, float ratio, bool isDrawn);
	void drawChange(sf::RenderWindow* window, sf::Sprite* squareSprite, sf::Color* fromSquareColor, sf::Color* toSquareColor, sf::Text* valueText, sf::Color* fromValueColor, sf::Color* toValueColor, float ratio, bool isDrawn);
};

#endif