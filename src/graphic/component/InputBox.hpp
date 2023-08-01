#ifndef INPUT_BOX_HPP
#define INPUT_BOX_HPP

#include "../../Global.hpp"
#include "../AssetsHolder.hpp"

namespace InputBoxData {
    const float characterSize = 25.f;
    const float inputBoxWidth = 250.f;
    const float inputBoxHeight = 50.f;
    const float randomBoxWidth = 50.f;
    const float randomBoxHeight = 50.f;
    const float fileBoxWidth = 300.f;
    const float fileBoxHeight = 50.f;
    const sf::Vector2f textPosition = sf::Vector2f(20.f, 8.f);
    const int maxCharacter = 50;

    enum InputType {
        keyboard,
        file
    };

    namespace CharacterSet {
        const std::set<char> alphabet = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm','n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
        const std::set<char> digit = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        const std::set<char> mark = {',', '.', ';'};
        const std::set<char> space = {' '};
    }
}

class InputBox : public sf::Drawable {
public:
    InputBox();
    InputBox(InputBoxData::InputType type, AssetsHolder* assets, sf::Vector2f position, std::string name);

    void setPosition(sf::Vector2f position);
    void setValidCharacters(std::set<char> validCharacters);
    void setValidator(std::function<std::string(std::string, std::string)> validator);
    void setRandomizer(std::function<std::string()> randomizer);
    std::string getError() const;
    std::string getValue() const;

    void updateState(sf::RenderWindow* window);
    void handleEvent(sf::RenderWindow* window, sf::Event event);

private:
    InputBoxData::InputType mType;

    AssetsHolder* mAssets;
    sf::Vector2f mPosition;
    std::string mName;
    std::string mValue;
    std::set<char> mValidCharacters;
    std::function<std::string(std::string, std::string)> mValidator;
    std::function<std::string()> mRandomizer;

    bool mIsInputBoxFocused;
    bool mIsRandomBoxFocused;
    bool mIsInputBoxSelected;
    sf::Clock mSelectedClock;

    bool mIsFileBoxFocused;

    void insertCharacter(char character);
    void deleteCharacter();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // INPUT_BOX_HPP