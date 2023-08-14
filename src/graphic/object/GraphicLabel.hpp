#ifndef GRAPHIC_LABEL_HPP
#define GRAPHIC_LABEL_HPP

#include "../../Global.hpp"
#include "../Animation.hpp"
#include "GraphicNode.hpp"

namespace GraphicLabelData {
    const float characterSize = 25.f;
    const float postionOffset = 50.f;
}

class GraphicLabel {
public:
    GraphicLabel();

    void draw(sf::RenderWindow* window, GraphicNode* node, std::string name, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawFadeIn(sf::RenderWindow* window, GraphicNode* node, std::string name, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawFadeOut(sf::RenderWindow* window, GraphicNode* node, std::string name, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawChangeName(sf::RenderWindow* window, GraphicNode* node, std::string oldName, std::string newName, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawFixed(sf::RenderWindow* window, GraphicNode node, std::string name, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw);

private:
};

#endif // GRAPHIC_LABEL_HPP