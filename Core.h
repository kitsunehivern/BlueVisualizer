#ifndef CORE_H
#define CORE_H

#include "Global.h"
#include "SLL.h"
#include "Dll.h"

namespace Core {

    sf::RenderWindow window;

    void run() {
        sf::ContextSettings settings;
        settings.antialiasingLevel = 16;

        window.create(sf::VideoMode(1600, 900), "BlueVisualizer", sf::Style::Close, settings);
        window.setFramerateLimit(60);

        Assets assets;
        
        //SLL sll(&window, &assets);
        //sll.run();

        DLL dll(&window, &assets);
        dll.run();
    }

} // namespace Core

#endif