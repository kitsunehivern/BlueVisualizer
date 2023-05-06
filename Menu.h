#ifndef MENU_H
#define MENU_H

#include "Global.h"
#include "Assets.h"
#include "IllustrationBox.h"

struct Menu {
public:
	Menu(sf::RenderWindow* window, Assets* assets);

	int run();

private:
	sf::RenderWindow* window;
	Assets* assets;

	std::vector <IllustrationBox> illustrationBoxs;

	void draw();
	int handleEvent(sf::Event* event);
};

#endif