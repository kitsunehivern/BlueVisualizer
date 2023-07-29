#ifndef OPTION_BOX_HPP
#define OPTION_BOX_HPP

#include "../../Global.hpp"
#include "../AssetsHolder.hpp"
#include "InputBox.hpp"
#include "Button.hpp"

namespace OptionBoxData {
    const float characterSize = 25.f;

    namespace Option {
        const float tableWidth = 180.f;
        const float tableHeight = 210.f;
        const sf::Vector2f tablePosition = sf::Vector2f(10.f, 680.f);
        const sf::FloatRect prevButtonRect = sf::FloatRect(0.f, 0.f, 180.f, 30.f);
        const sf::FloatRect prevRect = sf::FloatRect(0.f, 30.f, 180.f, 50.f);
        const sf::FloatRect currRect = sf::FloatRect(0.f, 80.f, 180.f, 50.f);
        const sf::FloatRect nextRect = sf::FloatRect(0.f, 130.f, 180.f, 50.f);
        const sf::FloatRect nextButtonRect = sf::FloatRect(0.f, 180.f, 180.f, 30.f);
    }

    namespace Suboption {
        const float tableWidth = 680.f;
        const float tableHeight = 50.f;
        const sf::Vector2f tablePosition = sf::Vector2f(210.f, 680.f);
        const sf::FloatRect prevButtonRect = sf::FloatRect(0.f, 0.f, 40.f, 50.f);
        const sf::FloatRect prevRect = sf::FloatRect(40.f, 0.f, 200.f, 50.f);
        const sf::FloatRect currRect = sf::FloatRect(240.f, 0.f, 200.f, 50.f);
        const sf::FloatRect nextRect = sf::FloatRect(440.f, 0.f, 200.f, 50.f);
        const sf::FloatRect nextButtonRect = sf::FloatRect(640.f, 0.f, 40.f, 50.f);
    }

    namespace Input {
        const float tableWidth = 680.f;
        const float tableHeight = 140.f;
        const sf::Vector2f tablePosition = sf::Vector2f(210.f, 750.f);
        const sf::Vector2f boxPosition = sf::Vector2f(190.f, 10.f);
        const sf::Vector2f box1Position = sf::Vector2f(20.f, 10.f);
        const sf::Vector2f box2Position = sf::Vector2f(360.f, 10.f);
        const float confirmBoxWidth = 680.f;
        const float confirmBoxHeight = 50.f;
        const sf::Vector2f confirmBoxPosition = sf::Vector2f(20.f, 80.f);
    }
}

class OptionBox : public sf::Drawable {
public:
    OptionBox();
    OptionBox(AssetsHolder* assets);

    void addOption(const std::string& option);
    void addSuboption(const std::string& suboption, std::function<bool()> condition);
    void addSuboptionInputBox(InputBoxData::InputType type, std::string name);
    void processOption(); // please call this when finish adding options and suboptions

    std::pair<int, int> getOption() const;
    std::vector<std::string> getValues() const;

    void updateState(sf::RenderWindow* window);
    bool handleEvent(sf::RenderWindow* window, sf::Event event);

private:
    AssetsHolder* mAssets;
    std::vector<std::string> mOptionList;
    int mOptionIndex;
    std::vector<std::vector<std::string>> mSuboptionList;
    std::vector<int> mSuboptionIndex;
    std::vector<std::vector<std::function<bool()>>> mSuboptionConditionList;
    std::vector<std::vector<std::vector<InputBox>>> mInputBoxList;
    Button mConfirmButton;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // OPTION_BOX_HPP