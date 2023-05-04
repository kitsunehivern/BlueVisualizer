#ifndef MENU_H
#define MENU_H

#include "Global.h"
#include "Assets.h"

struct Menu {
public:
	Menu(sf::RenderWindow* window, Assets* assets);

	int run();

private:
	sf::RenderWindow window;
	Assets* assets;

	void draw();
	int handleEvent();
};

#endif