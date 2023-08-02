#include "Label.hpp"

Label::Label() {
}

Label::Label(Node* node, std::string name) {
    mNode = node;
    mName = name;
}

void Label::draw(sf::RenderWindow* window, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    ratioTime = Animation::Bezier(ratioTime);

    sf::Text labelText(mName, *font, LabelData::characterSize);
    labelText.setFillColor(*color);
    labelText.setOrigin(sfhelper::getCenterPosition(labelText.getLocalBounds()).x, 0.f);
    labelText.setPosition(sfhelper::getCenterPosition(mNode->getPosition(), NodeData::nodeSize).x, mNode->getPosition().y + LabelData::postionOffset);
    window->draw(labelText);
}

void Label::drawFadeIn(sf::RenderWindow* window, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    sf::Color newColor = (*color) * sf::Color(255, 255, 255, std::round(Animation::Bezier(ratioTime) * 255));

    draw(window, font, &newColor, ratioTime, fakeDraw);
}

void Label::drawFadeOut(sf::RenderWindow* window, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    drawFadeIn(window, font, color, 1.f - ratioTime, fakeDraw);
}

void Label::drawChangeName(sf::RenderWindow* window, sf::Font* font, sf::Color* color, std::string oldName, std::string newName, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    if (ratioTime <= 0.5f) {
        mName = oldName;
        drawFadeOut(window, font, color, ratioTime * 2.f, fakeDraw);
    } else {
        mName = newName;
        drawFadeIn(window, font, color, (ratioTime - 0.5f) * 2.f, fakeDraw);
    }
}