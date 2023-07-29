#ifndef CONTROL_BOX_HPP
#define CONTROL_BOX_HPP

#include "../../Global.hpp"
#include "../AssetsHolder.hpp"
#include "Button.hpp"

namespace ControlBoxData {
    const float tableWidth = 880.f;
    const float tableHeight = 50.f;
    const sf::Vector2f controlBoxPosition = sf::Vector2f(10.f, 610.f);

    const sf::FloatRect frontButtonTextureRect = sf::FloatRect(0.f, 0.f, 20.f, 30.f);
    const sf::FloatRect prevButtonTextureRect = sf::FloatRect(20.f, 0.f, 20.f, 30.f);
    const sf::FloatRect nextButtonTextureRect = sf::FloatRect(40.f, 0.f, 20.f, 30.f);
    const sf::FloatRect backButtonTextureRect = sf::FloatRect(60.f, 0.f, 20.f, 30.f);

    const sf::FloatRect frontButtonRect = sf::FloatRect(30.f, 10.f, 20.f, 30.f);
    const sf::FloatRect prevButtonRect = sf::FloatRect(90.f, 10.f, 20.f, 30.f);
    const sf::FloatRect nextButtonRect = sf::FloatRect(210.f, 10.f, 20.f, 30.f);
    const sf::FloatRect backButtonRect = sf::FloatRect(270.f, 10.f, 20.f, 30.f);

    enum ButtonType {
        front,
        prev,
        next,
        back,
    };
}

class ControlBox : public sf::Drawable {
public:
    ControlBox();
    ControlBox(AssetsHolder* assets);

    ControlBoxData::ButtonType getButton() const;

    void updateState(sf::RenderWindow* window);
    bool handleEvent(sf::RenderWindow* window, sf::Event event);

private:
    AssetsHolder* mAssets;
    Button mFrontButton;
    Button mPrevButton;
    Button mNextButton;
    Button mBackButton;
    ControlBoxData::ButtonType mButton;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // CONTROL_BOX_HPP