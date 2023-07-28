#include "Global.hpp"
#include "graphic/AssetsHolder.hpp"
#include "graphic/component/OptionBox.hpp"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "BlueSimulator", sf::Style::Close, settings);
    window.setFramerateLimit(60);

    AssetsHolder assets;
    assets.load(AssetsData::Image::hollowCircle, "assets/image/object/HollowCircle.png");
    assets.load(AssetsData::Image::solidCircle, "assets/image/object/SolidCircle.png");
    assets.load(AssetsData::Image::inputBox, "assets/image/component/InputBox.png");
    assets.load(AssetsData::Image::fileBox, "assets/image/component/FileBox.png");
    assets.load(AssetsData::Image::randomBox, "assets/image/component/RandomBox.png");
    assets.load(AssetsData::Image::randomIcon, "assets/image/component/RandomIcon.png");
    assets.load(AssetsData::Image::optionTable, "assets/image/component/OptionTable.png");
    assets.load(AssetsData::Image::suboptionTable, "assets/image/component/SuboptionTable.png");
    assets.load(AssetsData::Image::inputTable, "assets/image/component/InputTable.png");
    assets.load(AssetsData::Image::confirmBox, "assets/image/component/ConfirmBox.png");

    assets.load(AssetsData::Font::consolasBold, "assets/font/consolas_bold.ttf");

    assets.set(AssetsData::Color::nodeCircle, sf::Color(0, 0, 0, 255));
    assets.set(AssetsData::Color::nodeCircleFocus1, sf::Color(255, 140, 40, 255));
    assets.set(AssetsData::Color::nodeCircleFocus2, sf::Color(45, 185, 210, 255));
    assets.set(AssetsData::Color::nodeCircleFocus3, sf::Color(90, 200, 90, 255));
    assets.set(AssetsData::Color::nodeCircleFocus4, sf::Color(215, 80, 60, 255));
    assets.set(AssetsData::Color::nodeText, sf::Color(0, 0, 0, 255));
    assets.set(AssetsData::Color::nodeTextFocus1, sf::Color(255, 255, 255, 255));
    assets.set(AssetsData::Color::nodeTextFocus2, sf::Color(255, 140, 40, 255));

    assets.set(AssetsData::Color::boxComponent, sf::Color(240, 245, 245, 150));
    assets.set(AssetsData::Color::box, sf::Color(105, 210, 255, 255));
    assets.set(AssetsData::Color::boxFocus, sf::Color( 45, 125, 185, 255));
    assets.set(AssetsData::Color::boxText, sf::Color(0, 0, 0, 255));
    assets.set(AssetsData::Color::boxTextFocus, sf::Color(255, 255, 255, 255));

    std::function <bool()> conditionNone = []() { return true; };

    OptionBox optionBox(&assets);
    optionBox.addOption("Create");
    optionBox.addSuboption("Random", conditionNone);
    optionBox.addSuboptionInputBox(InputBoxData::InputType::keyboard, "n");
    optionBox.addSuboption("Manual", conditionNone);
    optionBox.addSuboptionInputBox(InputBoxData::InputType::keyboard, "v[]");
    optionBox.addSuboptionInputBox(InputBoxData::InputType::file, "file");
    
    optionBox.addOption("Search");
    optionBox.addSuboption("Value", conditionNone);
    optionBox.addSuboptionInputBox(InputBoxData::InputType::keyboard, "v");

    optionBox.addOption("Update");
    optionBox.addSuboption("Value", conditionNone);
    optionBox.addSuboptionInputBox(InputBoxData::InputType::keyboard, "v");

    optionBox.addOption("Insert");
    optionBox.addSuboption("Front", conditionNone);
    optionBox.addSuboptionInputBox(InputBoxData::InputType::keyboard, "v");
    optionBox.addSuboption("Back", conditionNone);
    optionBox.addSuboptionInputBox(InputBoxData::InputType::keyboard, "v");
    optionBox.addSuboption("Middle", conditionNone);
    optionBox.addSuboptionInputBox(InputBoxData::InputType::keyboard, "i");
    optionBox.addSuboptionInputBox(InputBoxData::InputType::keyboard, "v");

    optionBox.addOption("Delete");
    optionBox.addSuboption("Front", conditionNone);
    optionBox.addSuboption("Back", conditionNone);
    optionBox.addSuboption("Middle", conditionNone);
    optionBox.addSuboptionInputBox(InputBoxData::InputType::keyboard, "i");

    optionBox.processOption();

    while (window.isOpen()) {
        optionBox.updateState(&window);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            optionBox.handleEvent(&window, event);
        }

        window.clear(sf::Color::White);

        window.draw(optionBox);

        window.display();
    }

    return EXIT_SUCCESS;
}