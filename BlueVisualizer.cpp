#include "Global.h"
#include "Assets.h"
#include "Menu.h"
#include "SLLVisualizer.h"
#include "DLLVisualizer.h"
#include "CLLVisualizer.h"
#include "StackVisualizer.h"
#include "QueueVisualizer.h"
#include "StaticArrayVisualizer.h"
#include "DynamicArrayVisualizer.h"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "BlueVisualizer", sf::Style::Close, settings);
    window.setFramerateLimit(60);

    Assets assets;

    int current = -1;
    while (window.isOpen()) {
        if (current != -1) {
            if (current == 0) {
                SLLVisualizer sll(&window, &assets);
                sll.run();
            } else if (current == 1) {
                DLLVisualizer dll(&window, &assets);
                dll.run();
            } else if (current == 2) {
                CLLVisualizer cll(&window, &assets);
                cll.run();
            } else if (current == 3) {
                StackVisualizer stack(&window, &assets);
                stack.run();
            } else if (current == 4) {
                QueueVisualizer queue(&window, &assets);
                queue.run();
            } else if (current == 5) {
                StaticArrayVisualizer staticArray(&window, &assets);
                staticArray.run();
            } else {
                DynamicArrayVisualizer dynamicArray(&window, &assets);
                dynamicArray.run();
            }

            current = -1;
        } else {
            Menu menu(&window, &assets);
            current = menu.run();   
        }
    }

	return EXIT_SUCCESS;
}