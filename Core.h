#ifndef CORE_H
#define CORE_H

#include "Global.h"
#include "Assets.h"
#include "SLLVisualizer.h"
#include "DLLVisualizer.h"
#include "StackVisualizer.h"

namespace Core {

    sf::RenderWindow window;

    void run() {
        sf::ContextSettings settings;
        settings.antialiasingLevel = 16;

        window.create(sf::VideoMode(1600, 900), "BlueVisualizer", sf::Style::Close, settings);
        window.setFramerateLimit(60);

        Assets assets;
        
        //SLLVisualizer sll(&window, &assets);
        //sll.run();

        //DLLVisualizer dll(&window, &assets);
        //dll.run();

        StackVisualizer stack(&window, &assets);
        stack.run();
    }

} // namespace Core

#endif