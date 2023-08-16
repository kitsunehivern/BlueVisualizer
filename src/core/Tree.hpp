#ifndef TREE_HPP
#define TREE_HPP

#include <map>

template<class T>
class Tree {
public:
    struct Node {
        T value;
        std::map<T, Node*> children;

        Node(const T& inital = T());
    };

    Tree();
    ~Tree();

    int size() const;
    int width(Node* node) const;
    int height(Node* node) const;

    void clear();

protected:
    Node* mRoot;

private:
    int getSize(Node* node) const;
    int getWidth(Node* node) const;
    int getHeight(Node* node) const;
    void deleteNode(Node* node);
};

template<class T>
Tree<T>::Node::Node(const T& inital) {
    value = inital;
    left = nullptr;
    right = nullptr;
}

template<class T>
Tree<T>::Tree() {
    mRoot = nullptr;
}

template<class T>
Tree<T>::~Tree() {
    clear();
}

template<class T>
int Tree<T>::size() const {
    return getSize(mRoot);
}

template<class T>
int Tree<T>::width(Node* node) const {
    return getWidth(node);
}

template<class T>
int Tree<T>::height(Node* node) const {
    return getHeight(node);
}

template<class T>
void Tree<T>::clear() {
    deleteNode(mRoot);
    mRoot = nullptr;
}

template<class T>
int Tree<T>::getSize(Node* node) const {
    if (node == nullptr) {
        return 0;
    }

    int size = 1;
    for (auto& child : node->children) {
        size += getSize(child.second);
    }

    return size;
}

template<class T>
int Tree<T>::getWidth(Node* node) const {
    if (node == nullptr) {
        return 0;
    }

    int width = 0;
    for (auto& child : node->children) {
        width += getWidth(child.second);
    }

    return width == 0 ? 1 : width;
}

template<class T>
int Tree<T>::getHeight(Node* node) const {
    if (node == nullptr) {
        return 0;
    }

    int height = 0;
    for (auto& child : node->children) {
        int childHeight = getHeight(child.second);
        if (childHeight > height) {
            height = childHeight;
        }
    }

    return height + 1;
}

template<class T>
void Tree<T>::deleteNode(Node* node) {
    if (node == nullptr) {
        return;
    }

    for (auto& child : node->children) {
        deleteNode(child.second);
    }

    delete node;
}

#endif // TREE_HPP