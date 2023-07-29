#include "ControlBox.hpp"

ControlBox::ControlBox() {
}

ControlBox::ControlBox(AssetsHolder* assets) {
    mAssets = assets;
    
    mFrontButton = Button(assets, AssetsData::Image::controlButtons, ControlBoxData::controlBoxPosition + ControlBoxData::frontButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mFrontButton.setImageRect(ControlBoxData::frontButtonTextureRect);

    mPrevButton = Button(assets, AssetsData::Image::controlButtons, ControlBoxData::controlBoxPosition + ControlBoxData::prevButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mPrevButton.setImageRect(ControlBoxData::prevButtonTextureRect);

    mNextButton = Button(assets, AssetsData::Image::controlButtons, ControlBoxData::controlBoxPosition + ControlBoxData::nextButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mNextButton.setImageRect(ControlBoxData::nextButtonTextureRect);
    
    mBackButton = Button(assets, AssetsData::Image::controlButtons, ControlBoxData::controlBoxPosition + ControlBoxData::backButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mBackButton.setImageRect(ControlBoxData::backButtonTextureRect);
}

void ControlBox::updateState(sf::RenderWindow* window) {
    mFrontButton.updateState(window);
    mNextButton.updateState(window);
    mPrevButton.updateState(window);
    mBackButton.updateState(window);
}

bool ControlBox::handleEvent(sf::RenderWindow* window, sf::Event event) {
    if (mFrontButton.handleEvent(window, event)) {
        mButton = ControlBoxData::ButtonType::front;
        return true;
    } else if (mNextButton.handleEvent(window, event)) {
        mButton = ControlBoxData::ButtonType::next;
        return true;
    } else if (mPrevButton.handleEvent(window, event)) {
        mButton = ControlBoxData::ButtonType::prev;
        return true;
    } else if (mBackButton.handleEvent(window, event)) {
        mButton = ControlBoxData::ButtonType::back;
        return true;
    }

    return false;
}

void ControlBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Sprite controlBoxSprite(*mAssets->get(AssetsData::Image::controlBox));
    controlBoxSprite.setColor(*mAssets->get(AssetsData::Color::boxComponent));
    controlBoxSprite.setPosition(ControlBoxData::controlBoxPosition);
    target.draw(controlBoxSprite, states);

    target.draw(mFrontButton, states);
    target.draw(mPrevButton, states);
    target.draw(mNextButton, states);
    target.draw(mBackButton, states);
}