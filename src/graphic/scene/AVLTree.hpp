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
    Node* mErasedNode;

    void setPositions();
    int getBalanceFactor(Node* node);
    Node* leftRotate(Node* node);
    Node* rightRotate(Node* node);

    void drawAllNode(Node* node);
    void drawAllNodeFadeIn(Node* node);
    void drawAllEdge(Node* node);
    void drawAllEdgeFadeIn(Node* node);

    void drawReformat();

    void freeMemory();

    void create(int n);
    void search(int value);
    void insert(int value);
    void erase(int value);
};

#endif // AVL_TREE_HPP