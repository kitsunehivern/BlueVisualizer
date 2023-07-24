#include "InputBox.hpp"

InputBox::InputBox() {
}

InputBox::InputBox(InputBoxData::InputType type, AssetsHolder* assets, sf::Vector2f position, std::string name) {
    mType = type;
    
    mAssets = assets;
    mPosition = position;
    mName = name;
    mValue = "";

    mIsInputBoxFocused = false;
    mIsRandomBoxFocused = false;
    mIsInputBoxSelected = false;

    mIsFileBoxFocused = false;
}

void InputBox::setPosition(sf::Vector2f position) {
    mPosition = position;
}

void InputBox::setValidCharacters(std::vector<char> validCharacters) {
    mValidCharacters = validCharacters;
}

void InputBox::setValidator(std::function<std::string(std::string, std::string)> validator) {
    mValidator = validator;
}

void InputBox::setRandomizer(std::function<std::string()> randomizer) {
    mRandomizer = randomizer;
}

void InputBox::handleEvent(sf::RenderWindow* window, sf::Event event) {
    if (mType == InputBoxData::keyboard) {
        if (sfhelper::isMouseOver(window, mPosition, sf::Vector2f(InputBoxData::inputBoxWidth, InputBoxData::inputBoxHeight))) {
            mIsInputBoxFocused = true;
        } else if (sfhelper::isMouseOver(window, sf::Vector2f(mPosition.x + InputBoxData::inputBoxWidth, mPosition.y), sf::Vector2f(InputBoxData::randomBoxWidth, InputBoxData::randomBoxHeight))) {
            mIsRandomBoxFocused = true;
        } else {
            mIsInputBoxFocused = false;
            mIsRandomBoxFocused = false;
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (sfhelper::isMouseOver(window, mPosition, sf::Vector2f(InputBoxData::inputBoxWidth, InputBoxData::inputBoxHeight))) {
                    mIsInputBoxSelected = true;
                    mSelectedClock.restart();
                } else {
                    mIsInputBoxSelected = false;
                }
                
                if (sfhelper::isMouseOver(window, sf::Vector2f(mPosition.x + InputBoxData::inputBoxWidth, mPosition.y), sf::Vector2f(InputBoxData::randomBoxWidth, InputBoxData::randomBoxHeight))) {
                    mValue = mRandomizer();
                }
            }
        } else if (event.type == sf::Event::KeyPressed) {
            if (mIsInputBoxSelected) {
                mSelectedClock.restart();
                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (!mValue.empty() > 0) {
                        mValue.pop_back();
                    }
                } else if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z) {
                    mValue.push_back(event.key.code - sf::Keyboard::A + 'a');
                }
            }
        }

        std::cerr << mValue << "\n";
    } else { // file

    }
}

void InputBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (mType == InputBoxData::keyboard) {
        sf::Sprite inputBoxSprite(*mAssets->get(AssetsData::ImageName::inputBox));
        if (mIsInputBoxFocused) {
            inputBoxSprite.setColor(*mAssets->get(AssetsData::ColorName::boxFocusColor));
        } else {
            inputBoxSprite.setColor(*mAssets->get(AssetsData::ColorName::boxColor));
        }

        inputBoxSprite.setPosition(mPosition);
        target.draw(inputBoxSprite, states);

        sf::Text inputBoxText(mValue, *mAssets->get(AssetsData::FontName::consolasBold), InputBoxData::characterSize);
        if (mIsInputBoxFocused) {
            inputBoxText.setFillColor(*mAssets->get(AssetsData::ColorName::boxTextFocusColor));
        } else {
            inputBoxText.setFillColor(*mAssets->get(AssetsData::ColorName::boxTextColor));
        }

        inputBoxText.setPosition(mPosition.x + InputBoxData::textPositionX, mPosition.y + InputBoxData::textPositionY);
        target.draw(inputBoxText, states);

        float currentTime = mSelectedClock.getElapsedTime().asSeconds();
		if (!mIsInputBoxSelected || currentTime - std::floor(currentTime) > 0.5f) {
			return;
		}

		sf::RectangleShape insertionPoint(sf::Vector2f(2, InputBoxData::inputBoxHeight - 2 * InputBoxData::textPositionY));
		if (mIsInputBoxFocused) {
			insertionPoint.setFillColor(*mAssets->get(AssetsData::ColorName::boxTextFocusColor));
		} else {
			insertionPoint.setFillColor(*mAssets->get(AssetsData::ColorName::boxTextColor));
		}

		insertionPoint.setOrigin(0, insertionPoint.getLocalBounds().top + insertionPoint.getLocalBounds().height / 2);
		insertionPoint.setPosition(mPosition.x + InputBoxData::textPositionX + inputBoxText.getLocalBounds().width + 2, mPosition.y + InputBoxData::inputBoxHeight / 2);
        target.draw(insertionPoint, states);
    } else { // file

    }
}