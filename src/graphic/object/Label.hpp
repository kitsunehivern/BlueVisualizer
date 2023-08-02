#ifndef LABEL_HPP
#define LABEL_HPP

#include "../../Global.hpp"
#include "../Animation.hpp"
#include "Node.hpp"

namespace LabelData {
    const float characterSize = 25.f;
    const float postionOffset = 50.f;
}

class Label {
public:
    Label();
    Label(Node* node, std::string name);

    void draw(sf::RenderWindow* window, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawFadeIn(sf::RenderWindow* window, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawFadeOut(sf::RenderWindow* window, sf::Font* font, sf::Color* color, float ratioTime, bool fakeDraw);
    void drawChangeName(sf::RenderWindow* window, sf::Font* font, sf::Color* color, std::string oldName, std::string newName, float ratioTime, bool fakeDraw);

private:
    Node* mNode;
    std::string mName;
};

#endif // LABEL_HPP