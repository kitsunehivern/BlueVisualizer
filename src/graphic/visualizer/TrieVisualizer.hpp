#ifndef TRIE_VISUALIZER_HPP
#define TRIE_VISUALIZER_HPP

#include "Visualizer.hpp"

namespace TrieVisualizerData {
    const int minLength = 1;
    const int maxLength = 6;
    const int minSize = 1;
    const int maxSize = 15;
    const int defaultSize = 7;
    
    const float treePositionY = 60.f;
    const sf::Vector2f space = sf::Vector2f(50.f, 80.f);
}

class TrieVisualizer : private Visualizer {
public:
    TrieVisualizer();
    TrieVisualizer(sf::RenderWindow* window, AssetsHolder* assets);

    void run();

private:
    struct Node {
        GraphicNode value;
        std::map<char, Node*> children;
        bool marked;

        Node();
        Node(GraphicNode value);
    };

    Node* mRoot;
    std::vector<Node*> mErasedNodes;

    void freeMemory();
    
    void deleteTree();

    void setPositions();

    void drawReformat();
    void drawAllNode(Node* node);
    void drawAllNodeFadeIn(Node* node);
    void drawAllNodeExceptRoot(Node* node);
    void drawAllNodeExceptChild(Node* node, char childLetter);
    void drawAllEdge(Node* node);
    void drawAllEdgeFadeIn(Node* node);
    void drawAllEdgeExceptChild(Node* node, char childLetter);

    void create(int number);
    void createFromList(std::string input);
    void search(std::string str);
    void insert(std::string str);
    void erase(std::string str);
};

#endif // TRIE_VISUALIZER_HPP