#ifndef DATA_H
#define DATA_H

#include "Global.h"

class Assets {
public:
	// Font
	sf::Font arialFont;
	sf::Font consolasFont;
	sf::Font consolasBoldFont;
	sf::Font hyperspaceItalicFont;

	// Text
	sf::Text arialText;
	sf::Text consolasText;
	sf::Text consolasBoldText;
	sf::Text hyperspaceItalicText;

	// Texture
	sf::Texture lightBackgroundTexture;
	sf::Texture darkBackgroundTexture;
	sf::Texture hollowCircleTexture;
	sf::Texture solidCircleTexture;
	sf::Texture hollowSquareTexture;
	sf::Texture solidSquareTexture;
	sf::Texture stickTexture;
	sf::Texture arrowTexture;
	sf::Texture box680x40Texture;
	sf::Texture box680x300Texture;
	sf::Texture box680x140Texture;
	sf::Texture box880x60Texture;
	sf::Texture box880x380Texture;
	sf::Texture box1580x60Texure;
	sf::Texture box1580x320Texure;
	sf::Texture skewBox100x40Texture;
	sf::Texture skewBox160x60Texture;
	sf::Texture skewBox400x60Texture;
	sf::Texture skewBox560x60Texture;
	sf::Texture backButtonTexture;
	sf::Texture modeButtonTexture;
	sf::Texture prevButtonTexture;
	sf::Texture nextButtonTexture;
	sf::Texture controlButtonsTexture;
	sf::Texture pasuseButtonTexture;
	sf::Texture continueButtonTexture;
	sf::Texture statusButtonsTexture;
	sf::Texture progressBarTexture;
	sf::Texture speedTexture;

	// Sprite
	sf::Sprite backgroundSprite;
	sf::Sprite hollowCircleSprite;
	sf::Sprite solidCircleSprite;
	sf::Sprite hollowSquareSprite;
	sf::Sprite solidSquareSprite;
	sf::Sprite stickSprite;
	sf::Sprite arrowSprite;
	sf::Sprite box680x40Sprite;
	sf::Sprite box680x300Sprite;
	sf::Sprite box680x140Sprite;
	sf::Sprite box880x60Sprite;
	sf::Sprite box880x380Sprite;
	sf::Sprite box1580x60Sprite;
	sf::Sprite box1580x320Sprite;
	sf::Sprite skewBox100x40Sprite;
	sf::Sprite skewBox160x60Sprite;
	sf::Sprite skewBox400x60Sprite;
	sf::Sprite skewBox560x60Sprite;
	sf::Sprite backButtonSprite;
	sf::Sprite modeButtonSprite;
	sf::Sprite prevButtonSprite;
	sf::Sprite nextButtonSprite;
	sf::Sprite controlButtonsSprite;
	sf::Sprite statusButtonsSprite;
	sf::Sprite progressBarSprite;
	sf::Sprite speedSprite;

	// Color
	sf::Color blankColor;

	sf::Color normalNodeCircleColor;
	sf::Color highlightNodeCircleColor1;
	sf::Color highlightNodeCircleColor2;
	sf::Color normalNodeTextColor;
	sf::Color highlightNodeTextColor1;
	sf::Color highlightNodeTextColor2;
	sf::Color insertedNodeCircleColor;
	sf::Color erasedNodeCircleColor;

	sf::Color normalCellSquareColor;
	sf::Color blurCellSquareColor;
	sf::Color highlightCellSquareColor1;
	sf::Color normalCellTextColor;
	sf::Color blurCellTextColor;
	sf::Color highlightCellTextColor1;
	sf::Color highlightCellTextColor2;
	sf::Color insertedCellSquareColor;
	sf::Color insertedCellTextColor;
	sf::Color erasedCellSquareColor;
	sf::Color erasedCellTextColor;

	sf::Color labelColor;

	sf::Color normalEdgeColor;
	sf::Color highlightEdgeColor;
	sf::Color insertedEdgeColor;
	sf::Color erasedEdgeColor;

	sf::Color boxColor1;
	sf::Color boxColor2;
	sf::Color boxColor3;
	sf::Color boxColor4;
	sf::Color boxColor5;
	sf::Color boxColor6;
	sf::Color boxTextColor1;
	sf::Color boxTextColor2;
	sf::Color seperatorLineColor1;
	sf::Color seperatorLineColor2;

	Assets();

	void switchMode();

private:
	enum { LIGHT, DARK } mode;

	void loadOne(sf::Font* font, sf::Text* text, std::string filename);
	void loadOne(sf::Texture* texture, sf::Sprite* sprite, std::string filename);
	void loadAll();
	void fix();
};

#endif