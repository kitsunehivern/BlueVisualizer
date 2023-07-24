#ifndef INPUT_BOX_HPP
#define INPUT_BOX_HPP

#include "../../Global.hpp"
#include "../AssetsHolder.hpp"

namespace InputBoxData {
    const float characterSize = 25.f;
    const float inputBoxWidth = 300.f;
    const float inputBoxHeight = 50.f;
    const float randomBoxWidth = 50.f;
    const float randomBoxHeight = 50.f;
    const float fileBoxWidth = 350.f;
    const float fileBoxHeight = 50.f;
    const float textPositionX = 20.f;
    const float textPositionY = 8.f;

    enum InputType {
        keyboard,
        file
    };
}

class InputBox : public sf::Drawable {
public:
    InputBox();
    InputBox(InputBoxData::InputType type, AssetsHolder* assets, sf::Vector2f position, std::string name);

    void setPosition(sf::Vector2f position);
    void setValidCharacters(std::vector<char> validCharacters);
    void setValidator(std::function<std::string(std::string, std::string)> validator);
    void setRandomizer(std::function<std::string()> randomizer);

    void handleEvent(sf::RenderWindow* window, sf::Event event);

private:
    InputBoxData::InputType mType;

    AssetsHolder* mAssets;
    sf::Vector2f mPosition;
    std::string mName;
    std::string mValue;
    std::vector<char> mValidCharacters;
    std::function<std::string(std::string, std::string)> mValidator;
    std::function<std::string()> mRandomizer;

    bool mIsInputBoxFocused;
    bool mIsRandomBoxFocused;
    bool mIsInputBoxSelected;
    sf::Clock mSelectedClock;

    bool mIsFileBoxFocused;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // INPUT_BOX_HPP