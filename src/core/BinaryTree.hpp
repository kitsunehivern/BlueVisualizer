#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

template<class T>
class BinaryTree {
public:
    struct Node {
        T value;
        Node* left;
        Node* right;

        Node(const T& inital = T());
    };

    BinaryTree();
    ~BinaryTree();

    int size() const;
    int height(Node* node) const;

    void clear();

protected:
    Node* mRoot;
    int mSize;

private:
    
    void deleteNode(Node* node);
};

template<class T>
BinaryTree<T>::Node::Node(const T& inital) {
    value = inital;
    left = nullptr;
    right = nullptr;
}

template<class T>
BinaryTree<T>::BinaryTree() {
    mRoot = nullptr;
    mSize = 0;
}

template<class T>
BinaryTree<T>::~BinaryTree() {
    deleteNode(mRoot);
}

template<class T>
int BinaryTree<T>::size() const {
    return mSize;
}

template<class T>
int BinaryTree<T>::height(Node* node) const {
    if (node == nullptr) {
        return 0;
    }

    int leftHeight = height(node->left);
    int rightHeight = height(node->right);

    return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
}

template<class T>
void BinaryTree<T>::clear() {
    deleteNode(mRoot);
    mRoot = nullptr;
    mSize = 0;
}

template<class T>
void BinaryTree<T>::deleteNode(Node* node) {
    if (node == nullptr) {
        return;
    }

    deleteNode(node->left);
    deleteNode(node->right);

    delete node;
}

#endif // BINARY_TREE_HPP