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

void GraphicEdge::drawWeight(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* edgeColor, sf::Font* font, sf::Color* weightColor, std::string weight, float ratioTime, bool fakeDraw) {
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
    sprite.setColor(*edgeColor);

    window->draw(sprite);
    
    sf::Text text(weight, *font, GraphicNodeData::characterSize);
    text.setFillColor(*weightColor);
    text.setOrigin(sfhelper::getCenterPosition(text.getLocalBounds()));
    text.setPosition(
        node1Center
        + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f)
        + sf::Vector2f(cos(angle) * length / 2.f, sin(angle) * length / 2.f)
        + sf::Vector2f(cos(angle + GraphicEdgeData::PI / 2.f) * GraphicEdgeData::weightDistance, sin(angle + GraphicEdgeData::PI / 2.f) * GraphicEdgeData::weightDistance)
    );


    window->draw(text);
}

void GraphicEdge::drawWeightFadeOut(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* edgeColor, sf::Font* font, sf::Color* weightColor, std::string weight, float ratioTime, bool fakeDraw) {
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
    sprite.setColor(*edgeColor * sf::Color(255, 255, 255, std::round(Animation::Bezier(1.f - ratioTime) * 255)));

    window->draw(sprite);
    
    sf::Text text(weight, *font, GraphicNodeData::characterSize);
    text.setFillColor(*weightColor * sf::Color(255, 255, 255, std::round(Animation::Bezier(1.f - ratioTime) * 255)));
    text.setOrigin(sfhelper::getCenterPosition(text.getLocalBounds()));
    text.setPosition(
        node1Center
        + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f)
        + sf::Vector2f(cos(angle) * length / 2.f, sin(angle) * length / 2.f)
        + sf::Vector2f(cos(angle + GraphicEdgeData::PI / 2.f) * GraphicEdgeData::weightDistance, sin(angle + GraphicEdgeData::PI / 2.f) * GraphicEdgeData::weightDistance)
    );


    window->draw(text);    
}

void GraphicEdge::drawWeightSlideIn(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* edgeColor, sf::Font* font, sf::Color* weightColor, std::string weight, float ratioTime, bool fakeDraw) {
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
    sprite.setColor(*edgeColor);

    window->draw(sprite);
    
    sf::Text text(weight, *font, GraphicNodeData::characterSize);
    text.setFillColor(*weightColor);
    text.setOrigin(sfhelper::getCenterPosition(text.getLocalBounds()));
    text.setPosition(
        node1Center
        + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f)
        + sf::Vector2f(cos(angle) * length / 2.f, sin(angle) * length / 2.f)
        + sf::Vector2f(cos(angle + GraphicEdgeData::PI / 2.f) * GraphicEdgeData::weightDistance, sin(angle + GraphicEdgeData::PI / 2.f) * GraphicEdgeData::weightDistance)
    );
}

void GraphicEdge::drawWeightChangeColor(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* oldEdgeColor, sf::Color* newEdgeColor, sf::Font* font, sf::Color* weightColor, std::string weight, float ratioTime, bool fakeDraw) {
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

	sf::Color curColor = (*oldEdgeColor);
	curColor.r += std::round(Animation::Bezier(ratioTime) * (newEdgeColor->r - (int)oldEdgeColor->r));
	curColor.g += std::round(Animation::Bezier(ratioTime) * (newEdgeColor->g - (int)oldEdgeColor->g));
	curColor.b += std::round(Animation::Bezier(ratioTime) * (newEdgeColor->b - (int)oldEdgeColor->b));
	curColor.a += std::round(Animation::Bezier(ratioTime) * (newEdgeColor->a - (int)oldEdgeColor->a));

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(node1Center + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f));
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(curColor);

    window->draw(sprite);

    sf::Text text(weight, *font, GraphicNodeData::characterSize);
    text.setFillColor(*weightColor);
    text.setOrigin(sfhelper::getCenterPosition(text.getLocalBounds()));
    text.setPosition(
        node1Center
        + sf::Vector2f(cos(angle) * GraphicNodeData::nodeSize.x / 2.f, sin(angle) * GraphicNodeData::nodeSize.x / 2.f)
        + sf::Vector2f(cos(angle) * length / 2.f, sin(angle) * length / 2.f)
        + sf::Vector2f(cos(angle + GraphicEdgeData::PI / 2.f) * GraphicEdgeData::weightDistance, sin(angle + GraphicEdgeData::PI / 2.f) * GraphicEdgeData::weightDistance)
    );

    window->draw(text);
}

void GraphicEdge::drawFree(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    ratioTime = Animation::Bezier(ratioTime);

    float length = sfhelper::getDistance(position1, position2);
    float angle = atan2(position2.y - position1.y, position2.x - position1.x);

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(position1);
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
}

void GraphicEdge::drawFreeFadeIn(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    float length = sfhelper::getDistance(position1, position2);
    float angle = atan2(position2.y - position1.y, position2.x - position1.x);

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(position1);
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color * sf::Color(255, 255, 255, std::round(Animation::Bezier(ratioTime) * 255)));

    window->draw(sprite);
}

void GraphicEdge::drawFreeSlideIn(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    ratioTime = Animation::Bezier(ratioTime);

    float length = sfhelper::getDistance(position1, position2) * Animation::Bezier(ratioTime);
    float angle = atan2(position2.y - position1.y, position2.x - position1.x);

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(position1);
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
}

void GraphicEdge::drawFreeSlideOut(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    drawFreeSlideIn(window, position1, position2, texture, color, 1.f - ratioTime, fakeDraw);
}

void GraphicEdge::drawFreeChangeColor(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* oldColor, sf::Color* newColor, float ratioTime, bool fakeDraw) {
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
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(position1);
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(curColor);

    window->draw(sprite);
}

void GraphicEdge::drawFreeChangePosition(sf::RenderWindow* window, sf::Vector2f oldPosition1, sf::Vector2f newPosition1, sf::Vector2f oldPosition2, sf::Vector2f newPosition2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    sf::Vector2f curPosition1 = oldPosition1 + (newPosition1 - oldPosition1) * Animation::Bezier(ratioTime);
    sf::Vector2f curPosition2 = oldPosition2 + (newPosition2 - oldPosition2) * Animation::Bezier(ratioTime);

    float length = sfhelper::getDistance(curPosition1, curPosition2);
    float angle = atan2(curPosition2.y - curPosition1.y, curPosition2.x - curPosition1.x);

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(oldPosition1 + (newPosition1 - oldPosition1) * ratioTime);
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
}

void GraphicEdge::drawFreeChangePositionSlideIn(sf::RenderWindow* window, sf::Vector2f oldPosition1, sf::Vector2f newPosition1, sf::Vector2f oldPosition2, sf::Vector2f newPosition2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    sf::Vector2f curPosition1 = oldPosition1 + (newPosition1 - oldPosition1) * Animation::Bezier(ratioTime);
    sf::Vector2f curPosition2 = oldPosition2 + (newPosition2 - oldPosition2) * Animation::Bezier(ratioTime);

    float length = sfhelper::getDistance(curPosition1, curPosition2) * Animation::Bezier(ratioTime);
    float angle = atan2(curPosition2.y - curPosition1.y, curPosition2.x - curPosition1.x);

    sf::Sprite sprite(*texture);
    sprite.setScale(length / GraphicEdgeData::length, 1.f);
    sprite.setOrigin(0.f, GraphicEdgeData::thickness / 2.f);
    sprite.setPosition(oldPosition1 + (newPosition1 - oldPosition1) * ratioTime);
    sprite.setRotation(angle * 180.f / GraphicEdgeData::PI);
    sprite.setColor(*color);

    window->draw(sprite);
}

void GraphicEdge::drawFreeChangePositionSlideOut(sf::RenderWindow* window, sf::Vector2f oldPosition1, sf::Vector2f newPosition1, sf::Vector2f oldPosition2, sf::Vector2f newPosition2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    drawFreeChangePositionSlideIn(window, oldPosition1, newPosition1, oldPosition2, newPosition2, texture, color, 1.f - ratioTime, fakeDraw);
}