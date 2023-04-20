#ifndef NODE_H
#define NODE_H

#include "Global.h"

struct Node {
	int value;
	sf::Vector2f position;

	Node(int value = 0, sf::Vector2f position = sf::Vector2f(0, 0));

	void draw(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* circleColor, sf::Text* valueText, sf::Color* valueColor, float dummy = 1, bool isDrawn = true);
	void drawFadeIn(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* circleColor, sf::Text* valueText, sf::Color* valueColor, float ratio, bool isDrawn);
	void drawFadeOut(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* circleColor, sf::Text* valueText, sf::Color* valueColor, float ratio, bool isDrawn);
	void drawMove(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* circleColor, sf::Text* valueText, sf::Color* valueColor, sf::Vector2f fromPosition, sf::Vector2f toPosition, float ratio, bool isDrawn);
	void drawSlide(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* circleColor, sf::Text* valueText, sf::Color* valueColor, Node* fromNode, Node* toNode, float ratio, bool isDrawn);
	void drawUpdate(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* circleColor, sf::Text* valueText, sf::Color* valueColor, int fromValue, int toValue, float ratio, bool isDrawn);
	void drawChange(sf::RenderWindow* window, sf::Sprite* circleSprite, sf::Color* fromCircleColor, sf::Color* toCircleColor, sf::Text* valueText, sf::Color* fromValueColor, sf::Color* toValueColor, float ratio, bool isDrawn);
};

#endif