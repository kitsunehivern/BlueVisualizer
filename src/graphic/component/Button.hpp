#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "../../Global.hpp"
#include "../AssetsHolder.hpp"

namespace ButtonData {
    enum State {
        normal,
        focused,
        invalid,
        disabled,
    };

    const float characterSize = 25.f;
}

struct ButtonColor {
public:
    typedef std::pair<AssetsData::Color, AssetsData::Color> Pair;

    ButtonColor();
    ButtonColor(Pair normal, Pair focused, Pair invalid, Pair disabled);

    void set(ButtonData::State state, Pair color);
    Pair get(ButtonData::State state);
    const Pair get(ButtonData::State state) const;

private:
    std::map<ButtonData::State, Pair> mColorMap;
};

namespace ButtonData {
    namespace ColorSet {
        const ButtonColor set1(
            std::make_pair(AssetsData::Color::box, AssetsData::Color::boxText),
            std::make_pair(AssetsData::Color::boxFocus, AssetsData::Color::boxTextFocus),
            std::make_pair(AssetsData::Color::none, AssetsData::Color::none),
            std::make_pair(AssetsData::Color::boxDisabled, AssetsData::Color::boxText)
        );

        const ButtonColor set2(
            std::make_pair(AssetsData::Color::boxActive, AssetsData::Color::boxText),
            std::make_pair(AssetsData::Color::boxFocus, AssetsData::Color::boxTextFocus),
            std::make_pair(AssetsData::Color::boxInvalid, AssetsData::Color::boxText),
            std::make_pair(AssetsData::Color::boxDisabled, AssetsData::Color::boxText)
        );
    }
} 

class Button : public sf::Drawable {
public:
    Button();
    Button(AssetsHolder* assets, AssetsData::Image image, sf::Vector2f position, ButtonColor color);

    void setImageRect(sf::FloatRect rect);
    void setDisabled(bool isDisabled);
    void setInvalid(bool isInvalid);
    void setText(std::string text);

    void updateState(sf::RenderWindow* window);
    bool handleEvent(sf::RenderWindow* window, sf::Event event);

private:
    AssetsHolder* mAssets;
    AssetsData::Image mImage;
    sf::FloatRect mImageRect;
    sf::Vector2f mPosition;
    ButtonColor mColor;
    bool mIsDisabled;
    bool mIsInvalid;
    ButtonData::State mState;
    std::string mText;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // BUTTON_HPP