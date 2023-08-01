#include "Button.hpp"

ButtonColor::ButtonColor() {
}

ButtonColor::ButtonColor(Pair normal, Pair focused, Pair invalid, Pair disabled) {
    set(ButtonData::State::normal, normal);
    set(ButtonData::State::focused, focused);
    set(ButtonData::State::invalid, invalid);
    set(ButtonData::State::disabled, disabled);
}

void ButtonColor::set(ButtonData::State state, Pair color) {
    mColorMap[state] = color;
}

ButtonColor::Pair ButtonColor::get(ButtonData::State state) {
    return mColorMap[state];
}

const ButtonColor::Pair ButtonColor::get(ButtonData::State state) const {
    return mColorMap.at(state);
}

Button::Button() {
}

Button::Button(AssetsHolder* assets, AssetsData::Image image, sf::Vector2f position, ButtonColor color) {
    mAssets = assets;
    mImage = image;
    mImageRect = sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(mAssets->get(mImage)->getSize().x, mAssets->get(mImage)->getSize().y));
    mPosition = position;
    mColor = color;
    mIsDisabled = false;
    mIsInvalid = false;
    mState = ButtonData::State::normal;
    mText = "";
}

void Button::setImageRect(sf::FloatRect rect) {
    mImageRect = rect;
}

void Button::setDisabled(bool isDisabled) {
    mIsDisabled = isDisabled;
}

void Button::setInvalid(bool isInvalid) {
    mIsInvalid = isInvalid;
}

void Button::setText(std::string text) {
    mText = text;
}

void Button::updateState(sf::RenderWindow* window) {
    if (mIsDisabled) {
       mState = ButtonData::State::disabled;
    } else if (mIsInvalid) {
        mState = ButtonData::State::invalid;
    } else if (sfhelper::isMouseOver(window, mPosition, mImageRect.getSize())) {
       mState = ButtonData::State::focused;
    } else {
        mState = ButtonData::State::normal;
    }
}

bool Button::handleEvent(sf::RenderWindow* window, sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (sfhelper::isMouseOver(window, mPosition, mImageRect.getSize())) {
                return mState == ButtonData::State::focused;
            }
        }
    }

    return false;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    assert(mAssets != nullptr);

    sf::Sprite sprite(*mAssets->get(mImage));
    sprite.setTextureRect(sfhelper::toIntRect(mImageRect));
    sprite.setColor(*mAssets->get(mColor.get(mState).first));
    sprite.setPosition(mPosition);
    target.draw(sprite, states);

    sf::Text text(mText, *mAssets->get(AssetsData::Font::consolasBold), ButtonData::characterSize);
    text.setFillColor(*mAssets->get(mColor.get(mState).second));
    text.setOrigin(sfhelper::getCenterPosition(text.getLocalBounds()));
    text.setPosition(sfhelper::getCenterPosition(mPosition, mImageRect.getSize()));
    target.draw(text, states);
}