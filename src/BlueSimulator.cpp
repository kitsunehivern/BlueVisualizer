#include "Global.hpp"
#include "graphic/AssetsHolder.hpp"
#include "graphic/scene/HashTable.hpp"
#include "graphic/scene/AVLTree.hpp"
#include "graphic/scene/BinaryHeap.hpp"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "BlueSimulator", sf::Style::Close, settings);
    window.setFramerateLimit(60);

    AssetsHolder assets;
    std::string imageDir = "assets/image/";
    assets.load(AssetsData::Image::hollowCircle, imageDir + "object/HollowCircle.png");
    assets.load(AssetsData::Image::filledCircle, imageDir + "object/FilledCircle.png");
    assets.load(AssetsData::Image::hollowSquare, imageDir + "object/HollowSquare.png");
    assets.load(AssetsData::Image::filledSquare, imageDir + "object/FilledSquare.png");
    assets.load(AssetsData::Image::stick, imageDir + "object/Stick.png");

    assets.load(AssetsData::Image::inputBox, imageDir + "component/InputBox.png");
    assets.load(AssetsData::Image::fileBox, imageDir + "component/FileBox.png");
    assets.load(AssetsData::Image::randomBox, imageDir + "component/RandomBox.png");
    assets.load(AssetsData::Image::randomIcon, imageDir + "component/RandomIcon.png");
    assets.load(AssetsData::Image::optionTable, imageDir + "component/OptionTable.png");
    assets.load(AssetsData::Image::suboptionTable, imageDir + "component/SuboptionTable.png");
    assets.load(AssetsData::Image::inputTable, imageDir + "component/InputTable.png");
    assets.load(AssetsData::Image::confirmBox, imageDir + "component/ConfirmBox.png");
    assets.load(AssetsData::Image::controlBox, imageDir + "component/ControlBox.png");
    assets.load(AssetsData::Image::controlButtons, imageDir + "component/ControlButtons.png");
    assets.load(AssetsData::Image::statusButtons, imageDir + "component/StatusButtons.png");
    assets.load(AssetsData::Image::videoBar, imageDir + "component/VideoBar.png");
    assets.load(AssetsData::Image::codeBox, imageDir + "component/CodeBox.png");
    assets.load(AssetsData::Image::codeBar, imageDir + "component/CodeBar.png");
    assets.load(AssetsData::Image::speedButton, imageDir + "component/SpeedButton.png");
    assets.load(AssetsData::Image::speed, imageDir + "component/Speed.png");

    assets.load(AssetsData::Font::consolasBold, "assets/font/consolas_bold.ttf");

    assets.set(AssetsData::Color::none, sf::Color(0, 0, 0, 0));

    assets.set(AssetsData::Color::node, sf::Color(0, 0, 0, 255));
    assets.set(AssetsData::Color::nodeFocus1, sf::Color(255, 140, 40, 255));
    assets.set(AssetsData::Color::nodeFocus2, sf::Color(45, 185, 210, 255));
    assets.set(AssetsData::Color::nodeFocus3, sf::Color(90, 200, 90, 255));
    assets.set(AssetsData::Color::nodeFocus4, sf::Color(215, 80, 60, 255));
    assets.set(AssetsData::Color::nodeText, sf::Color(0, 0, 0, 255));
    assets.set(AssetsData::Color::nodeTextFocus1, sf::Color(255, 255, 255, 255));
    assets.set(AssetsData::Color::nodeTextFocus2, sf::Color(255, 140, 40, 255));

    assets.set(AssetsData::Color::edge, sf::Color(0, 0, 0, 255));
    assets.set(AssetsData::Color::edgeFocus, sf::Color(255, 140, 40, 255));

    assets.set(AssetsData::Color::boxComponent, sf::Color(240, 245, 245, 150));
    assets.set(AssetsData::Color::box, sf::Color(105, 210, 255, 255));
    assets.set(AssetsData::Color::boxFocus, sf::Color(45, 125, 185, 255));
    assets.set(AssetsData::Color::boxActive, sf::Color(70, 255, 140, 255));
    assets.set(AssetsData::Color::boxInvalid, sf::Color(255, 150, 150, 255));
    assets.set(AssetsData::Color::boxDisabled, sf::Color(180, 180, 180, 255));
    assets.set(AssetsData::Color::boxText, sf::Color(0, 0, 0, 255));
    assets.set(AssetsData::Color::boxTextFocus, sf::Color(255, 255, 255, 255));

    assets.set(AssetsData::Color::label, sf::Color(255, 40, 40, 255));

    // HashTable HT(&window, &assets);
    // HT.run();

    // AVLTree AVL(&window, &assets);
    // AVL.run();

    BinaryHeap BH(&window, &assets);
    BH.run();

    return EXIT_SUCCESS;
}