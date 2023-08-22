#ifndef GRAPHIC_FREE_EDGE_HPP
#define GRAPHIC_FREE_EDGE_HPP

#include "../../Global.hpp"
#include "../Animation.hpp"

namespace GraphicFreeEdgeData {
    const float PI = std::acos(-1);

    const float thickness = 5.f;
    const float length = 50.f;
};

struct GraphicFreeEdge {
public:
    GraphicFreeEdge();

    void draw(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawFadeIn(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawSlideIn(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawSlideOut(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawChangeColor(sf::RenderWindow* window, sf::Vector2f position1, sf::Vector2f position2, sf::Texture* texture, sf::Color* oldColor, sf::Color* newColor, float ratioTime, bool fakeDraw);
};

#endif // GRAPHIC_FREE_EDGE_HPP