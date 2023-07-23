#ifndef TextureHolder_HPP
#define TextureHolder_HPP

#include "../Global.hpp"

namespace AssetsData {
    enum ImageName {
        hollowCircle,
        solidCircle,
    };

    enum FontName {
        consolasBold
    };

    enum ColorName {
        nodeCircleColor,
        nodeFocusCircleColor1, // Foucs 1
        nodeFocusCircleColor2, // Focus 2
        nodeFocusCircleColor3, // Inserted
        nodeFocusCircleColor4, // Erased
        nodeTextColor,
        nodeFocusTextColor1, // Focus 1
        nodeFocusTextColor2, // Focus 2
        nodeInsertedCircleColor,
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