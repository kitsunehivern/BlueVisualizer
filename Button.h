#ifndef BUTTON_H
#define BUTTON_H

#include "Global.h"
#include "Assets.h"

class Button {
public:
	Button();

	void setSprite(sf::Sprite* sprite);
	void setText(sf::Text* text, std::string name, int characterSize);
	void setPosition(sf::Vector2f position);
	sf::FloatRect getGlobalBounds();

	void draw(sf::RenderWindow* window, Assets* assets);

private:
	sf::Sprite* sprite;
	sf::Text* text; 
	std::string name;
	int characterSize;
	sf::Vector2f position;
};

#endif