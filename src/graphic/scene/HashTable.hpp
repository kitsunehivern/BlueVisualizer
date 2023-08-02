#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "Visualizer.hpp"

namespace HashTableData {
    const int minCapacity = 1;
    const int maxCapacity = 50;
    const int minSize = 1;
    const int maxSize = 30;
    const int maxSizePerLine = 10;
    const int minValue = 0;
    const int maxValue = 99;

    const sf::Vector2f position = sf::Vector2f(325.f, 80.f);
    const sf::Vector2f space = sf::Vector2f(100.f, 100.f);

    const std::string emptyValue = "";
    const std::string deletedValue = "X";
}

class HashTable : private Visualizer {
public:
    HashTable();
    HashTable(sf::RenderWindow* window, AssetsHolder* assets);

    void run();
private:
    int mCapacity;
    int mSize;
    std::vector<Node> mTable;
    std::vector<Label> mLabels;

    std::vector<Node*> getNodes(int left, int right);
    std::vector<Label*> getLabels(int left, int right);

    void create(int capcity, int size);
    void search(int key);
    void insert(int key);
    void erase(int key);
};

#endif // HASH_TABLE_HPP