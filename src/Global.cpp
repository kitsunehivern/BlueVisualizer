#include "Global.hpp"

bool sfhelper::isMouseOver(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
    return mousePosition.x >= position.x && mousePosition.x <= position.x + size.x && mousePosition.y >= position.y && mousePosition.y <= position.y + size.y;
}

bool sfhelper::isMouseOver(sf::RenderWindow* window, sf::FloatRect rect) {
    return isMouseOver(window, sf::Vector2f(rect.left, rect.top), sf::Vector2f(rect.width, rect.height));
}

sf::Vector2f sfhelper::getCenterPosition(sf::Vector2f position, sf::Vector2f size) {
    return sf::Vector2f(position.x + size.x / 2.f, position.y + size.y / 2.f);
}

sf::Vector2f sfhelper::getCenterPosition(sf::FloatRect rect) {
    return getCenterPosition(sf::Vector2f(rect.left, rect.top), sf::Vector2f(rect.width, rect.height));
}