#ifndef HASH_TABLE_VISUALIZER_HPP
#define HASH_TABLE_VISUALIZER_HPP

#include "Visualizer.hpp"

namespace HashTableVisualizerData {
    const int minCapacity = 1;
    const int maxCapacity = 50;
    const int defaultCapacity = 30;
    const int minSize = 1;
    const int maxSize = 50;
    const int defaultSize = 15;
    const int maxSizePerLine = 10;
    const int minValue = 0;
    const int maxValue = 99;

    const sf::Vector2f position = sf::Vector2f(325.f, 80.f);
    const sf::Vector2f space = sf::Vector2f(100.f, 100.f);

    const std::string emptyValue = "";
    const std::string deletedValue = "X";
}

class HashTableVisualizer : private Visualizer {
public:
    HashTableVisualizer();
    HashTableVisualizer(sf::RenderWindow* window, AssetsHolder* assets);

    void run();

private:
    int mCapacity;
    int mSize;
    std::vector<GraphicNode> mTable;

    std::vector<GraphicNode*> getNodes(int left, int right);
    void drawAllLabels();
    void drawAllLabelsFadeIn();

    void create(int capcity, int size);
    void search(int key);
    void insert(int key);
    void erase(int key);
};

#endif // HASH_TABLE_VISUALIZER_HPP