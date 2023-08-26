#include "Global.hpp"
#include "graphic/AssetsHolder.hpp"
#include "graphic/Menu.hpp"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "BlueVisualizer", sf::Style::Close, settings);
    window.setFramerateLimit(60);

    AssetsHolder assets;
    std::string imageDir = "assets/image/";
    assets.load(AssetsData::Image::lightBackground, imageDir + "theme/LightBackground.png");
    assets.load(AssetsData::Image::darkBackground, imageDir + "theme/DarkBackground.png");
    assets.load(AssetsData::Image::lightLogo, imageDir + "theme/LightLogo.png");
    assets.load(AssetsData::Image::darkLogo, imageDir + "theme/DarkLogo.png");

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
    assets.load(AssetsData::Image::optionPrevButton, imageDir + "component/OptionPrevButton.png");
    assets.load(AssetsData::Image::optionNextButton, imageDir + "component/OptionNextButton.png");
    assets.load(AssetsData::Image::suboptionTable, imageDir + "component/SuboptionTable.png");
    assets.load(AssetsData::Image::suboptionPrevButton, imageDir + "component/SuboptionPrevButton.png");
    assets.load(AssetsData::Image::suboptionNextButton, imageDir + "component/SuboptionNextButton.png");
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
    assets.load(AssetsData::Image::visualizeBox, imageDir + "component/VisualizeBox.png");
    assets.load(AssetsData::Image::quitButton, imageDir + "component/QuitButton.png");
    assets.load(AssetsData::Image::themeBox, imageDir + "component/ThemeBox.png");
    assets.load(AssetsData::Image::themeButton, imageDir + "component/ThemeButton.png");
    assets.load(AssetsData::Image::titleBox, imageDir + "component/TitleBox.png");

    assets.load(AssetsData::Font::consolasBold, "assets/font/consolas_bold.ttf");
    assets.load(AssetsData::Font::hyperspaceItalicBold, "assets/font/hyperspace_race_expanded_heavy_italic.ttf");

    // Light theme colors
    assets.setLight(AssetsData::Color::none, sf::Color(0, 0, 0, 0));

    assets.setLight(AssetsData::Color::node, sf::Color(0, 0, 0, 255));
    assets.setLight(AssetsData::Color::nodeFocus1, sf::Color(255, 140, 40, 255));
    assets.setLight(AssetsData::Color::nodeFocus2, sf::Color(45, 185, 210, 255));
    assets.setLight(AssetsData::Color::nodeFocus3, sf::Color(90, 200, 90, 255));
    assets.setLight(AssetsData::Color::nodeFocus4, sf::Color(215, 80, 60, 255));
    assets.setLight(AssetsData::Color::nodeText, sf::Color(0, 0, 0, 255));
    assets.setLight(AssetsData::Color::nodeTextFocus1, sf::Color(255, 255, 255, 255));
    assets.setLight(AssetsData::Color::nodeTextFocus2, sf::Color(255, 140, 40, 255));
    assets.setLight(AssetsData::Color::nodeTextFocus3, sf::Color(45, 185, 210, 255));

    assets.setLight(AssetsData::Color::edge, sf::Color(0, 0, 0, 255));
    assets.setLight(AssetsData::Color::edgeFocus, sf::Color(255, 140, 40, 255));

    assets.setLight(AssetsData::Color::boxBackground, sf::Color(160, 225, 250, 150));
    assets.setLight(AssetsData::Color::boxComponent, sf::Color(240, 245, 245, 150));
    assets.setLight(AssetsData::Color::box, sf::Color(105, 210, 255, 255));
    assets.setLight(AssetsData::Color::boxFocus, sf::Color(45, 125, 185, 255));
    assets.setLight(AssetsData::Color::boxActive, sf::Color(70, 255, 140, 255));
    assets.setLight(AssetsData::Color::boxInvalid, sf::Color(255, 150, 150, 255));
    assets.setLight(AssetsData::Color::boxDisabled, sf::Color(200, 200, 200, 200));
    assets.setLight(AssetsData::Color::boxText, sf::Color(0, 0, 0, 255));
    assets.setLight(AssetsData::Color::boxTextFocus, sf::Color(255, 255, 255, 255));

    assets.setLight(AssetsData::Color::label, sf::Color(255, 40, 40, 255));

    // Dark theme colors
    assets.setDark(AssetsData::Color::none, sf::Color(0, 0, 0, 0));

    assets.setDark(AssetsData::Color::node, sf::Color(255, 255, 255, 255));
    assets.setDark(AssetsData::Color::nodeFocus1, sf::Color(255, 145, 55, 255));
    assets.setDark(AssetsData::Color::nodeFocus2, sf::Color(45, 185, 210, 255));
    assets.setDark(AssetsData::Color::nodeFocus3, sf::Color(90, 200, 90, 255));
    assets.setDark(AssetsData::Color::nodeFocus4, sf::Color(215, 80, 60, 255));
    assets.setDark(AssetsData::Color::nodeText, sf::Color(255, 255, 255, 255));
    assets.setDark(AssetsData::Color::nodeTextFocus1, sf::Color(255, 255, 255, 255));
    assets.setDark(AssetsData::Color::nodeTextFocus2, sf::Color(255, 145, 55, 255));
    assets.setDark(AssetsData::Color::nodeTextFocus3, sf::Color(45, 185, 210, 255));

    assets.setDark(AssetsData::Color::edge, sf::Color(255, 255, 255, 255));
    assets.setDark(AssetsData::Color::edgeFocus, sf::Color(255, 145, 55, 255));

    assets.setDark(AssetsData::Color::boxBackground, sf::Color(60, 125, 195, 150));
    assets.setDark(AssetsData::Color::boxComponent, sf::Color(40, 70, 115, 150));
    assets.setDark(AssetsData::Color::box, sf::Color(45, 125, 185, 255));
    assets.setDark(AssetsData::Color::boxFocus, sf::Color(105, 210, 255, 255));
    assets.setDark(AssetsData::Color::boxActive, sf::Color(45, 225, 120, 255));
    assets.setDark(AssetsData::Color::boxInvalid, sf::Color(230,  65,  80, 255));
    assets.setDark(AssetsData::Color::boxDisabled, sf::Color(200, 200, 200, 200));
    assets.setDark(AssetsData::Color::boxText, sf::Color(255, 255, 255, 255));
    assets.setDark(AssetsData::Color::boxTextFocus, sf::Color(0, 0, 0, 255));

    assets.setDark(AssetsData::Color::label, sf::Color(255, 255, 255, 255));

    assets.switchTheme();
    assets.switchTheme();

    Menu menu(&window, &assets);
    menu.run();

    return EXIT_SUCCESS;
}