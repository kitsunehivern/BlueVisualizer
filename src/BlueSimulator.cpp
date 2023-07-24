#include "Global.hpp"
#include "graphic/AssetsHolder.hpp"
#include "graphic/component/InputBox.hpp"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "BlueSimulator", sf::Style::Close, settings);
    window.setFramerateLimit(60);

    AssetsHolder assets;
    assets.load(AssetsData::hollowCircle, "assets/image/object/HollowCircle.png");
    assets.load(AssetsData::solidCircle, "assets/image/object/SolidCircle.png");
    assets.load(AssetsData::inputBox, "assets/image/component/InputBox.png");
    assets.load(AssetsData::randomBox, "assets/image/component/RandomBox.png");

    assets.load(AssetsData::consolasBold, "assets/font/consolas_bold.ttf");

    assets.set(AssetsData::nodeCircleColor, sf::Color(0, 0, 0, 255));
    assets.set(AssetsData::nodeCircleFocusColor1, sf::Color(255, 140, 40, 255));
    assets.set(AssetsData::nodeCircleFocusColor2, sf::Color(45, 185, 210, 255));
    assets.set(AssetsData::nodeCircleFocusColor3, sf::Color(90, 200, 90, 255));
    assets.set(AssetsData::nodeCircleFocusColor4, sf::Color(215, 80, 60, 255));
    assets.set(AssetsData::nodeTextColor, sf::Color(0, 0, 0, 255));
    assets.set(AssetsData::nodeTextFocusColor1, sf::Color(255, 255, 255, 255));
    assets.set(AssetsData::nodeTextFocusColor2, sf::Color(255, 140, 40, 255));

    assets.set(AssetsData::boxComponentColor, sf::Color(240, 245, 245, 150));
    assets.set(AssetsData::boxColor, sf::Color(105, 210, 255, 255));
    assets.set(AssetsData::boxFocusColor, sf::Color( 45, 125, 185, 255));
    assets.set(AssetsData::boxTextColor, sf::Color(0, 0, 0, 255));
    assets.set(AssetsData::boxTextFocusColor, sf::Color(255, 255, 255, 255));

    InputBox inputBox(InputBoxData::InputType::keyboard, &assets, sf::Vector2f(100.f, 100.f), "InputBox");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            inputBox.handleEvent(&window, event);
        }

        window.clear(sf::Color::White);

        window.draw(inputBox);

        window.display();
    }

    return EXIT_SUCCESS;
}