#ifndef ASSETS_HOLDER_HPP
#define ASSETS_HOLDER_HPP

#include "../Global.hpp"

namespace AssetsData {
    enum ImageName {
        hollowCircle,
        solidCircle,

        inputBox,
        randomBox,
    };

    enum FontName {
        consolasBold
    };

    enum ColorName {
        nodeCircleColor,
        nodeCircleFocusColor1, // Foucs 1
        nodeCircleFocusColor2, // Focus 2
        nodeCircleFocusColor3, // Inserted
        nodeCircleFocusColor4, // Erased
        nodeTextColor,
        nodeTextFocusColor1, // Focus 1
        nodeTextFocusColor2, // Focus 2

        boxComponentColor,
        boxColor,
        boxFocusColor,
        boxTextColor,
        boxTextFocusColor,  
    };
}

class AssetsHolder {
public:
    AssetsHolder();
    ~AssetsHolder();

    void load(AssetsData::ImageName name, const std::string& filename);
    sf::Texture* get(AssetsData::ImageName name);

    void load(AssetsData::FontName name, const std::string& filename);
    sf::Font* get(AssetsData::FontName name);

    void set(AssetsData::ColorName name, sf::Color color);
    sf::Color* get(AssetsData::ColorName name);

private:
    std::map<AssetsData::ImageName, sf::Texture*> mImageMap;
    std::map<AssetsData::FontName, sf::Font*> mFontMap;
    std::map<AssetsData::ColorName, sf::Color*> mLightColorMap;
};

#endif // TextureHolder_HPP