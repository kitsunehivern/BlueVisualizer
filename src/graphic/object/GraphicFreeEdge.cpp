#include "GraphicFreeEdge.hpp"

GraphicFreeEdge::GraphicFreeEdge() {
}

void GraphicFreeEdge::draw(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    ratioTime = Animation::Bezier(ratioTime);

    float length = sfhelper::getDistance(position1, position2);
    float angle = atan2(position2.y - position1.y, position2.x - position1.x);

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicFreeEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicFreeEdgeData::thickness / 2.f);
    sprite.setPosition(position1);
    sprite.setRotation(angle * 180.f / GraphicFreeEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
}

void GraphicFreeEdge::drawFadeIn(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    float length = sfhelper::getDistance(position1, position2);
    float angle = atan2(position2.y - position1.y, position2.x - position1.x);

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicFreeEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicFreeEdgeData::thickness / 2.f);
    sprite.setPosition(position1);
    sprite.setRotation(angle * 180.f / GraphicFreeEdgeData::PI);
    sprite.setColor(*color * sf::Color(255, 255, 255, std::round(Animation::Bezier(ratioTime) * 255)));

    window->draw(sprite);
}

void GraphicFreeEdge::drawSlideIn(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    ratioTime = Animation::Bezier(ratioTime);

    float length = sfhelper::getDistance(position1, position2);
    float angle = atan2(position2.y - position1.y, position2.x - position1.x);

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicFreeEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicFreeEdgeData::thickness / 2.f);
    sprite.setPosition(position1 + sf::Vector2f(cos(angle) * GraphicFreeEdgeData::length * (1.f - ratioTime), sin(angle) * GraphicFreeEdgeData::length * (1.f - ratioTime)));
    sprite.setRotation(angle * 180.f / GraphicFreeEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
}

void GraphicFreeEdge::drawSlideOut(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    drawSlideIn(window, position1, position2, texture, color, 1.f - ratioTime, fakeDraw);
}

void GraphicFreeEdge::drawChangeColor(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* oldColor, sf::Color* newColor, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    float length = sfhelper::getDistance(position1, position2);
    float angle = atan2(position2.y - position1.y, position2.x - position1.x);

	sf::Color curColor = (*oldColor);
	curColor.r += std::round(Animation::Bezier(ratioTime) * (newColor->r - (int)oldColor->r));
	curColor.g += std::round(Animation::Bezier(ratioTime) * (newColor->g - (int)oldColor->g));
	curColor.b += std::round(Animation::Bezier(ratioTime) * (newColor->b - (int)oldColor->b));
	curColor.a += std::round(Animation::Bezier(ratioTime) * (newColor->a - (int)oldColor->a));

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicFreeEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicFreeEdgeData::thickness / 2.f);
    sprite.setPosition(position1);
    sprite.setRotation(angle * 180.f / GraphicFreeEdgeData::PI);
    sprite.setColor(curColor);

    window->draw(sprite);
}