#include "CodeBox.hpp"

CodeBox::CodeBox() {
    mCodes = std::vector<std::string>();
}

void CodeBox::update(const std::vector<std::string>& codes) {
    mCodes = codes;
}

void CodeBox::draw(sf::RenderWindow* window, sf::Texture* boxTexture, sf::Color* boxColor, sf::Font* codeFont, sf::Color* codeColor, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    ratioTime = Animation::Bezier(ratioTime);

    sf::Sprite boxSprite(*boxTexture);
    boxSprite.setColor(*boxColor);
    boxSprite.setPosition(CodeBoxData::position);
    window->draw(boxSprite);

    for (int line = 0; line < (int)mCodes.size(); line++) {
        sf::Text codeText(mCodes[line], *codeFont, CodeBoxData::characterSize);
        codeText.setFillColor(*codeColor);
        codeText.setPosition(CodeBoxData::position + CodeBoxData::codePosition + sf::Vector2f(0.f, CodeBoxData::lineHeight * line));
        window->draw(codeText);
    }
}

void CodeBox::drawFadeIn(sf::RenderWindow* window, sf::Texture* boxTexture, sf::Color* boxColor, sf::Texture* barTexture, sf::Color* barColor, sf::Font* codeFont, sf::Color* codeColor, int focusLine, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    sf::Sprite boxSprite(*boxTexture);
    boxSprite.setColor(*boxColor);
    boxSprite.setPosition(CodeBoxData::position);
    window->draw(boxSprite);

    sf::Sprite barSprite(*barTexture);
    barSprite.setColor(*barColor * sf::Color(255, 255, 255, std::round(Animation::Bezier(ratioTime) * 255)));
    barSprite.setPosition(CodeBoxData::position.x, CodeBoxData::position.y + CodeBoxData::lineHeight * focusLine);
    window->draw(barSprite);

    for (int line = 0; line < (int)mCodes.size(); line++) {
        sf::Text codeText(mCodes[line], *codeFont, CodeBoxData::characterSize);
        codeText.setFillColor(*codeColor);
        codeText.setPosition(CodeBoxData::position + CodeBoxData::codePosition + sf::Vector2f(0.f, CodeBoxData::lineHeight * line));
        window->draw(codeText);
    }
}

void CodeBox::drawFadeOut(sf::RenderWindow* window, sf::Texture* boxTexture, sf::Color* boxColor, sf::Texture* barTexture, sf::Color* barColor, sf::Font* codeFont, sf::Color* codeColor, int focusLine, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    drawFadeIn(window, boxTexture, boxColor, barTexture, barColor, codeFont, codeColor, focusLine, 1.f - ratioTime, fakeDraw);
}

void CodeBox::drawChangeLine(sf::RenderWindow* window, sf::Texture* boxTexture, sf::Color* boxColor, sf::Texture* barTexture, sf::Color* barColor, sf::Font* codeFont, sf::Color* codeColor, int oldFocusLine, int newFocusLine, float ratioTime, bool fakeDraw) {
    if (fakeDraw) {
        return;
    }

    sf::Sprite boxSprite(*boxTexture);
    boxSprite.setColor(*boxColor);
    boxSprite.setPosition(CodeBoxData::position);
    window->draw(boxSprite);

    sf::Sprite barSprite(*barTexture);
    barSprite.setColor(*barColor);
    if (ratioTime <= 0.25f) {
        barSprite.setPosition(CodeBoxData::position.x, CodeBoxData::position.y + CodeBoxData::lineHeight * oldFocusLine);
    } else if (ratioTime <= 0.75f) {
        barSprite.setPosition(CodeBoxData::position.x, CodeBoxData::position.y + CodeBoxData::lineHeight * (oldFocusLine + Animation::Bezier(2.f * (ratioTime - 0.25f)) * (newFocusLine - oldFocusLine)));
    } else {
        barSprite.setPosition(CodeBoxData::position.x, CodeBoxData::position.y + CodeBoxData::lineHeight * newFocusLine);
    }

    window->draw(barSprite);

    for (int line = 0; line < (int)mCodes.size(); line++) {
        sf::Text codeText(mCodes[line], *codeFont, CodeBoxData::characterSize);
        codeText.setFillColor(*codeColor);
        codeText.setPosition(CodeBoxData::position + CodeBoxData::codePosition + sf::Vector2f(0.f, CodeBoxData::lineHeight * line));
        window->draw(codeText);
    }
}