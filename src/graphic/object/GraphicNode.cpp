#include "GraphicNode.hpp"

GraphicNode::GraphicNode() {
}

GraphicNode::GraphicNode(std::string value, sf::Vector2f position) {
    mValue = value;
    mPosition = position;
}

std::string GraphicNode::getValue() {
    return mValue;
}

void GraphicNode::setValue(std::string value) {
    mValue = value;
}

sf::Vector2f GraphicNode::getPosition() {
    return mPosition;
}

void GraphicNode::setPosition(sf::Vector2f position) {
    mPosition = position;
}

void GraphicNode::draw(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    ratioTime = Animation::Bezier(ratioTime);
    
    sf::Sprite nodeSprite(*nodeTexture);
    nodeSprite.setColor(*nodeColor);
    nodeSprite.setPosition(mPosition);
    window->draw(nodeSprite);

    sf::Text valueText(mValue, *valueFont, GraphicNodeData::characterSize);
    valueText.setFillColor(*valueColor);
    valueText.setOrigin(sfhelper::getCenterPosition(valueText.getLocalBounds()));
    valueText.setPosition(sfhelper::getCenterPosition(nodeSprite.getGlobalBounds()));
    window->draw(valueText);
}

void GraphicNode::drawFadeIn(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }
    
    sf::Color newNodeColor = (*nodeColor) * sf::Color(255, 255, 255, std::round(Animation::Bezier(ratioTime) * 255));
	sf::Color newValueColor = (*valueColor) * sf::Color(255, 255, 255, std::round(Animation::Bezier(ratioTime) * 255));

    draw(window, nodeTexture, &newNodeColor, valueFont, &newValueColor, ratioTime, fakeDraw);
}

void GraphicNode::drawFadeOut(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }
    
    drawFadeIn(window, nodeTexture, nodeColor, valueFont, valueColor, 1.f - ratioTime, fakeDraw);
}

void GraphicNode::drawValueFadeIn(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }
    
    sf::Color newValueColor = (*valueColor) * sf::Color(255, 255, 255, std::round(Animation::Bezier(ratioTime) * 255));

    draw(window, nodeTexture, nodeColor, valueFont, &newValueColor, ratioTime, fakeDraw);
}

void GraphicNode::drawValueFadeOut(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }
    
    drawValueFadeIn(window, nodeTexture, nodeColor, valueFont, valueColor, 1.f - ratioTime, fakeDraw);
}

void GraphicNode::drawChangePosition(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, sf::Vector2f oldPosition, sf::Vector2f newPosition, float ratioTime, bool fakeDraw) {
	mPosition.x = oldPosition.x + Animation::Bezier(ratioTime) * (newPosition.x - oldPosition.x);
	mPosition.y = oldPosition.y + Animation::Bezier(ratioTime) * (newPosition.y - oldPosition.y);
    
    if (fakeDraw) {
        return;
    }
    
    draw(window, nodeTexture, nodeColor, valueFont, valueColor, ratioTime, fakeDraw);
}

void GraphicNode::drawChangeValue(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, std::string oldValue, std::string newValue, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }
    
    if (ratioTime <= 0.5f) {
        mValue = oldValue;
        sf::Color curValueColor = *valueColor * sf::Color(255, 255, 255, std::round(Animation::Bezier(1.f - ratioTime * 2.f) * 255));
        draw(window, nodeTexture, nodeColor, valueFont, &curValueColor, ratioTime, fakeDraw);
    } else {
        mValue = newValue;
        sf::Color curValueColor = *valueColor * sf::Color(255, 255, 255, std::round(Animation::Bezier((ratioTime - 0.5f) * 2.f) * 255));
        draw(window, nodeTexture, nodeColor, valueFont, &curValueColor, ratioTime, fakeDraw);
    }
}

void GraphicNode::drawChangeColor(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* oldNodeColor, sf::Color* newNodeColor, sf::Font* valueFont, sf::Color* oldValueColor, sf::Color* newValueColor, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }
    
	sf::Color curNodeColor = *oldNodeColor;
	curNodeColor.r += std::round(Animation::Bezier(ratioTime) * (newNodeColor->r - (int)oldNodeColor->r));
	curNodeColor.g += std::round(Animation::Bezier(ratioTime) * (newNodeColor->g - (int)oldNodeColor->g));
	curNodeColor.b += std::round(Animation::Bezier(ratioTime) * (newNodeColor->b - (int)oldNodeColor->b));
	curNodeColor.a += std::round(Animation::Bezier(ratioTime) * (newNodeColor->a - (int)oldNodeColor->a));

	sf::Color curValueColor = *oldValueColor;
	curValueColor.r += std::round(Animation::Bezier(ratioTime) * (newValueColor->r - (int)oldValueColor->r));
	curValueColor.g += std::round(Animation::Bezier(ratioTime) * (newValueColor->g - (int)oldValueColor->g));
	curValueColor.b += std::round(Animation::Bezier(ratioTime) * (newValueColor->b - (int)oldValueColor->b));
	curValueColor.a += std::round(Animation::Bezier(ratioTime) * (newValueColor->a - (int)oldValueColor->a));
    
    draw(window, nodeTexture, &curNodeColor, valueFont, &curValueColor, ratioTime, fakeDraw);
}