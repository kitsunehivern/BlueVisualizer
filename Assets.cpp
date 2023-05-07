#include "Assets.h"

Assets::Assets() {
	loadAll();
	fix();
	
	std::ifstream fin("Data/theme.dat");
	if (!fin.good()) {
		mode = LIGHT;
	} else {
		bool value;
		fin >> value;

		mode = !value ? LIGHT : DARK;
	}
	fin.close();

	switchMode();
	switchMode();
}

Assets::~Assets() {
	std::ofstream fout("Data/theme.dat");
	fout << (mode == LIGHT ? 0 : 1);
	fout.close();
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
	loadOne(&arialFont, &arialText, "arial.ttf");
	loadOne(&consolasFont, &consolasText, "consolas.ttf");
	loadOne(&consolasBoldFont, &consolasBoldText, "consolas_bold.ttf");
	loadOne(&hyperspaceItalicFont, &hyperspaceItalicText, "hyperspace_race_expanded_heavy_italic.ttf");

	loadOne(&lightBackgroundTexture, &backgroundSprite, "Main/LightBackground.png");
	loadOne(&darkBackgroundTexture, &backgroundSprite, "Main/DarkBackground.png");
	loadOne(&lightLogoTexture, &logoSprite, "Main/LightLogo.png");
	loadOne(&darkLogoTexture, &logoSprite, "Main/DarkLogo.png");
	loadOne(&hollowCircleTexture, &hollowCircleSprite, "Main/HollowCircle.png");
	loadOne(&solidCircleTexture, &solidCircleSprite, "Main/SolidCircle.png");
	loadOne(&hollowSquareTexture, &hollowSquareSprite, "Main/HollowSquare.png");
	loadOne(&solidSquareTexture, &solidSquareSprite, "Main/SolidSquare.png");
	loadOne(&stickTexture, &stickSprite, "Main/Stick.png");
	loadOne(&arrowTexture, &arrowSprite, "Main/Arrow.png");
	loadOne(&box60x60Texture, &box60x60Sprite, "Main/Box60x60.png");
	loadOne(&box320x240Texture, &box320x240Sprite, "Main/Box320x240.png");
	loadOne(&box680x40Texture, &box680x40Sprite, "Main/Box680x40.png");
	loadOne(&box680x300Texture, &box680x300Sprite, "Main/Box680x300.png");
	loadOne(&box680x140Texture, &box680x140Sprite, "Main/Box680x140.png");
	loadOne(&box880x60Texture, &box880x60Sprite, "Main/Box880x60.png");
	loadOne(&box880x380Texture, &box880x380Sprite, "Main/Box880x380.png");
	loadOne(&box1580x60Texure, &box1580x60Sprite, "Main/Box1580x60.png");
	loadOne(&box1580x320Texure, &box1580x320Sprite, "Main/Box1580x320.png");
	loadOne(&skewBox100x40Texture, &skewBox100x40Sprite, "Main/SkewBox100x40.png");
	loadOne(&skewBox160x60Texture, &skewBox160x60Sprite, "Main/SkewBox160x60.png");
	loadOne(&skewBox400x60Texture, &skewBox400x60Sprite, "Main/SkewBox400x60.png");
	loadOne(&skewBox560x60Texture, &skewBox560x60Sprite, "Main/SkewBox560x60.png");
	loadOne(&backButtonTexture, &backButtonSprite, "Main/BackButton.png");
	loadOne(&quitButtonTexture, &quitButtonSprite, "Main/QuitButton.png");
	loadOne(&modeButtonTexture, &modeButtonSprite, "Main/ModeButton.png");
	loadOne(&prevButtonTexture, &prevButtonSprite, "Main/PrevNextButton.png");
	loadOne(&nextButtonTexture, &nextButtonSprite, "Main/PrevNextButton.png");
	loadOne(&controlButtonsTexture, &controlButtonsSprite, "Main/ControlButtons.png");
	loadOne(&statusButtonsTexture, &statusButtonsSprite, "Main/StatusButtons.png");
	loadOne(&progressBarTexture, &progressBarSprite, "Main/ProgressBar.png");
	loadOne(&speedTexture, &speedSprite, "Main/Speed.png");

	loadOne(&lightSLLTexture, &SLLSprite, "SLL/Light.png");
	loadOne(&darkSLLTexture, &SLLSprite, "SLL/Dark.png");
	loadOne(&lightDLLTexture, &DLLSprite, "DLL/Light.png");
	loadOne(&darkDLLTexture, &DLLSprite, "DLL/Dark.png");
	loadOne(&lightCLLTexture, &CLLSprite, "CLL/Light.png");
	loadOne(&darkCLLTexture, &CLLSprite, "CLL/Dark.png");
	loadOne(&lightStackTexture, &StackSprite, "Stack/Light.png");
	loadOne(&darkStackTexture, &StackSprite, "Stack/Dark.png");
	loadOne(&lightQueueTexture, &QueueSprite, "Queue/Light.png");
	loadOne(&darkQueueTexture, &QueueSprite, "Queue/Dark.png");
	loadOne(&lightStaticArrayTexture, &StaticArraySprite, "StaticArray/Light.png");
	loadOne(&darkStaticArrayTexture, &StaticArraySprite, "StaticArray/Dark.png");
	loadOne(&lightDynamicArrayTexture, &DynamicArraySprite, "DynamicArray/Light.png");
	loadOne(&darkDynamicArrayTexture, &DynamicArraySprite, "DynamicArray/Dark.png");
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
		logoSprite.setTexture(lightLogoTexture);

		SLLSprite.setTexture(lightSLLTexture);
		DLLSprite.setTexture(lightDLLTexture);
		CLLSprite.setTexture(lightCLLTexture);
		StackSprite.setTexture(lightStackTexture);
		QueueSprite.setTexture(lightQueueTexture);
		StaticArraySprite.setTexture(lightStaticArrayTexture);
		DynamicArraySprite.setTexture(lightDynamicArrayTexture);

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
		normalCellTextColor       = sf::Color(  0,   0,   0, 255);
		blurCellTextColor         = sf::Color(150, 150, 150, 255);
		highlightCellTextColor1   = sf::Color(255, 255, 255, 255);
		highlightCellTextColor2   = sf::Color(255, 140,  40, 255);
		insertedCellSquareColor   = sf::Color( 90, 200,  90, 255);
		insertedCellTextColor     = sf::Color( 90, 200,  90, 255);
		erasedCellSquareColor     = sf::Color(215,  80,  60, 255);
		erasedCellTextColor       = sf::Color(215,  80,  60, 255);

		labelColor                = sf::Color(255,  40,  40, 255);

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
		logoSprite.setTexture(darkLogoTexture);

		SLLSprite.setTexture(darkSLLTexture);
		DLLSprite.setTexture(darkDLLTexture);
		CLLSprite.setTexture(darkCLLTexture);
		StackSprite.setTexture(darkStackTexture);
		QueueSprite.setTexture(darkQueueTexture);
		StaticArraySprite.setTexture(darkStaticArrayTexture);
		DynamicArraySprite.setTexture(darkDynamicArrayTexture);

		blankColor                = sf::Color(  0,   0,   0,   0);

		normalNodeCircleColor     = sf::Color(255, 255, 255, 255);
		highlightNodeCircleColor1 = sf::Color(255, 145,  55, 255);
		highlightNodeCircleColor2 = sf::Color( 45, 185, 210, 255);
		normalNodeTextColor       = sf::Color(255, 255, 255, 255);
		highlightNodeTextColor1   = sf::Color(255, 255, 255, 255);
		highlightNodeTextColor2   = sf::Color(255, 145,  55, 255);
		insertedNodeCircleColor   = sf::Color( 90, 200,  90, 255);
		erasedNodeCircleColor     = sf::Color(215,  80,  60, 255);

		normalCellSquareColor     = sf::Color(255, 255, 255, 255);
		blurCellSquareColor       = sf::Color(100, 100, 100, 255);
		highlightCellSquareColor1 = sf::Color(255, 145,  55, 255);
		normalCellTextColor       = sf::Color(255, 255, 255, 255);
		blurCellTextColor         = sf::Color(100, 100, 100, 255);
		highlightCellTextColor1   = sf::Color(255, 255, 255, 255);
		highlightCellTextColor2   = sf::Color(255, 145,  55, 255);
		insertedCellSquareColor   = sf::Color( 90, 200,  90, 255);
		insertedCellTextColor     = sf::Color( 90, 200,  90, 255);
		erasedCellSquareColor     = sf::Color(215,  80,  60, 255);
		erasedCellTextColor       = sf::Color(215,  80,  60, 255);

		labelColor                = sf::Color(255, 255, 255, 255);

		normalEdgeColor           = sf::Color(255, 255, 255, 255);
		highlightEdgeColor        = sf::Color(255, 145,  55, 255);
		insertedEdgeColor         = sf::Color( 90, 200,  90, 255);
		erasedEdgeColor           = sf::Color(215,  80,  60, 255);

		boxColor1                 = sf::Color( 60, 125, 195, 150);
		boxColor2                 = sf::Color( 40,  70, 115, 150);
		boxColor3                 = sf::Color( 45, 125, 185, 255);
		boxColor4                 = sf::Color(105, 210, 255, 255);
		boxColor5                 = sf::Color( 45, 225, 120, 255);
		boxColor6                 = sf::Color(230,  65,  80, 255);
		boxTextColor1             = sf::Color(255, 255, 255, 255);
		boxTextColor2             = sf::Color(  0,   0,   0, 255);
		seperatorLineColor1       = sf::Color(110, 195, 250, 255);
		seperatorLineColor2       = sf::Color( 60, 100, 145, 255);
	}
}