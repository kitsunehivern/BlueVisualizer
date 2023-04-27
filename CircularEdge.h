#ifndef CIRCULAR_EDGE_H
#define CIRCULAR_EDGE_H

#include "Global.h"
#include "Node.h"

static const float CEDGE_INIT_LENGTH = 50;

struct CircularEdge {
	Node* left;
	Node* right;
	std::array <float, 4> length;
	std::array <sf::Vector2f, 4> position;

	CircularEdge(Node* left = nullptr, Node* right = nullptr);

	void calculate();

	void draw(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float dummy = 1, bool isDrawn = true);
	void drawSlideIn(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float ratio, bool isDrawn);
	void drawSlideOut(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* color, float ratio, bool isDrawn);
	void drawChange(sf::RenderWindow* window, sf::Sprite* stickSprite, sf::Sprite* arrowSprite, sf::Color* fromColor, sf::Color* toColor, float ratio, bool isDrawn);
};

#endif