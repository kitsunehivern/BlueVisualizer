#include "Assets.h"

Assets::Assets() {
	loadAll();
	fix();
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

	loadOne(&hollowCircleTexture, &hollowCircleSprite, "HollowCircle.png");
	loadOne(&solidCircleTexture, &solidCircleSprite, "SolidCircle.png");
	loadOne(&stickTexture, &stickSprite, "Stick.png");
	loadOne(&arrowTexture, &arrowSprite, "Arrow.png");
	loadOne(&box680x40Texture, &box680x40Sprite, "Box680x40.png");
	loadOne(&box680x300Texture, &box680x300Sprite, "Box680x300.png");
	loadOne(&box680x140Texture, &box680x140Sprite, "Box680x140.png");
	loadOne(&box880x60Texture, &box880x60Sprite, "Box880x60.png");
	loadOne(&box880x380Texture, &box880x380Sprite, "Box880x380.png");
	loadOne(&skewBox100x40Texture, &skewBox100x40Sprite, "SkewBox100x40.png");
	loadOne(&skewBox160x60Texture, &skewBox160x60Sprite, "SkewBox160x60.png");
	loadOne(&skewBox400x60Texture, &skewBox400x60Sprite, "SkewBox400x60.png");
	loadOne(&skewBox560x60Texture, &skewBox560x60Sprite, "SkewBox560x60.png");
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