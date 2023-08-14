#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "BinaryTreeVisualizer.hpp"

namespace AVLTreeData {
    const int minSize = 1;
    const int maxSize = 31;
    const int minValue = 0;
    const int maxValue = 99;
}

class AVLTree : private BinaryTreeVisualizer {
public:
    AVLTree();
    AVLTree(sf::RenderWindow* window, AssetsHolder* assets);

    void run();

private:
    Node* mErasedNode;

    int getBalanceFactor(Node* node);
    Node* leftRotate(Node* node);
    Node* rightRotate(Node* node);

    void freeMemory();

    void create(int size);
    void search(int value);
    void insert(int value);
    void erase(int value);
};

#endif // AVL_TREE_HPP