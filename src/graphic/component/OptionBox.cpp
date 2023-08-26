#include "OptionBox.hpp"

OptionBox::OptionBox() {
}

OptionBox::OptionBox(AssetsHolder* assets) {
    mAssets = assets;
    mOptionIndex = 0;
    mOptionPrevButton = Button(mAssets, AssetsData::Image::optionPrevButton, OptionBoxData::Option::tablePosition + OptionBoxData::Option::prevButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mOptionNextButton = Button(mAssets, AssetsData::Image::optionNextButton, OptionBoxData::Option::tablePosition + OptionBoxData::Option::nextButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mSuboptionPrevButton = Button(mAssets, AssetsData::Image::suboptionPrevButton, OptionBoxData::Suboption::tablePosition + OptionBoxData::Suboption::prevButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mSuboptionNextButton = Button(mAssets, AssetsData::Image::suboptionNextButton, OptionBoxData::Suboption::tablePosition + OptionBoxData::Suboption::nextButtonRect.getPosition(), ButtonData::ColorSet::set1);
    mConfirmButton = Button(mAssets, AssetsData::Image::confirmBox, OptionBoxData::Input::tablePosition + OptionBoxData::Input::confirmBoxPosition,  ButtonData::ColorSet::set2);
}

void OptionBox::addOption(const std::string& option) {
    mOptionList.push_back(option);
    mSuboptionList.push_back(std::vector<std::string>());
    mSuboptionIndex.push_back(0);
    mSuboptionConditionList.push_back(std::vector<std::function<bool()>>());
    mInputBoxList.push_back(std::vector<std::vector<InputBox>>());
}

void OptionBox::addSuboption(const std::string& suboption, std::function<bool()> condition) {
    mSuboptionList.back().push_back(suboption);
    mSuboptionConditionList.back().push_back(condition);
    mInputBoxList.back().push_back(std::vector<InputBox>());
}

void OptionBox::addSuboptionInputBox(std::string name, std::function<std::string(std::string, std::string)> validator, std::function<std::string()> randomizer) {
    mInputBoxList.back().back().push_back(InputBox(InputBoxData::InputType::keyboard, mAssets, sf::Vector2f(0.f, 0.f), name));
    mInputBoxList.back().back().back().setValidator(validator);
    mInputBoxList.back().back().back().setRandomizer(randomizer);
}

void OptionBox::addSuboptionFileBox(std::string name, std::function<std::string(std::string, std::string)> validator) {
    mInputBoxList.back().back().push_back(InputBox(InputBoxData::InputType::file, mAssets, sf::Vector2f(0.f, 0.f), name));
    mInputBoxList.back().back().back().setValidator(validator);
}

void OptionBox::addSuboptionEditorBox(std::function<std::string(std::string, std::string)> validator) {
    mInputBoxList.back().back().push_back(InputBox(InputBoxData::InputType::editor, mAssets, sf::Vector2f(0.f, 0.f), ""));
    mInputBoxList.back().back().back().setValidator(validator);
}

void OptionBox::processOption() {
    for (int i = 0; i < (int)mOptionList.size(); i++) {
        for (int j = 0; j < (int)mSuboptionList[i].size(); j++) {
            if (mInputBoxList[i][j].size() == 0) {

            } else if (mInputBoxList[i][j].size() == 1) {
                mInputBoxList[i][j][0].setPosition(OptionBoxData::Input::tablePosition + OptionBoxData::Input::boxPosition);
            } else if (mInputBoxList[i][j].size() == 2) {
                mInputBoxList[i][j][0].setPosition(OptionBoxData::Input::tablePosition + OptionBoxData::Input::box1Position);
                mInputBoxList[i][j][1].setPosition(OptionBoxData::Input::tablePosition + OptionBoxData::Input::box2Position);
            } else {
                assert(false);
            }
        }
    }
}

bool OptionBox::isAnyInputBoxSelected() const {
    for (int i = 0; i < (int)mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]].size(); i++) {
        if (mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]][i].isInputBoxSelected()) {
            return true;
        }
    }

    return false;
}

std::pair<int, int> OptionBox::getOption() const {
    return std::make_pair(mOptionIndex, mSuboptionIndex[mOptionIndex]);
}

std::vector<std::string> OptionBox::getValues() const {
    std::vector<std::string> values;
    for (int i = 0; i < (int)mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]].size(); i++) {
        values.push_back(mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]][i].getValue());
    }

    return values;
}

std::string OptionBox::getValue(int optionIndex, int suboptionIndex, int inputBoxIndex) const {
    return mInputBoxList[optionIndex][suboptionIndex][inputBoxIndex].getValue();
}

void OptionBox::updateState(sf::RenderWindow* window) {
    mError.clear();
    if (!mSuboptionConditionList[mOptionIndex][mSuboptionIndex[mOptionIndex]]()) {
        mError = "Unavailable";
        mConfirmButton.setDisabled(true);
    } else {
        mConfirmButton.setDisabled(false);
    }

    for (int i = 0; i < (int)mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]].size(); i++) {
        mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]][i].updateState(window);
        if (mError.empty()) {
            mError = mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]][i].getError();
        }
    }

    if (mOptionIndex == 0) {
        mOptionPrevButton.setDisabled(true);
    } else {
        mOptionPrevButton.setDisabled(false);
    }

    mOptionPrevButton.updateState(window);

    if (mOptionIndex == (int)mOptionList.size() - 1) {
        mOptionNextButton.setDisabled(true);
    } else {
        mOptionNextButton.setDisabled(false);
    }

    mOptionNextButton.updateState(window);

    if (mSuboptionIndex[mOptionIndex] == 0) {
        mSuboptionPrevButton.setDisabled(true);
    } else {
        mSuboptionPrevButton.setDisabled(false);
    }

    mSuboptionPrevButton.updateState(window);

    if (mSuboptionIndex[mOptionIndex] == (int)mSuboptionList[mOptionIndex].size() - 1) {
        mSuboptionNextButton.setDisabled(true);
    } else {
        mSuboptionNextButton.setDisabled(false);
    }

    mSuboptionNextButton.updateState(window);

    if (!mError.empty()) {
        mConfirmButton.setInvalid(true);
    } else {
        mConfirmButton.setInvalid(false);
    }

    mConfirmButton.setTextInside(mError.empty() ? "Confirm" : mError);
    mConfirmButton.updateState(window);
}

bool OptionBox::handleEvent(sf::RenderWindow* window, sf::Event event) {
    for (int i = 0; i < (int)mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]].size(); i++) {
        mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]][i].handleEvent(window, event);
    }

    if (mOptionPrevButton.handleEvent(window, event)) {
        mOptionIndex = std::max(mOptionIndex - 1, 0);
    } else if (mOptionNextButton.handleEvent(window, event)) {
        mOptionIndex = std::min(mOptionIndex + 1, (int)mOptionList.size() - 1);
    } else if (mSuboptionPrevButton.handleEvent(window, event)) {
        mSuboptionIndex[mOptionIndex] = std::max(mSuboptionIndex[mOptionIndex] - 1, 0);
    } else if (mSuboptionNextButton.handleEvent(window, event)) {
        mSuboptionIndex[mOptionIndex] = std::min(mSuboptionIndex[mOptionIndex] + 1, (int)mSuboptionList[mOptionIndex].size() - 1);
    }

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            if (mConfirmButton.getState() == ButtonData::State::normal || mConfirmButton.getState() == ButtonData::State::focused) {
                return true;
            }
        }
    }

    return mConfirmButton.handleEvent(window, event);
}

void OptionBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Sprite optionTableSprite(*mAssets->get(AssetsData::Image::optionTable));
    optionTableSprite.setColor(*mAssets->get(AssetsData::Color::boxComponent));
    optionTableSprite.setPosition(OptionBoxData::Option::tablePosition);
    target.draw(optionTableSprite, states);

    sf::Text optionPrevText("", *mAssets->get(AssetsData::Font::consolasBold), OptionBoxData::characterSize);
    if (mOptionIndex > 0) {
        optionPrevText.setString(mOptionList[mOptionIndex - 1]);
    }

    target.draw(mOptionPrevButton, states);

    optionPrevText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
    optionPrevText.setOrigin(sfhelper::getCenterPosition(optionPrevText.getLocalBounds()));
    optionPrevText.setPosition(OptionBoxData::Option::tablePosition + sfhelper::getCenterPosition(OptionBoxData::Option::prevRect));
    target.draw(optionPrevText, states);

    sf::Text optionCurrText(mOptionList[mOptionIndex], *mAssets->get(AssetsData::Font::consolasBold), OptionBoxData::characterSize);
    optionCurrText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
    optionCurrText.setOrigin(sfhelper::getCenterPosition(optionCurrText.getLocalBounds()));
    optionCurrText.setPosition(OptionBoxData::Option::tablePosition + sfhelper::getCenterPosition(OptionBoxData::Option::currRect));
    target.draw(optionCurrText, states);

    sf::Text optionNextText("", *mAssets->get(AssetsData::Font::consolasBold), OptionBoxData::characterSize);
    if (mOptionIndex < (int)mOptionList.size() - 1) {
        optionNextText.setString(mOptionList[mOptionIndex + 1]);
    }

    optionNextText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
    optionNextText.setOrigin(sfhelper::getCenterPosition(optionNextText.getLocalBounds()));
    optionNextText.setPosition(OptionBoxData::Option::tablePosition + sfhelper::getCenterPosition(OptionBoxData::Option::nextRect));
    target.draw(optionNextText, states);

    target.draw(mOptionNextButton, states);

    sf::Sprite suboptionTableSprite(*mAssets->get(AssetsData::Image::suboptionTable));
    suboptionTableSprite.setColor(*mAssets->get(AssetsData::Color::boxComponent));
    suboptionTableSprite.setPosition(OptionBoxData::Suboption::tablePosition);
    target.draw(suboptionTableSprite, states);

    target.draw(mSuboptionPrevButton, states);

    sf::Text suboptionPrevText("", *mAssets->get(AssetsData::Font::consolasBold), OptionBoxData::characterSize);
    if (mSuboptionIndex[mOptionIndex] > 0) {
        suboptionPrevText.setString(mSuboptionList[mOptionIndex][mSuboptionIndex[mOptionIndex] - 1]);
    }

    suboptionPrevText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
    suboptionPrevText.setOrigin(sfhelper::getCenterPosition(suboptionPrevText.getLocalBounds()));
    suboptionPrevText.setPosition(OptionBoxData::Suboption::tablePosition + sfhelper::getCenterPosition(OptionBoxData::Suboption::prevRect));
    target.draw(suboptionPrevText, states);

    sf::Text suboptionCurrText(mSuboptionList[mOptionIndex][mSuboptionIndex[mOptionIndex]], *mAssets->get(AssetsData::Font::consolasBold), OptionBoxData::characterSize);
    suboptionCurrText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
    suboptionCurrText.setOrigin(sfhelper::getCenterPosition(suboptionCurrText.getLocalBounds()));
    suboptionCurrText.setPosition(OptionBoxData::Suboption::tablePosition + sfhelper::getCenterPosition(OptionBoxData::Suboption::currRect));
    target.draw(suboptionCurrText, states);

    sf::Text suboptionNextText("", *mAssets->get(AssetsData::Font::consolasBold), OptionBoxData::characterSize);
    if (mSuboptionIndex[mOptionIndex] < (int)mSuboptionList[mOptionIndex].size() - 1) {
        suboptionNextText.setString(mSuboptionList[mOptionIndex][mSuboptionIndex[mOptionIndex] + 1]);
    }

    suboptionNextText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
    suboptionNextText.setOrigin(sfhelper::getCenterPosition(suboptionNextText.getLocalBounds()));
    suboptionNextText.setPosition(OptionBoxData::Suboption::tablePosition + sfhelper::getCenterPosition(OptionBoxData::Suboption::nextRect));
    target.draw(suboptionNextText, states);

    target.draw(mSuboptionNextButton, states);

    sf::Sprite inputTableSprite(*mAssets->get(AssetsData::Image::inputTable));
    inputTableSprite.setColor(*mAssets->get(AssetsData::Color::boxComponent));
    inputTableSprite.setPosition(OptionBoxData::Input::tablePosition);
    target.draw(inputTableSprite, states);

    for (int i = 0; i < (int)mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]].size(); i++) {
        target.draw(mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]][i], states);
    }

    target.draw(mConfirmButton, states);
}