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

float sfhelper::getDistance(sf::Vector2f position1, sf::Vector2f position2) {
    return std::sqrt((position1.x - position2.x) * (position1.x - position2.x) + (position1.y - position2.y) * (position1.y - position2.y));
}

sf::IntRect sfhelper::toIntRect(sf::FloatRect rect) {
    return sf::IntRect(rect.left, rect.top, rect.width, rect.height);
}

sf::FloatRect sfhelper::toFloatRect(sf::IntRect rect) {
    return sf::FloatRect(rect.left, rect.top, rect.width, rect.height);
}

char sfhelper::keyCodeToChar(sf::Keyboard::Key keyCode) {
    if (keyCode >= sf::Keyboard::A && keyCode <= sf::Keyboard::Z) {
        return keyCode - sf::Keyboard::A + 'a';
    }

    if (keyCode >= sf::Keyboard::Num0 && keyCode <= sf::Keyboard::Num9) {
        return keyCode - sf::Keyboard::Num0 + '0';
    }

    if (keyCode >= sf::Keyboard::Numpad0 && keyCode <= sf::Keyboard::Numpad9) {
        return keyCode - sf::Keyboard::Numpad0 + '0';
    }

    if (keyCode == sf::Keyboard::Space) {
        return ' ';
    }

    if (keyCode == sf::Keyboard::Period) {
        return '.';
    }

    if (keyCode == sf::Keyboard::Comma) {
        return ',';
    }

    if (keyCode == sf::Keyboard::Dash) {
        return '-';
    }

    return '\0';
}