#ifndef B_TREE_VISUALIZER_HPP
#define B_TREE_VISUALIZER_HPP

#include "Visualizer.hpp"

namespace BTreeVisualizerData {
    const int minSize = 1;
    const int maxSize = 30;
    const int defaultSize = 15;
    const int minValue = 0;
    const int maxValue = 99;

    const sf::Vector2f space = sf::Vector2f(50.f, 80.f);
    const float treePositionY = 100.f;
}

class BTreeVisualizer : public Visualizer {
public:
    BTreeVisualizer();
    BTreeVisualizer(sf::RenderWindow* window, AssetsHolder* assets);

    void run();

private:
    struct Node {
        std::vector<GraphicNode*> values;
        std::vector<Node*> children;

        Node();

        float getLength();
        sf::Vector2f getUpperPoint();
        sf::Vector2f getLowerPoint(int index);
    };

    Node* mRoot;
    std::map<GraphicNode*, sf::Vector2f> mOldPosition;
    
    int getSize();
    void clearTree();
    int getChildIndex(Node* node, int value);

    void setPositions();

    void drawReformat();
    void drawAllNode(Node* node);
    void drawAllEdge(Node* node);
    void drawAllNodeFadeIn(Node* node);
    void drawAllEdgeFadeIn(Node* node);
    void drawAllEdgeChangePosition(Node* node);

    void create(int size);
    void createFromList(std::string input);
    void insert(int value);
};

#endif // B_TREE_VISUALIZER_HPP