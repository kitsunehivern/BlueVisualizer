#ifndef GRAPHIC_EDGE_HPP
#define GRAPHIC_EDGE_HPP

#include "../../Global.hpp"
#include "../Animation.hpp"
#include "GraphicNode.hpp"

namespace GraphicEdgeData {
    const float PI = std::acos(-1);

    const float thickness = 5.f;
    const float length = 50.f;

    const float weightDistance = 20.f;
};

struct GraphicEdge {
public:
    GraphicEdge();

    void draw(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawFadeIn(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawSlideIn(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawSlideOut(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawChangeColor(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* oldColor, sf::Color* newColor, float ratioTime, bool fakeDraw);
    void drawChangeNode(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* oldNode2, GraphicNode* newNode2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawSlideOutChangeNode(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* oldNode2, GraphicNode* newNode2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawFixed(sf::RenderWindow* window, GraphicNode node1, GraphicNode node2, sf::Texture* texture, sf::Color* color, float ratioTime, bool fakeDraw);

    void drawWeight(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* edgeColor, sf::Font* font, sf::Color* weightColor, std::string weight, float ratioTime, bool fakeDraw);
    void drawWeightFadeOut(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* edgeColor, sf::Font* font, sf::Color* weightColor, std::string weight, float ratioTime, bool fakeDraw);
    void drawWeightSlideIn(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* edgeColor, sf::Font* font, sf::Color* weightColor, std::string weight, float ratioTime, bool fakeDraw);
    void drawWeightChangeColor(sf::RenderWindow* window, GraphicNode* node1, GraphicNode* node2, sf::Texture* texture, sf::Color* oldEdgeColor, sf::Color* newEdgeColor, sf::Font* font, sf::Color* weightColor, std::string weight, float ratioTime, bool fakeDraw);

private:
};

#endif // GRAPHIC_EDGE_HPP