#ifndef DATA_H
#define DATA_H

#include "Global.h"

struct Assets {
	sf::Font arialFont;
	sf::Font consolasFont;
	sf::Font consolasBoldFont;
	sf::Font hyperspaceItalicFont;

	sf::Text arialText;
	sf::Text consolasText;
	sf::Text consolasBoldText;
	sf::Text hyperspaceItalicText;

	sf::Texture hollowCircleTexture;
	sf::Texture solidCircleTexture;
	sf::Texture stickTexture;
	sf::Texture arrowTexture;
	sf::Texture box680x40Texture;
	sf::Texture box680x300Texture;
	sf::Texture box680x140Texture;
	sf::Texture box880x60Texture;
	sf::Texture box880x380Texture;
	sf::Texture skewBox100x40Texture;
	sf::Texture skewBox160x60Texture;
	sf::Texture skewBox400x60Texture;
	sf::Texture skewBox560x60Texture;
	sf::Texture prevButtonTexture;
	sf::Texture nextButtonTexture;
	sf::Texture controlButtonsTexture;
	sf::Texture pasuseButtonTexture;
	sf::Texture continueButtonTexture;
	sf::Texture statusButtonsTexture;
	sf::Texture progressBarTexture;
	sf::Texture speedTexture;

	sf::Sprite hollowCircleSprite;
	sf::Sprite solidCircleSprite;
	sf::Sprite stickSprite;
	sf::Sprite arrowSprite;
	sf::Sprite box680x40Sprite;
	sf::Sprite box680x300Sprite;
	sf::Sprite box680x140Sprite;
	sf::Sprite box880x60Sprite;
	sf::Sprite box880x380Sprite;
	sf::Sprite skewBox100x40Sprite;
	sf::Sprite skewBox160x60Sprite;
	sf::Sprite skewBox400x60Sprite;
	sf::Sprite skewBox560x60Sprite;
	sf::Sprite prevButtonSprite;
	sf::Sprite nextButtonSprite;
	sf::Sprite controlButtonsSprite;
	sf::Sprite statusButtonsSprite;
	sf::Sprite progressBarSprite;
	sf::Sprite speedSprite;

	Assets();

	void loadOne(sf::Font* font, sf::Text* text, std::string filename);
	void loadOne(sf::Texture* texture, sf::Sprite* sprite, std::string filename);
	void loadAll();
	void fix();
};

#endif