#ifndef NODE_HPP
#define NODE_HPP

#include "../../Global.hpp"
#include "../Animation.hpp"

namespace NodeData {
    const sf::Vector2f nodeSize = sf::Vector2f(50.f, 50.f);
    const float characterSize = 25.f;

    enum Shape {
        circle,
        square,
    };

    enum Type {
        hollow,
        filled,
        marked,
    };
}

class Node {
public:
    Node();
    Node(std::string value, sf::Vector2f position);

    std::string getValue();
    void setValue(std::string value);

    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f position);

    void draw(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, float ratioTime, bool fakeDraw);
    void drawFadeIn(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, float ratioTime, bool fakeDraw);
    void drawFadeOut(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, float ratioTime, bool fakeDraw);
    void drawValueFadeIn(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, float ratioTime, bool fakeDraw);
    void drawValueFadeOut(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, float ratioTime, bool fakeDraw);    
    void drawChangePosition(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, sf::Vector2f oldPosition, sf::Vector2f newPosition, float ratioTime, bool fakeDraw);
    void drawChangeValue(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* nodeColor, sf::Font* valueFont, sf::Color* valueColor, std::string oldValue, std::string newValue, float ratioTime, bool fakeDraw);
    void drawChangeColor(sf::RenderWindow* window, sf::Texture* nodeTexture, sf::Color* oldNodeColor, sf::Color* newNodeColor, sf::Font* valueFont, sf::Color* oldValueColor, sf::Color* newValueColor, float ratioTime, bool fakeDraw);

private:
    std::string mValue;
    sf::Vector2f mPosition;
};

#endif // NODE_HPP