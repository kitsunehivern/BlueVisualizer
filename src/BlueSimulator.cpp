#include "Global.hpp"
#include "graphic/AssetsHolder.hpp"
#include "graphic/scene/HashTable.hpp"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "BlueSimulator", sf::Style::Close, settings);
    window.setFramerateLimit(60);

    AssetsHolder assets;
    assets.load(AssetsData::Image::hollowCircle, "assets/image/object/HollowCircle.png");
    assets.load(AssetsData::Image::filledCircle, "assets/image/object/FilledCircle.png");
    assets.load(AssetsData::Image::hollowSquare, "assets/image/object/HollowSquare.png");
    assets.load(AssetsData::Image::filledSquare, "assets/image/object/FilledSquare.png");
    assets.load(AssetsData::Image::markedSquare, "assets/image/object/MarkedSquare.png");

    assets.load(AssetsData::Image::inputBox, "assets/image/component/InputBox.png");
    assets.load(AssetsData::Image::fileBox, "assets/image/component/FileBox.png");
    assets.load(AssetsData::Image::randomBox, "assets/image/component/RandomBox.png");
    assets.load(AssetsData::Image::randomIcon, "assets/image/component/RandomIcon.png");
    assets.load(AssetsData::Image::optionTable, "assets/image/component/OptionTable.png");
    assets.load(AssetsData::Image::suboptionTable, "assets/image/component/SuboptionTable.png");
    assets.load(AssetsData::Image::inputTable, "assets/image/component/InputTable.png");
    assets.load(AssetsData::Image::confirmBox, "assets/image/component/ConfirmBox.png");
    assets.load(AssetsData::Image::controlBox, "assets/image/component/ControlBox.png");
    assets.load(AssetsData::Image::controlButtons, "assets/image/component/ControlButtons.png");
    assets.load(AssetsData::Image::statusButtons, "assets/image/component/StatusButtons.png");
    assets.load(AssetsData::Image::videoBar, "assets/image/component/VideoBar.png");
    assets.load(AssetsData::Image::codeBox, "assets/image/component/CodeBox.png");
    assets.load(AssetsData::Image::codeBar, "assets/image/component/CodeBar.png");

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

    assets.set(AssetsData::Color::boxComponent, sf::Color(240, 245, 245, 150));
    assets.set(AssetsData::Color::box, sf::Color(105, 210, 255, 255));
    assets.set(AssetsData::Color::boxFocus, sf::Color( 45, 125, 185, 255));
    assets.set(AssetsData::Color::boxText, sf::Color(0, 0, 0, 255));
    assets.set(AssetsData::Color::boxTextFocus, sf::Color(255, 255, 255, 255));
    assets.set(AssetsData::Color::boxActive, sf::Color(70, 255, 140, 255));
    assets.set(AssetsData::Color::boxInvalid, sf::Color(255, 150, 150, 255));
    assets.set(AssetsData::Color::boxDisabled, sf::Color(150, 150, 150, 255));

    HashTable HT(&window, &assets);
    HT.run();

    return EXIT_SUCCESS;
}