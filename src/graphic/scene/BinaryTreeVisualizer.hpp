#ifndef BINARY_TREE_VISUALIZER_HPP
#define BINARY_TREE_VISUALIZER_HPP

#include "Visualizer.hpp"
#include "../../core/BinaryTree.hpp"

namespace BinaryTreeVisualizerData {
    const float treePositionY = 100.f;
    const sf::Vector2f space = sf::Vector2f(50.f, 80.f);
}

class BinaryTreeVisualizer : protected Visualizer, protected BinaryTree<GraphicNode> {
protected:
    BinaryTreeVisualizer();
    BinaryTreeVisualizer(sf::RenderWindow* window, AssetsHolder* assets);

    void setPositions();
    void drawReformat();

    void drawAllNode(Node* node);
    void drawAllNodeFadeIn(Node* node);
    void drawAllEdge(Node* node);
    void drawAllEdgeFadeIn(Node* node);
    void drawAllEdgeFixed(Node* node);
};

#endif // BINARY_TREE_VISUALIZER_HPP