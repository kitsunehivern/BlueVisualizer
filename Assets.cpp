#include "Assets.h"

Assets::Assets() {
	loadAll();
	fix();
	mode = DARK;
	switchMode();
}

void Assets::loadOne(sf::Font* font, sf::Text* text, std::string filename) {
	assert(font->loadFromFile("Fonts/" + filename));
	text->setFont(*font);
}

void Assets::loadOne(sf::Texture* texture, sf::Sprite* sprite, std::string filename) {
	assert(texture->loadFromFile("Images/" + filename));
	texture->setSmooth(true);
	sprite->setTexture(*texture, true);
}

void Assets::loadAll() {
	loadOne(&arialFont, &arialText,"arial.ttf");
	loadOne(&consolasFont, &consolasText, "consolas.ttf");
	loadOne(&consolasBoldFont, &consolasBoldText, "consolas_bold.ttf");
	loadOne(&hyperspaceItalicFont, &hyperspaceItalicText, "hyperspace_race_expanded_heavy_italic.ttf");

	loadOne(&lightBackgroundTexture, &backgroundSprite, "LightBackground.png");
	loadOne(&darkBackgroundTexture, &backgroundSprite, "DarkBackground.png");
	loadOne(&hollowCircleTexture, &hollowCircleSprite, "HollowCircle.png");
	loadOne(&solidCircleTexture, &solidCircleSprite, "SolidCircle.png");
	loadOne(&hollowSquareTexture, &hollowSquareSprite, "HollowSquare.png");
	loadOne(&solidSquareTexture, &solidSquareSprite, "SolidSquare.png");
	loadOne(&stickTexture, &stickSprite, "Stick.png");
	loadOne(&arrowTexture, &arrowSprite, "Arrow.png");
	loadOne(&box680x40Texture, &box680x40Sprite, "Box680x40.png");
	loadOne(&box680x300Texture, &box680x300Sprite, "Box680x300.png");
	loadOne(&box680x140Texture, &box680x140Sprite, "Box680x140.png");
	loadOne(&box880x60Texture, &box880x60Sprite, "Box880x60.png");
	loadOne(&box880x380Texture, &box880x380Sprite, "Box880x380.png");
	loadOne(&box1580x60Texure, &box1580x60Sprite, "Box1580x60.png");
	loadOne(&box1580x320Texure, &box1580x320Sprite, "Box1580x320.png");
	loadOne(&skewBox100x40Texture, &skewBox100x40Sprite, "SkewBox100x40.png");
	loadOne(&skewBox160x60Texture, &skewBox160x60Sprite, "SkewBox160x60.png");
	loadOne(&skewBox400x60Texture, &skewBox400x60Sprite, "SkewBox400x60.png");
	loadOne(&skewBox560x60Texture, &skewBox560x60Sprite, "SkewBox560x60.png");
	loadOne(&backButtonTexture, &backButtonSprite, "BackButton.png");
	loadOne(&modeButtonTexture, &modeButtonSprite, "ModeButton.png");
	loadOne(&prevButtonTexture, &prevButtonSprite, "PrevNextButton.png");
	loadOne(&nextButtonTexture, &nextButtonSprite, "PrevNextButton.png");
	loadOne(&controlButtonsTexture, &controlButtonsSprite, "ControlButtons.png");
	loadOne(&statusButtonsTexture, &statusButtonsSprite, "StatusButtons.png");
	loadOne(&progressBarTexture, &progressBarSprite, "ProgressBar.png");
	loadOne(&speedTexture, &speedSprite, "Speed.png");
}

void Assets::fix() {
	stickSprite.setOrigin(0, 2.5f);
	arrowSprite.setOrigin(0, 7.5f);
	prevButtonSprite.setTextureRect(sf::IntRect(0, 0, 60, 60));
	nextButtonSprite.setTextureRect(sf::IntRect(60, 0, 60, 60));
}

void Assets::switchMode() {
	if (mode == DARK) {
		mode = LIGHT;
		backgroundSprite.setTexture(lightBackgroundTexture);

		blankColor                = sf::Color(  0,   0,   0,   0);

		normalNodeCircleColor     = sf::Color(  0,   0,   0, 255);
		highlightNodeCircleColor1 = sf::Color(255, 140,  40, 255);
		highlightNodeCircleColor2 = sf::Color( 45, 185, 210, 255);
		normalNodeTextColor       = sf::Color(  0,   0,   0, 255);
		highlightNodeTextColor1   = sf::Color(255, 255, 255, 255);
		highlightNodeTextColor2   = sf::Color(255, 140,  40, 255);
		insertedNodeCircleColor   = sf::Color( 90, 200,  90, 255);
		erasedNodeCircleColor     = sf::Color(215,  80,  60, 255);

		normalCellSquareColor     = sf::Color(  0,   0,   0, 255);
		blurCellSquareColor       = sf::Color(150, 150, 150, 255);
		highlightCellSquareColor1 = sf::Color(255, 140,  40, 255);
		highlightCellSquareColor2 = sf::Color( 45, 185, 210, 255);
		normalCellTextColor       = sf::Color(  0,   0,   0, 255);
		blurCellTextColor         = sf::Color(150, 150, 150, 255);
		highlightCellTextColor1   = sf::Color(255, 255, 255, 255);
		highlightCellTextColor2   = sf::Color(255, 140,  40, 255);
		insertedCellSquareColor   = sf::Color( 90, 200,  90, 255);
		insertedCellTextColor     = sf::Color( 90, 200,  90, 255);
		erasedCellSquareColor     = sf::Color(215,  80,  60, 255);
		erasedCellTextColor       = sf::Color(215,  80,  60, 255);

		labelColor                = sf::Color(250,  70,  70, 255);

		normalEdgeColor           = sf::Color(  0,   0,   0, 255);
		highlightEdgeColor        = sf::Color(255, 140,  40, 255);
		insertedEdgeColor         = sf::Color( 90, 200,  90, 255);
		erasedEdgeColor           = sf::Color(215,  80,  60, 255);

		boxColor1                 = sf::Color(160, 225, 250, 150);
		boxColor2                 = sf::Color(240, 245, 245, 150);
		boxColor3                 = sf::Color(105, 210, 255, 255);
		boxColor4                 = sf::Color( 45, 125, 185, 255);
		boxColor5                 = sf::Color( 70, 255, 140, 255);
		boxColor6                 = sf::Color(255, 150, 150, 255);
		boxTextColor1             = sf::Color(  0,   0,   0, 255);
		boxTextColor2             = sf::Color(255, 255, 255, 255);
		seperatorLineColor1       = sf::Color(120, 200, 220, 255);
		seperatorLineColor2       = sf::Color(200, 215, 215, 255);
	} else {
		mode = DARK;
		backgroundSprite.setTexture(darkBackgroundTexture);

		blankColor                = sf::Color(  0,   0,   0,   0);
	}
}