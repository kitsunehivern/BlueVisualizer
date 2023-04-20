#ifndef DOUBLY_EDGE_H
#define DOUBLY_EDGE_H

#include "Global.h"
#include "Node.h"

struct DoublyEdge {
	Node* left;
	Node* right;
	float length, angle;
	sf::Vector2f position;

	DoublyEdge(Node* left = nullptr, Node* right = nullptr);

	void calculate();

	void draw(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* leftColor, sf::Color* rightColor, float dummy = 1, bool isDrawn = true);
	void drawSlideIn(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* leftColor, sf::Color* rightColor, float ratio, bool isDrawn);
	void drawSlideOut(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* leftColor, sf::Color* rightColor, float ratio, bool isDrawn);
	void drawChange(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* fromLeftColor, sf::Color* toLeftColor, sf::Color* fromRightColor, sf::Color* toRightColor, float ratio, bool isDrawn);
};

#endif