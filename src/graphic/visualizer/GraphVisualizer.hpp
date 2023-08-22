#ifndef GRAPH_VISULIZER_HPP
#define GRAPH_VISULIZER_HPP

#include "Visualizer.hpp"

namespace GraphVisualizerData {
    const int minNode = 1;
    const int maxNode = 20;
    const int defaultNode = 10;
    const int defaultEdge = 10;

    const float springConstant = 0.05f;
    const float idealLength = 200.0f;
}

class GraphVisualizer : private Visualizer {
public:
    GraphVisualizer();
    GraphVisualizer(sf::RenderWindow* window, AssetsHolder* assets);

    void run();

private:
    std::vector<GraphicNode> mNodes;
    std::vector<std::vector<int>> mAdjacencyMatrix;

    std::pair<int, sf::Vector2f> mSelectedNode;

    sf::Vector2f springForce(sf::Vector2f fixedPosition, sf::Vector2f movingPosition, float idealLength);

    void updateGraphState();
    void handleGraphEvent(sf::Event event);

    void drawAllNode();
    void drawAllEdge();

    void create(int numNode, int numEdge);
    void findCC();
    void findMST();
    void findShortestPath();
};

#endif // GRAPH_VISULIZER_HPP