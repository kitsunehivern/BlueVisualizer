#include "Visualizer.hpp"

Visualizer::Visualizer() {
}

Visualizer::Visualizer(sf::RenderWindow* window, AssetsHolder* assets) {
    mWindow = window;
    mAssets = assets;
    mOption = OptionBox(mAssets);
    mControl = ControlBox(mAssets);
}

void Visualizer::updateState() {
    mOption.updateState(mWindow);
    mControl.updateState(mWindow);
}