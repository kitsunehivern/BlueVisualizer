#ifndef CODE_BOX_H
#define CODE_BOX_H

#include "Global.h"
#include "Assets.h"

class CodeBox {
public:
	CodeBox();
	CodeBox(Assets* assets, sf::Vector2f position);

	void update(std::vector <std::string> code);

	void draw(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Sprite* barSprite, sf::Text* codeText, bool withoutBar, float dummy = 1, bool isDrawn = true);
	void drawFadeIn(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Sprite* barSprite, sf::Text* codeText, int line, float ratio, bool isDrawn);
	void drawFadeOut(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Sprite* barSprite, sf::Text* codeText, int line, float ratio, bool isDrawn);
	void drawMove(sf::RenderWindow* window, sf::Sprite* boxSprite, sf::Sprite* barSprite, sf::Text* codeText, int fromLine, int toLine, float ratio, bool isDrawn);

private:
	Assets* assets;
	std::vector <std::string> code;
	sf::Vector2f position;
};

#endif