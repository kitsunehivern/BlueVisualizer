#ifndef CORE_H
#define CORE_H

#include "Global.h"
#include "Assets.h"
#include "SLLVisualizer.h"
#include "DLLVisualizer.h"
#include "CLLVisualizer.h"
#include "StackVisualizer.h"
#include "QueueVisualizer.h"
#include "StaticArrayVisualizer.h"

namespace Core {

sf::RenderWindow window;

void run() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;

    window.create(sf::VideoMode(1600, 900), "BlueVisualizer", sf::Style::Close, settings);
    window.setFramerateLimit(60);

    Assets assets;

    while (window.isOpen()) {
        //SLLVisualizer sll(&window, &assets);
        //sll.run();

        //DLLVisualizer dll(&window, &assets);
        //dll.run();

        //CLLVisualizer cll(&window, &assets);
        //cll.run();

        //StackVisualizer stack(&window, &assets);
        //stack.run();

        //QueueVisualizer queue(&window, &assets);
        //queue.run();

        StaticArrayVisualizer staticArray(&window, &assets);
        staticArray.run();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }
    }
}

} // namespace Core

#endif