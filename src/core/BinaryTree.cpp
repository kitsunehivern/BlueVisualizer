#include "BinaryTree.hpp"

BinaryTree::BinaryTree() {
    mRoot = nullptr;
    mSize = 0;
}

BinaryTree::~BinaryTree() {
    deleteNode(mRoot);
}

int BinaryTree::size() const {
    return mSize;
}

int BinaryTree::height() const {
    return getHeight(mRoot);
}

int BinaryTree::getHeight(Node* node) const {
    if (node == nullptr) {
        return 0;
    }

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);

    return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
}

void BinaryTree::deleteNode(Node* node) {
    if (node == nullptr) {
        return;
    }

    deleteNode(node->left);
    deleteNode(node->right);

    delete node;
}