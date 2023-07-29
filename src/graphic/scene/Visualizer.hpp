#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include "../../Global.hpp"
#include "../AssetsHolder.hpp"
#include "../component/OptionBox.hpp"
#include "../component/ControlBox.hpp"

class Visualizer {
protected:
    Visualizer();
    Visualizer(sf::RenderWindow* window, AssetsHolder* assets);

    void updateState();
    void handleEvent(sf::Event event);

protected:
    sf::RenderWindow* mWindow;
    AssetsHolder* mAssets;
    OptionBox mOption;
    ControlBox mControl;
};

#endif // VISUALIZER_HPP