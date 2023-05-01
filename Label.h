#ifndef LABEL_H
#define LABEL_H

#include "Global.h"
#include "Node.h"
#include "Cell.h"

struct Label {
	Node* node;
	Cell* cell;

	Label(Node* node = nullptr, Cell* cell = nullptr);

	void draw(sf::RenderWindow* window, sf::Text* text, sf::Color* color, std::string name, float dummy = 1, bool isDrawn = true);
	void drawFadeIn(sf::RenderWindow* window, sf::Text* text, sf::Color* color, std::string name, float ratio, bool isDrawn);
	void drawFadeOut(sf::RenderWindow* window, sf::Text* text, sf::Color* color, std::string name, float ratio, bool isDrawn);
	void drawChange(sf::RenderWindow* window, sf::Text* text, sf::Color* color, std::string fromName, std::string toName, float ratio, bool isDrawn);
};

#endif