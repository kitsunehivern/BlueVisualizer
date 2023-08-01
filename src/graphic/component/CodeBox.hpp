#ifndef CODE_BOX_HPP
#define CODE_BOX_HPP

#include "../../Global.hpp"
#include "../Animation.hpp"

namespace CodeBoxData {
    const sf::Vector2f position = sf::Vector2f(910.f, 610.f);
    const float lineHeight = 40.f;
    const float characterSize= 25.f;
    const sf::Vector2f codePosition = sf::Vector2f(15.f, 4.f);
}

class CodeBox {
public:
    CodeBox();

    void update(const std::vector<std::string>& codes);

    void draw(sf::RenderWindow* window, sf::Texture* boxTexture, sf::Color* boxColor, sf::Font* codeFont, sf::Color* codeColor, float ratioTime, bool fakeDraw);
    void drawFadeIn(sf::RenderWindow* window, sf::Texture* boxTexture, sf::Color* boxColor, sf::Texture* barTexture, sf::Color* barColor, sf::Font* codeFont, sf::Color* codeColor, int focusLine, float ratioTime, bool fakeDraw);
    void drawFadeOut(sf::RenderWindow* window, sf::Texture* boxTexture, sf::Color* boxColor, sf::Texture* barTexture, sf::Color* barColor, sf::Font* codeFont, sf::Color* codeColor, int focusLine, float ratioTime, bool fakeDraw);
    void drawChangeLine(sf::RenderWindow* window, sf::Texture* boxTexture, sf::Color* boxColor, sf::Texture* barTexture, sf::Color* barColor, sf::Font* codeFont, sf::Color* codeColor, int oldFocusLine, int newFocusLine, float ratioTime, bool fakeDraw);

private:
    std::vector<std::string> mCodes;
};

#endif // CODE_BOX_HPP