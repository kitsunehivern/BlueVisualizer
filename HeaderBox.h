#ifndef HEADER_BOX_H
#define HEADER_BOX_H

#include "Global.h"
#include "Assets.h"

class HeaderBox {
public:
	HeaderBox();
	HeaderBox(Assets* assets, std::string name, sf::Vector2f position);

	bool handleEvent(sf::RenderWindow* window, sf::Event* event);
	void draw(sf::RenderWindow* window);

private:
	Assets* assets;
	std::string name;
	sf::Vector2f position;
};

#endif