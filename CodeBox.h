#ifndef CODE_BOX_H
#define CODE_BOX_H

#include "Global.h"

class CodeBox {
public:
	CodeBox(sf::Vector2f position = sf::Vector2f(0, 0));

	void update(std::vector <std::string> code);

	void draw(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Sprite* barSprite, sf::Text* codeText, bool withoutBar, float dummy = 1, bool isDrawn = true);
	void drawFadeIn(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Sprite* barSprite, sf::Text* codeText, int line, float ratio, bool isDrawn);
	void drawFadeOut(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Sprite* barSprite, sf::Text* codeText, int line, float ratio, bool isDrawn);
	void drawMove(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Sprite* barSprite, sf::Text* codeText, int fromLine, int toLine, float ratio, bool isDrawn);

private:
	std::vector <std::string> code;
	sf::Vector2f position;
};

#endif