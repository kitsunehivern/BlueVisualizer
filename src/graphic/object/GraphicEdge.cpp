#include "GraphicEdge.hpp"

GraphicEdge::GraphicEdge() {
}

void GraphicEdge::draw(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    ratioTime = Animation::Bezier(ratioTime);
    
    sf::Vector2f node1Center = node1->getPosition() + GraphicNodeData::nodeSize / 2.f;
    sf::Vector2f node2Center = node2->getPosition() + GraphicNodeData::nodeSize / 2.f;
    float length = sfhelper::getDistance(node1Center, node2Center) - GraphicNodeData::nodeSize.x;
    float angle = atan2(node2Center.y - node1Center.y, node2Center.x - node1Center.x);

    if (length <= 0.f) {
        return;
    }

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(node1Center + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f));
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
}

void GraphicEdge::drawFadeIn(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    
    sf::Vector2f node1Center = node1->getPosition() + GraphicNodeData::nodeSize / 2.f;
    sf::Vector2f node2Center = node2->getPosition() + GraphicNodeData::nodeSize / 2.f;
    float length = sfhelper::getDistance(node1Center, node2Center) - GraphicNodeData::nodeSize.x;
    float angle = atan2(node2Center.y - node1Center.y, node2Center.x - node1Center.x);

    if (length <= 0.f) {
        return;
    }

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(node1Center + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f));
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color * sf::Color(255, 255, 255, std::round(Animation::Bezier(ratioTime) * 255)));

    window->draw(sprite);
}

void GraphicEdge::drawSlideIn(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }
    
    sf::Vector2f node1Center = node1->getPosition() + GraphicNodeData::nodeSize / 2.f;
    sf::Vector2f node2Center = node2->getPosition() + GraphicNodeData::nodeSize / 2.f;
    float length = (sfhelper::getDistance(node1Center, node2Center) - GraphicNodeData::nodeSize.x) * Animation::Bezier(ratioTime);
    float angle = atan2(node2Center.y - node1Center.y, node2Center.x - node1Center.x);

    if (length <= 0.f) {
        return;
    }

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(node1Center + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f));
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
}

void GraphicEdge::drawSlideOut(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }
    
    sf::Vector2f node1Center = node1->getPosition() + GraphicNodeData::nodeSize / 2.f;
    sf::Vector2f node2Center = node2->getPosition() + GraphicNodeData::nodeSize / 2.f;
    float length = (sfhelper::getDistance(node1Center, node2Center) - GraphicNodeData::nodeSize.x) * (1.f - Animation::Bezier(ratioTime));
    float angle = atan2(node2Center.y - node1Center.y, node2Center.x - node1Center.x);

    if (length <= 0.f) {
        return;
    }

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(node1Center + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f));
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
}

void GraphicEdge::drawChangeColor(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* oldColor, sf::Color* newColor, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }
    
    sf::Vector2f node1Center = node1->getPosition() + GraphicNodeData::nodeSize / 2.f;
    sf::Vector2f node2Center = node2->getPosition() + GraphicNodeData::nodeSize / 2.f;
    float length = sfhelper::getDistance(node1Center, node2Center) - GraphicNodeData::nodeSize.x;
    float angle = atan2(node2Center.y - node1Center.y, node2Center.x - node1Center.x);

    if (length <= 0.f) {
        return;
    }

	sf::Color curColor = (*oldColor);
	curColor.r += std::round(Animation::Bezier(ratioTime) * (newColor->r - (int)oldColor->r));
	curColor.g += std::round(Animation::Bezier(ratioTime) * (newColor->g - (int)oldColor->g));
	curColor.b += std::round(Animation::Bezier(ratioTime) * (newColor->b - (int)oldColor->b));
	curColor.a += std::round(Animation::Bezier(ratioTime) * (newColor->a - (int)oldColor->a));

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(node1Center + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f));
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(curColor);

    window->draw(sprite);
}

void GraphicEdge::drawChangeNode(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* oldNode2, GraphicNode* newNode2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }
    
    sf::Vector2f node1Center = node1->getPosition() + GraphicNodeData::nodeSize / 2.f;
    sf::Vector2f node2Center = oldNode2->getPosition() + (newNode2->getPosition() - oldNode2->getPosition()) * Animation::Bezier(ratioTime) + GraphicNodeData::nodeSize / 2.f;
    float length = sfhelper::getDistance(node1Center, node2Center) - GraphicNodeData::nodeSize.x;
    float angle = atan2(node2Center.y - node1Center.y, node2Center.x - node1Center.x);

    if (length <= 0.f) {
        return;
    }

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(node1Center + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f));
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
}

void GraphicEdge::drawSlideOutChangeNode(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* oldNode2, GraphicNode* newNode2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }
    
    sf::Vector2f node1Center = node1->getPosition() + GraphicNodeData::nodeSize / 2.f;
    sf::Vector2f node2Center = oldNode2->getPosition() + (newNode2->getPosition() - oldNode2->getPosition()) * Animation::Bezier(ratioTime) + GraphicNodeData::nodeSize / 2.f;
    float length = (sfhelper::getDistance(node1Center, node2Center) - GraphicNodeData::nodeSize.x) * (1.f - Animation::Bezier(ratioTime));
    float angle = atan2(node2Center.y - node1Center.y, node2Center.x - node1Center.x);

    if (length <= 0.f) {
        return;
    }

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(node1Center + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f));
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
}

void GraphicEdge::drawFixed(sf::RenderWindow* window, GraphicNode node1, GraphicNode node2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    ratioTime = Animation::Bezier(ratioTime);
    
    sf::Vector2f node1Center = node1.getPosition() + GraphicNodeData::nodeSize / 2.f;
    sf::Vector2f node2Center = node2.getPosition() + GraphicNodeData::nodeSize / 2.f;
    float length = sfhelper::getDistance(node1Center, node2Center) - GraphicNodeData::nodeSize.x;
    float angle = atan2(node2Center.y - node1Center.y, node2Center.x - node1Center.x);

    if (length <= 0.f) {
        return;
    }

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(node1Center + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f));
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
}

void GraphicEdge::drawWeight(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* color, sf::Font* font, std::string weight, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    ratioTime = Animation::Bezier(ratioTime);
    
    sf::Vector2f node1Center = node1->getPosition() + GraphicNodeData::nodeSize / 2.f;
    sf::Vector2f node2Center = node2->getPosition() + GraphicNodeData::nodeSize / 2.f;
    float length = sfhelper::getDistance(node1Center, node2Center) - GraphicNodeData::nodeSize.x;
    float angle = atan2(node2Center.y - node1Center.y, node2Center.x - node1Center.x);

    if (length <= 0.f) {
        return;
    }

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(node1Center + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f));
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
    
    sf::Text text(weight, *font, GraphicNodeData::characterSize);
    text.setFillColor(*color);
    text.setOrigin(sfhelper::getCenterPosition(text.getLocalBounds()));
    text.setPosition(
        node1Center
        + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f)
        + sf::Vector2f(cos(angle) * length / 2.f, sin(angle) * length / 2.f)
        + sf::Vector2f(cos(angle + GraphicEdgeData::PI / 2.f) * GraphicEdgeData::weightDistance, sin(angle + GraphicEdgeData::PI / 2.f) * GraphicEdgeData::weightDistance)
    );


    window->draw(text);
}