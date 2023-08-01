#include "InputBox.hpp"

InputBox::InputBox() {
}

InputBox::InputBox(InputBoxData::InputType type, AssetsHolder* assets, sf::Vector2f position, std::string name) {
    mType = type;
    
    mAssets = assets;
    mPosition = position;
    mName = name;
    mValue = "";
    mValidCharacters = std::set<char>();
    mValidator = [](std::string value, std::string name) -> std::string {
        std::swap(value, name); // no warning hehe
        return "";
    };  

    mRandomizer = []() -> std::string {
        return "";
    };

    mIsInputBoxFocused = false;
    mIsRandomBoxFocused = false;
    mIsInputBoxSelected = false;

    mIsFileBoxFocused = false;
}

void InputBox::setPosition(sf::Vector2f position) {
    mPosition = position;
}

void InputBox::setValidCharacters(std::set<char> validCharacters) {
    mValidCharacters.insert(validCharacters.begin(), validCharacters.end());
}

void InputBox::setValidator(std::function<std::string(std::string, std::string)> validator) {
    mValidator = validator;
}

void InputBox::setRandomizer(std::function<std::string()> randomizer) {
    mRandomizer = randomizer;
}

std::string InputBox::getError() const {
    return mValidator(mValue, mName);
}

std::string InputBox::getValue() const {
    return mValue;
}

void InputBox::updateState(sf::RenderWindow* window) {
    if (mType == InputBoxData::keyboard) {
        if (sfhelper::isMouseOver(window, mPosition, sf::Vector2f(InputBoxData::inputBoxWidth, InputBoxData::inputBoxHeight))) {
            mIsInputBoxFocused = true;
            mIsRandomBoxFocused = false;
        } else if (sfhelper::isMouseOver(window, sf::Vector2f(mPosition.x + InputBoxData::inputBoxWidth, mPosition.y), sf::Vector2f(InputBoxData::randomBoxWidth, InputBoxData::randomBoxHeight))) {
            mIsInputBoxFocused = false;
            mIsRandomBoxFocused = true;
        } else {
            mIsInputBoxFocused = false;
            mIsRandomBoxFocused = false;
        }

        if (mSelectedClock.getElapsedTime().asSeconds() > 1.f) {
            mSelectedClock.restart();
        }
    } else { // file
        if (sfhelper::isMouseOver(window, mPosition, sf::Vector2f(InputBoxData::fileBoxWidth, InputBoxData::fileBoxHeight))) {
            mIsFileBoxFocused = true;
        } else {
            mIsFileBoxFocused = false;
        }
    }
}

void InputBox::insertCharacter(char character) {
    if (mValue.size() < InputBoxData::maxCharacter && character != '\0' && (mValidCharacters.empty() || mValidCharacters.find(character) != mValidCharacters.end())) {
        mValue.push_back(character);
    }
}

void InputBox::deleteCharacter() {
    if (!mValue.empty()) {
        mValue.pop_back();
    }
}

HWND hwnd;
TCHAR szFileName[MAX_PATH];

void InputBox::handleEvent(sf::RenderWindow* window, sf::Event event) {
    if (mType == InputBoxData::keyboard) {
        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (sfhelper::isMouseOver(window, mPosition, sf::Vector2f(InputBoxData::inputBoxWidth, InputBoxData::inputBoxHeight))) {
                    mIsInputBoxSelected = true;
                    mSelectedClock.restart();
                } else {
                    mIsInputBoxSelected = false;
                }
                
                if (sfhelper::isMouseOver(window, sf::Vector2f(mPosition.x + InputBoxData::inputBoxWidth, mPosition.y), sf::Vector2f(InputBoxData::randomBoxWidth, InputBoxData::randomBoxHeight))) {
                    try {
                        mValue = mRandomizer();
                    } catch (std::exception& e) {
                        mValue.clear();
                    }
                }
            }
        } else if (event.type == sf::Event::KeyPressed) {
            if (mIsInputBoxSelected) {
                mSelectedClock.restart();
                if (event.key.code == sf::Keyboard::BackSpace) {
                    deleteCharacter();
                } else {
                    insertCharacter(sfhelper::keyCodeToChar(event.key.code));
                }
            }
        }
    } else { // file
        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (sfhelper::isMouseOver(window, mPosition, sf::Vector2f(InputBoxData::fileBoxWidth, InputBoxData::fileBoxHeight))) {
                    OPENFILENAME ofn;
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0";
                    ofn.lpstrFile = szFileName;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
                    ofn.lpstrDefExt = L"txt";

                    if (GetOpenFileName(&ofn) == TRUE) {
                        std::ifstream fin(ofn.lpstrFile);
                        std::string line, text;
                        while (std::getline(fin, line)) {
                            text += line;
                        }

                        for (auto character : text) {
                            insertCharacter(character);
                        }
                    }
                }
            }
        }
    }
}

void InputBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (mType == InputBoxData::keyboard) {
        sf::Sprite inputBoxSprite(*mAssets->get(AssetsData::Image::inputBox));
        if (mIsInputBoxFocused) {
            inputBoxSprite.setColor(*mAssets->get(AssetsData::Color::boxFocus));
        } else {
            inputBoxSprite.setColor(*mAssets->get(AssetsData::Color::box));
        }

        inputBoxSprite.setPosition(mPosition);
        target.draw(inputBoxSprite, states);

        std::string nameAndValue = mName + " = " + mValue;
        sf::Text inputBoxText(nameAndValue, *mAssets->get(AssetsData::Font::consolasBold), InputBoxData::characterSize);
        if (mIsInputBoxFocused) {
            inputBoxText.setFillColor(*mAssets->get(AssetsData::Color::boxTextFocus));
        } else {
            inputBoxText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
        }
        
        if (inputBoxText.getLocalBounds().width > InputBoxData::inputBoxWidth - 2 * InputBoxData::textPosition.x) {
            int textSize = -1;
            for (int low = 1, high = nameAndValue.size(); low <= high; ) {
                int middle = (low + high) / 2;

                inputBoxText.setString(nameAndValue.substr(nameAndValue.size() - middle, middle));
                if (inputBoxText.getLocalBounds().width <= InputBoxData::inputBoxWidth - 2 * InputBoxData::textPosition.x) {
                    textSize = middle;
                    low = middle + 1;
                } else {
                    high = middle - 1;
                }
            }

            inputBoxText.setString(nameAndValue.substr(nameAndValue.size() - textSize, textSize));
            inputBoxText.setOrigin(inputBoxText.getLocalBounds().left + inputBoxText.getLocalBounds().width, 0);
            inputBoxText.setPosition(mPosition.x + InputBoxData::inputBoxWidth - InputBoxData::textPosition.x, mPosition.y + InputBoxData::textPosition.y);
        } else {
            inputBoxText.setPosition(mPosition + InputBoxData::textPosition);
        }

        target.draw(inputBoxText, states);

        sf::Sprite randomBoxSprite(*mAssets->get(AssetsData::Image::randomBox));
        if (mIsRandomBoxFocused) {
            randomBoxSprite.setColor(*mAssets->get(AssetsData::Color::boxFocus));
        } else {
            randomBoxSprite.setColor(*mAssets->get(AssetsData::Color::box));
        }

        randomBoxSprite.setPosition(mPosition.x + InputBoxData::inputBoxWidth, mPosition.y);
        target.draw(randomBoxSprite, states);

        sf::Sprite randomIconSprite(*mAssets->get(AssetsData::Image::randomIcon));
        if (mIsRandomBoxFocused) {
            randomIconSprite.setColor(*mAssets->get(AssetsData::Color::boxTextFocus));
        } else {
            randomIconSprite.setColor(*mAssets->get(AssetsData::Color::boxText));
        }

        randomIconSprite.setPosition(mPosition.x + InputBoxData::inputBoxWidth, mPosition.y);
        target.draw(randomIconSprite, states);

		if (!mIsInputBoxSelected || mSelectedClock.getElapsedTime().asSeconds() > 0.5f) {
			return;
		}

		sf::RectangleShape insertionPoint(sf::Vector2f(2, InputBoxData::inputBoxHeight - 2 * InputBoxData::textPosition.y));
		if (mIsInputBoxFocused) {
			insertionPoint.setFillColor(*mAssets->get(AssetsData::Color::boxTextFocus));
		} else {
			insertionPoint.setFillColor(*mAssets->get(AssetsData::Color::boxText));
		}

		insertionPoint.setOrigin(0, insertionPoint.getLocalBounds().top + insertionPoint.getLocalBounds().height / 2);
		insertionPoint.setPosition(inputBoxText.getGlobalBounds().left + inputBoxText.getGlobalBounds().width + 2, mPosition.y + InputBoxData::inputBoxHeight / 2);
        target.draw(insertionPoint, states);
    } else { // file
        sf::Sprite fileBoxSprite(*mAssets->get(AssetsData::Image::fileBox));
        if (mIsFileBoxFocused) {
            fileBoxSprite.setColor(*mAssets->get(AssetsData::Color::boxFocus));
        } else {
            fileBoxSprite.setColor(*mAssets->get(AssetsData::Color::box));
        }

        fileBoxSprite.setPosition(mPosition);
        target.draw(fileBoxSprite, states);

        sf::Text fileBoxText("Load from file", *mAssets->get(AssetsData::Font::consolasBold), InputBoxData::characterSize);
        if (mIsFileBoxFocused) {
            fileBoxText.setFillColor(*mAssets->get(AssetsData::Color::boxTextFocus));
        } else {
            fileBoxText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
        }

        fileBoxText.setOrigin(sfhelper::getCenterPosition(fileBoxText.getLocalBounds()));
        fileBoxText.setPosition(sfhelper::getCenterPosition(mPosition, sf::Vector2f(InputBoxData::fileBoxWidth, InputBoxData::fileBoxHeight)));
        target.draw(fileBoxText, states);
    }
}