#ifndef ASSETS_HOLDER_HPP
#define ASSETS_HOLDER_HPP

#include "../Global.hpp"

namespace AssetsData {
    enum Image {
        lightBackground,
        darkBackground,
        lightLogo,
        darkLogo,

        hollowCircle,
        filledCircle,
        hollowSquare,
        filledSquare,
        stick,

        inputBox,
        randomBox,
        fileBox,
        randomIcon,
        optionTable,
        optionPrevButton,
        optionNextButton,
        suboptionTable,
        suboptionPrevButton,
        suboptionNextButton,
        inputTable,
        confirmBox,
        controlBox,
        controlButtons,
        statusButtons,
        videoBar,
        codeBox,
        codeBar,
        speedButton,
        speed,
        visualizeBox,
        quitButton,
        themeBox,
        themeButton,
        titleBox,
    };

    enum Font {
        consolasBold,
        hyperspaceItalicBold,
    };

    enum Color {
        none,

        node,
        nodeFocus1, // Foucs 1
        nodeFocus2, // Focus 2
        nodeFocus3, // Inserted
        nodeFocus4, // Erased
        nodeText,
        nodeTextFocus1, // Focus 1
        nodeTextFocus2, // Focus 2
        nodeTextFocus3, // Focus 3

        edge,
        edgeFocus,

        boxBackground,
        boxComponent,
        box,
        boxFocus,
        boxActive,
        boxInvalid,
        boxDisabled,
        boxText,
        boxTextFocus,

        label,
    };

    enum Theme {
        light,
        dark,
    };
}

class AssetsHolder {
public:
    AssetsHolder();
    ~AssetsHolder();

    void load(AssetsData::Image name, const std::string& filename);
    sf::Texture* get(AssetsData::Image name);
    const sf::Texture* get(AssetsData::Image name) const;

    void load(AssetsData::Font name, const std::string& filename);
    sf::Font* get(AssetsData::Font name);
    const sf::Font* get(AssetsData::Font name) const;

    void setLight(AssetsData::Color name, sf::Color color);
    void setDark(AssetsData::Color name, sf::Color color);
    sf::Color* get(AssetsData::Color name);
    const sf::Color* get(AssetsData::Color name) const;

    void switchTheme();
    AssetsData::Theme getTheme() const;

private:
    AssetsData::Theme mTheme;
    std::map<AssetsData::Image, sf::Texture*> mImageMap;
    std::map<AssetsData::Font, sf::Font*> mFontMap;
    std::map<AssetsData::Color, sf::Color> mLightColorMap, mDarkColorMap;
    std::map<AssetsData::Color, sf::Color*> mColorMap;
};

#endif // ASSETS_HOLDER_HPP