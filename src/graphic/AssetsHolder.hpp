#ifndef ASSETS_HOLDER_HPP
#define ASSETS_HOLDER_HPP

#include "../Global.hpp"

namespace AssetsData {
    enum Image {
        hollowCircle,
        solidCircle,

        inputBox,
        randomBox,
        fileBox,
        randomIcon,
        optionTable,
        suboptionTable,
        inputTable,
        confirmBox,
        controlBox,
        controlButtons,
        statusButtons,
    };

    enum Font {
        consolasBold,
    };

    enum Color {
        none,

        nodeCircle,
        nodeCircleFocus1, // Foucs 1
        nodeCircleFocus2, // Focus 2
        nodeCircleFocus3, // Inserted
        nodeCircleFocus4, // Erased
        nodeText,
        nodeTextFocus1, // Focus 1
        nodeTextFocus2, // Focus 2

        boxComponent,
        box,
        boxFocus,
        boxActive,
        boxInvalid,
        boxDisabled,

        boxText,
        boxTextFocus,
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

    void set(AssetsData::Color name, sf::Color color);
    sf::Color* get(AssetsData::Color name);
    const sf::Color* get(AssetsData::Color name) const;

private:
    std::map<AssetsData::Image, sf::Texture*> mImageMap;
    std::map<AssetsData::Font, sf::Font*> mFontMap;
    std::map<AssetsData::Color, sf::Color*> mLightColorMap;
};

#endif // TextureHolder_HPP