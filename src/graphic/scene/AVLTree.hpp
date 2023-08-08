#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "Visualizer.hpp"
#include "../../core/BinaryTree.hpp"

namespace AVLTreeData {
    const int minSize = 1;
    const int maxSize = 30;
    const int minValue = 0;
    const int maxValue = 99;
    
    const float treePositionY = 100.f;
    const sf::Vector2f space = sf::Vector2f(50.f, 80.f);
}

class AVLTree : private Visualizer, private BinaryTree<GraphicNode> {
public:
    AVLTree();
    AVLTree(sf::RenderWindow* window, AssetsHolder* assets);

    void run();

private:
    void setPositions();
    std::vector<GraphicNode*> getGraphicNodes(Node* root);
    int getBalanceFactor(Node* node);
    Node* leftRotate(Node* node);
    Node* rightRotate(Node* node);
    void drawAllEdge(Node* node);
    void drawAllEdgeFadeIn(Node* node);

    void create(int n);
};

#endif // AVL_TREE_HPP