#ifndef MENU_HPP
#define MENU_HPP

#include "../Global.hpp"
#include "component/Button.hpp"
#include "visualizer/HashTableVisualizer.hpp"
#include "visualizer/AVLTreeVisualizer.hpp"
#include "visualizer/BinaryHeapVisualizer.hpp"
#include "visualizer/TrieVisualizer.hpp"
#include "visualizer/BTreeVisualizer.hpp"
#include "visualizer/GraphVisualizer.hpp"

class Menu {
public:
    Menu(sf::RenderWindow* window, AssetsHolder* assets);

    void run();

private:
    sf::RenderWindow* mWindow;
    AssetsHolder* mAssets;

    Button mHashTableButton;
    Button mAVLTreeButton;
    Button mBinaryHeapButton;
    Button mTrieButton;
    Button mBTreeButton;
    Button mGraphButton;

    void updateState();
    void handleEvent(sf::Event event);
    void draw();
};

#endif // MENU_HPP