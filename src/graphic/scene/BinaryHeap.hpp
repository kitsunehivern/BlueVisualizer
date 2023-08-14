#ifndef BINARY_HEAP_HPP
#define BINARY_HEAP_HPP

#include "BinaryTreeVisualizer.hpp"

namespace BinaryHeapData {
    const int minSize = 1;
    const int maxSize = 31;
    const int minValue = 1;
    const int maxValue = 98;

    enum HeapType {
        MaxHeap,
        MinHeap
    };
}

class BinaryHeap : private BinaryTreeVisualizer {
public:
    BinaryHeap();
    BinaryHeap(sf::RenderWindow* window, AssetsHolder* assets);

    void run();

private:
    BinaryHeapData::HeapType mHeapType;

    std::vector<Node*> mNodes;
    Node* mErasedNode;

    bool compare(int value1, int value2);

    void setBinaryTree();
    void drawAllLabel(Node* node, int index);
    void drawAllLabelFadeIn(Node* node, int index);
    void drawAllLabelFixed(Node* node, int index);

    void freeMemory();

    void create(int size);
    void getSize();
    void getTop();
    void eraseTop();
    void insert(int value);
    void erase(int index);
};

#endif // BINARY_HEAP_HPP