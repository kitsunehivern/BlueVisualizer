#ifndef NODE_HPP
#define NODE_HPP

#include "../../Global.hpp"
#include "../AssetsHolder.hpp"

namespace NodeData {
    const float nodeSize = 50.f;
    const float textSize = 25.f;

    enum Type {
        hollow,
        solid
    };

    enum State {
        none,
        fadeIn,
        fadeOut,
        move,
        changeColor,
        changeValue
    };
}

class Node {
public:
    Node();
    Node(sf::RenderWindow* window, AssetsHolder* assetsHolder);

    int getValue();
    void setValue(int value);

    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f position);

    void draw(NodeData::Type type, NodeData::State state, std::vector<std::any> args, float ratioTime, bool fakeDraw = false);

private:
    sf::RenderWindow* mWindow;
    AssetsHolder* mAssetsHolder;
    int mValue;
    sf::Vector2f mPosition;
};

#endif // NODE_HPP