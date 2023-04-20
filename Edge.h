#ifndef EDGE_H
#define EDGE_H

#include "Global.h"
#include "Node.h"

struct Edge {
	Node* left;
	Node* right;
	float length, angle;
	sf::Vector2f position;

	Edge(Node* left = nullptr, Node* right = nullptr);

	void calculate();

	void draw(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float dummy = 1, bool isDrawn = true);
	void drawFadeOut(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float ratio, bool isDrawn);
	void drawSlideIn(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float ratio, bool isDrawn);
	void drawSlideOut(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float ratio, bool isDrawn);
	void drawChange(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* fromColor, sf::Color* toColor, float ratio, bool isDrawn);
};

#endif