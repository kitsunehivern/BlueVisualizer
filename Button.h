#ifndef BUTTON_H
#define BUTTON_H

#include "Global.h"

class Button : public sf::Drawable {
public:
	Button();

	void setSprite(sf::Sprite* sprite);
	void setText(sf::Text* text, std::string name, int characterSize);
	void setFillColor(sf::Color* spriteColor, sf::Color* textColor);
	void setPosition(sf::Vector2f position);
	sf::FloatRect getGlobalBounds();

private:
	sf::Sprite* sprite;
	sf::Text* text; 
	std::string name;
	int characterSize;
	sf::Color* spriteColor;
	sf::Color* textColor;
	sf::Vector2f position;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
};

#endif