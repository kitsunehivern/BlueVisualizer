#ifndef TREE_234_VISUALIZER_HPP
#define TREE_234_VISUALIZER_HPP

#include "Visualizer.hpp"

namespace Tree234VisualizerData {
    const int minSize = 1;
    const int maxSize = 30;
    const int defaultSize = 10;
    const int minValue = 1;
    const int maxValue = 99;

    const float treePositionY = 100.f;
    const sf::Vector2f space = sf::Vector2f(50.f, 80.f);
}

class Tree234Visualizer : public Visualizer {
public:
    Tree234Visualizer();
    Tree234Visualizer(sf::RenderWindow* window, AssetsHolder* assets);

    void run();
private:
    struct Node {

        std::vector<GraphicNode*> values;
        std::vector<Node*> children;

        Node();
    };

    Node* mRoot;

    void setPositions();

    void create(int size);
};

#endif // TREE_234_VISUALIZER_HPP