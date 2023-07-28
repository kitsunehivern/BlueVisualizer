#include "OptionBox.hpp"

OptionBox::OptionBox() {
}

OptionBox::OptionBox(AssetsHolder* assets) {
    mAssets = assets;
    mOptionIndex = 0;
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

void OptionBox::addSuboptionInputBox(InputBoxData::InputType type, std::string name) {
    mInputBoxList.back().back().push_back(InputBox(type, mAssets, sf::Vector2f(0.f, 0.f), name));
}

void OptionBox::processOption() {
    for (int i = 0; i < (int)mOptionList.size(); i++) {
        for (int j = 0; j < (int)mSuboptionList[i].size(); j++) {
            if (mInputBoxList[i][j].size() == 0) {

            } else if (mInputBoxList[i][j].size() == 1) {
                mInputBoxList[i][j][0].setPosition(OptionBoxData::optionBoxPosition + OptionBoxData::Input::tablePosition + OptionBoxData::Input::boxPosition);
            } else if (mInputBoxList[i][j].size() == 2) {
                mInputBoxList[i][j][0].setPosition(OptionBoxData::optionBoxPosition + OptionBoxData::Input::tablePosition + OptionBoxData::Input::box1Position);
                mInputBoxList[i][j][1].setPosition(OptionBoxData::optionBoxPosition + OptionBoxData::Input::tablePosition + OptionBoxData::Input::box2Position);
            } else {
                assert(false);
            }
        }
    }
}

void OptionBox::updateState(sf::RenderWindow* window) {
    for (int i = 0; i < (int)mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]].size(); i++) {
        mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]][i].updateState(window);
    }
}

std::vector<std::string> OptionBox::handleEvent(sf::RenderWindow* window, sf::Event event) {
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (sfhelper::isMouseOver(window, OptionBoxData::optionBoxPosition + OptionBoxData::Option::tablePosition + OptionBoxData::Option::prevButtonRect.getPosition(), OptionBoxData::Option::prevButtonRect.getSize())) {
                mOptionIndex = std::max(mOptionIndex - 1, 0);
            } else if (sfhelper::isMouseOver(window, OptionBoxData::optionBoxPosition + OptionBoxData::Option::tablePosition + OptionBoxData::Option::nextButtonRect.getPosition(), OptionBoxData::Option::nextButtonRect.getSize())) {
                mOptionIndex = std::min(mOptionIndex + 1, (int)mOptionList.size() - 1);
            } else if (sfhelper::isMouseOver(window, OptionBoxData::optionBoxPosition + OptionBoxData::Suboption::tablePosition + OptionBoxData::Suboption::prevButtonRect.getPosition(), OptionBoxData::Suboption::prevButtonRect.getSize())) {
                mSuboptionIndex[mOptionIndex] = std::max(mSuboptionIndex[mOptionIndex] - 1, 0);
            } else if (sfhelper::isMouseOver(window, OptionBoxData::optionBoxPosition + OptionBoxData::Suboption::tablePosition + OptionBoxData::Suboption::nextButtonRect.getPosition(), OptionBoxData::Suboption::nextButtonRect.getSize())) {
                mSuboptionIndex[mOptionIndex] = std::min(mSuboptionIndex[mOptionIndex] + 1, (int)mSuboptionList[mOptionIndex].size() - 1);
            }
        }
    }

    for (int i = 0; i < (int)mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]].size(); i++) {
        mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]][i].handleEvent(window, event);
    }

    return {};
}

void OptionBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Sprite optionTableSprite(*mAssets->get(AssetsData::Image::optionTable));
    optionTableSprite.setColor(*mAssets->get(AssetsData::Color::boxComponent));
    optionTableSprite.setPosition(OptionBoxData::optionBoxPosition + OptionBoxData::Option::tablePosition);
    target.draw(optionTableSprite, states);

    sf::Text optionPrevText("", *mAssets->get(AssetsData::Font::consolasBold), OptionBoxData::characterSize);
    if (mOptionIndex > 0) {
        optionPrevText.setString(mOptionList[mOptionIndex - 1]);
    }

    optionPrevText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
    optionPrevText.setOrigin(sfhelper::getCenterPosition(optionPrevText.getLocalBounds()));
    optionPrevText.setPosition(OptionBoxData::optionBoxPosition + OptionBoxData::Option::tablePosition + sfhelper::getCenterPosition(OptionBoxData::Option::prevRect));
    target.draw(optionPrevText, states);

    sf::Text optionCurrText(mOptionList[mOptionIndex], *mAssets->get(AssetsData::Font::consolasBold), OptionBoxData::characterSize);
    optionCurrText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
    optionCurrText.setOrigin(sfhelper::getCenterPosition(optionCurrText.getLocalBounds()));
    optionCurrText.setPosition(OptionBoxData::optionBoxPosition + OptionBoxData::Option::tablePosition + sfhelper::getCenterPosition(OptionBoxData::Option::currRect));
    target.draw(optionCurrText, states);

    sf::Text optionNextText("", *mAssets->get(AssetsData::Font::consolasBold), OptionBoxData::characterSize);
    if (mOptionIndex < (int)mOptionList.size() - 1) {
        optionNextText.setString(mOptionList[mOptionIndex + 1]);
    }

    optionNextText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
    optionNextText.setOrigin(sfhelper::getCenterPosition(optionNextText.getLocalBounds()));
    optionNextText.setPosition(OptionBoxData::optionBoxPosition + OptionBoxData::Option::tablePosition + sfhelper::getCenterPosition(OptionBoxData::Option::nextRect));
    target.draw(optionNextText, states);

    sf::Sprite suboptionTableSprite(*mAssets->get(AssetsData::Image::suboptionTable));
    suboptionTableSprite.setColor(*mAssets->get(AssetsData::Color::boxComponent));
    suboptionTableSprite.setPosition(OptionBoxData::optionBoxPosition + OptionBoxData::Suboption::tablePosition);
    target.draw(suboptionTableSprite, states);

    sf::Text suboptionPrevText("", *mAssets->get(AssetsData::Font::consolasBold), OptionBoxData::characterSize);
    if (mSuboptionIndex[mOptionIndex] > 0) {
        suboptionPrevText.setString(mSuboptionList[mOptionIndex][mSuboptionIndex[mOptionIndex] - 1]);
    }

    suboptionPrevText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
    suboptionPrevText.setOrigin(sfhelper::getCenterPosition(suboptionPrevText.getLocalBounds()));
    suboptionPrevText.setPosition(OptionBoxData::optionBoxPosition + OptionBoxData::Suboption::tablePosition + sfhelper::getCenterPosition(OptionBoxData::Suboption::prevRect));
    target.draw(suboptionPrevText, states);

    sf::Text suboptionCurrText(mSuboptionList[mOptionIndex][mSuboptionIndex[mOptionIndex]], *mAssets->get(AssetsData::Font::consolasBold), OptionBoxData::characterSize);
    suboptionCurrText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
    suboptionCurrText.setOrigin(sfhelper::getCenterPosition(suboptionCurrText.getLocalBounds()));
    suboptionCurrText.setPosition(OptionBoxData::optionBoxPosition + OptionBoxData::Suboption::tablePosition + sfhelper::getCenterPosition(OptionBoxData::Suboption::currRect));
    target.draw(suboptionCurrText, states);

    sf::Text suboptionNextText("", *mAssets->get(AssetsData::Font::consolasBold), OptionBoxData::characterSize);
    if (mSuboptionIndex[mOptionIndex] < (int)mSuboptionList[mOptionIndex].size() - 1) {
        suboptionNextText.setString(mSuboptionList[mOptionIndex][mSuboptionIndex[mOptionIndex] + 1]);
    }

    suboptionNextText.setFillColor(*mAssets->get(AssetsData::Color::boxText));
    suboptionNextText.setOrigin(sfhelper::getCenterPosition(suboptionNextText.getLocalBounds()));
    suboptionNextText.setPosition(OptionBoxData::optionBoxPosition + OptionBoxData::Suboption::tablePosition + sfhelper::getCenterPosition(OptionBoxData::Suboption::nextRect));
    target.draw(suboptionNextText, states);

    sf::Sprite inputTableSprite(*mAssets->get(AssetsData::Image::inputTable));
    inputTableSprite.setColor(*mAssets->get(AssetsData::Color::boxComponent));
    inputTableSprite.setPosition(OptionBoxData::optionBoxPosition + OptionBoxData::Input::tablePosition);
    target.draw(inputTableSprite, states);

    for (int i = 0; i < (int)mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]].size(); i++) {
        target.draw(mInputBoxList[mOptionIndex][mSuboptionIndex[mOptionIndex]][i], states);
    }
}