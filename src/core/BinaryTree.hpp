#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

class BinaryTree {
public:
    struct Node {
        int value;
        Node* left;
        Node* right;
    };

    BinaryTree();
    ~BinaryTree();

    int size() const;
    int height() const;

protected:
    Node* mRoot;
    int mSize;

private:
    int getHeight(Node* node) const;
    void deleteNode(Node* node);
};

#endif // BINARY_TREE_HPP