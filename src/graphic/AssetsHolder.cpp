#include "AssetsHolder.hpp"

AssetsHolder::AssetsHolder() {
}

AssetsHolder::~AssetsHolder() {
    for (auto& pair : mImageMap) {
        delete pair.second;
    }

    for (auto& pair : mFontMap) {
        delete pair.second;
    }

    for (auto& pair : mLightColorMap) {
        delete pair.second;
    }
}

void AssetsHolder::load(AssetsData::ImageName name, const std::string& filename) {
    sf::Texture* texture = new sf::Texture();
    texture->loadFromFile(filename);
    mImageMap[name] = texture;
}

sf::Texture* AssetsHolder::get(AssetsData::ImageName name) {
    return mImageMap[name];
}

void AssetsHolder::load(AssetsData::FontName name, const std::string& filename) {
    sf::Font* font = new sf::Font();
    font->loadFromFile(filename);
    mFontMap[name] = font;
}

sf::Font* AssetsHolder::get(AssetsData::FontName name) {
    return mFontMap[name];
}

void AssetsHolder::set(AssetsData::ColorName name, sf::Color color) {
    mLightColorMap[name] = new sf::Color(color);
}

sf::Color* AssetsHolder::get(AssetsData::ColorName name) {
    return mLightColorMap[name];
}