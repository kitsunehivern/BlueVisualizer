#include "Global.hpp"
#include "graphic/AssetsHolder.hpp"
#include "graphic/object/Node.hpp"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "BlueSimulator", sf::Style::Close, settings);
    window.setFramerateLimit(60);

    AssetsHolder assetsHolder;
    assetsHolder.load(AssetsData::hollowCircle, "assets/image/object/HollowCircle.png");
    assetsHolder.load(AssetsData::solidCircle, "assets/image/object/SolidCircle.png");
    assetsHolder.load(AssetsData::consolasBold, "assets/font/consolas_bold.ttf");
    assetsHolder.set(AssetsData::nodeCircleColor, sf::Color(0, 0, 0, 255));
    assetsHolder.set(AssetsData::nodeFocusCircleColor1, sf::Color(255, 140, 40, 255));
    assetsHolder.set(AssetsData::nodeFocusCircleColor2, sf::Color(45, 185, 210, 255));
    assetsHolder.set(AssetsData::nodeFocusCircleColor3, sf::Color(90, 200, 90, 255));
    assetsHolder.set(AssetsData::nodeFocusCircleColor4, sf::Color(215, 80, 60, 255));
    assetsHolder.set(AssetsData::nodeTextColor, sf::Color(0, 0, 0, 255));
    assetsHolder.set(AssetsData::nodeFocusTextColor1, sf::Color(255, 255, 255, 255));
    assetsHolder.set(AssetsData::nodeFocusTextColor2, sf::Color(255, 140, 40, 255));

    Node node(&window, &assetsHolder);
    node.setValue(10);
    node.setPosition(sf::Vector2f(100, 100));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::White);

        node.draw(NodeData::hollow, NodeData::none, { assetsHolder.get(AssetsData::nodeCircleColor), assetsHolder.get(AssetsData::nodeTextColor) }, 0.f);

        window.display();
    }

    return EXIT_SUCCESS;
}