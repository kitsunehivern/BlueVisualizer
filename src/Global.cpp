#include "Global.hpp"

bool sfhelper::isMouseOver(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
    return mousePosition.x >= position.x && mousePosition.x <= position.x + size.x && mousePosition.y >= position.y && mousePosition.y <= position.y + size.y;
}