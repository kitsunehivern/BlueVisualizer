#ifndef ILLUSTRATION_BOX
#define ILLUSTRATION_BOX

#include "Global.h"
#include "Assets.h"

class IllustrationBox {
public:
	IllustrationBox();
	IllustrationBox(sf::Sprite* sprite, std::string name, sf::Vector2f position);

	void draw(sf::RenderWindow* window, Assets* assets);
	bool handleEvent(sf::RenderWindow* window, sf::Event* event);

private:
	sf::Sprite* sprite;
	std::string name;
	sf::Vector2f position;
};

#endif