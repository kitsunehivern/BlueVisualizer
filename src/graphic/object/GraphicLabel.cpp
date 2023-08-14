#include "GraphicLabel.hpp"

GraphicLabel::GraphicLabel() {
}

void GraphicLabel::draw(sf::RenderWindow* window, GraphicNode* node, std::string name, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    ratioTime = Animation::Bezier(ratioTime);

    sf::Text labelText(name, *font, GraphicLabelData::characterSize);
    labelText.setFillColor(*color);
    labelText.setOrigin(sfhelper::getCenterPosition(labelText.getLocalBounds()).x, 0.f);
    labelText.setPosition(sfhelper::getCenterPosition(node->getPosition(), GraphicNodeData::nodeSize).x, node->getPosition().y + GraphicLabelData::postionOffset);
    window->draw(labelText);
}

void GraphicLabel::drawFadeIn(sf::RenderWindow* window, GraphicNode* node, std::string name, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    sf::Color newColor = (*color) * sf::Color(255, 255, 255, std::round(Animation::Bezier(ratioTime) * 255));

    draw(window, node, name, font, &newColor, ratioTime, fakeDraw);
}

void GraphicLabel::drawFadeOut(sf::RenderWindow* window, GraphicNode* node, std::string name, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    drawFadeIn(window, node, name, font, color, 1.f - ratioTime, fakeDraw);
}

void GraphicLabel::drawChangeName(sf::RenderWindow* window, GraphicNode* node, std::string oldName, std::string newName, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    if (ratioTime <= 0.5f) {
        drawFadeOut(window, node, oldName, font, color, ratioTime * 2.f, fakeDraw);
    } else {
        drawFadeIn(window, node, newName, font, color, (ratioTime - 0.5f) * 2.f, fakeDraw);
    }
}

void GraphicLabel::drawFixed(sf::RenderWindow* window, GraphicNode node, std::string name, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    draw(window, &node, name, font, color, ratioTime, fakeDraw);
}