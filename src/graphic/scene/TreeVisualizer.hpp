#ifndef TREE_VISUALIZER_HPP
#define TREE_VISUALIZER_HPP

#include "Visualizer.hpp"
#include "../../core/Tree.hpp"

namespace TreeVisualizerData {
    const float treePositionY = 100.f;
    const sf::Vector2f space = sf::Vector2f(50.f, 80.f);
}

class TreeVisualizer : protected Visualizer, protected Tree<GraphicNode> {
protected:
    TreeVisualizer();
    TreeVisualizer(sf::RenderWindow* window, AssetsHolder* assets);

    void setPositions();
    void drawReformat();

    void drawAllNode(Node* node);
    void drawAllNodeFadeIn(Node* node);
    void drawAllEdge(Node* node);
    void drawAllEdgeFadeIn(Node* node);
};

#endif // TREE_VISUALIZER_HPP