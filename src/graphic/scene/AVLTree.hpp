#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "../../core/BinaryTree.hpp"
#include "Visualizer.hpp"

class AVLTree : private Visualizer, private BinaryTree {
public:
    AVLTree();
    AVLTree(sf::RenderWindow* window, AssetsHolder* assets);

    void run();
};

#endif // AVL_TREE_HPP