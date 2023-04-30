#ifndef DESCRIPTION_BOX_H
#define DESCRIPTION_BOX_H

#include "Global.h"
#include "Assets.h"

class DescriptionBox {
public:
	DescriptionBox();
	DescriptionBox(Assets* assets, sf::Vector2f position);

	void newOperation(std::string operation);
	void addDescription(std::vector <std::string> description);
	int size();

	void draw(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Text* operationText, sf::Text* descriptionText, int line, float dummy, bool isDrawn);
	void drawFadeIn(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Text* operationText, sf::Text* descriptionText, int line, float ratio, bool isDrawn);
	void drawFadeOut(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Text* operationText, sf::Text* descriptionText, int line, float ratio, bool isDrawn);
	void drawChange(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Text* operationText, sf::Text* descriptinText, int fromLine, int toLine, float ratio, bool isDrawn);

private:
	Assets* assets;
	std::string operation;
	std::vector <std::vector <std::string> > description;
	sf::Vector2f position;
};

#endif