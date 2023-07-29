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

void AssetsHolder::load(AssetsData::Image name, const std::string& filename) {
    sf::Texture* texture = new sf::Texture();
    texture->loadFromFile(filename);
    texture->setSmooth(true);
    mImageMap[name] = texture;
}

sf::Texture* AssetsHolder::get(AssetsData::Image name) {
    return mImageMap[name];
}

const sf::Texture* AssetsHolder::get(AssetsData::Image name) const {
    return mImageMap.at(name);
}

void AssetsHolder::load(AssetsData::Font name, const std::string& filename) {
    sf::Font* font = new sf::Font();
    font->loadFromFile(filename);
    mFontMap[name] = font;
}

sf::Font* AssetsHolder::get(AssetsData::Font name) {
    return mFontMap[name];
}

const sf::Font* AssetsHolder::get(AssetsData::Font name) const {
    return mFontMap.at(name);
}

void AssetsHolder::set(AssetsData::Color name, sf::Color color) {
    mLightColorMap[name] = new sf::Color(color);
}

sf::Color* AssetsHolder::get(AssetsData::Color name) {
    return mLightColorMap[name];
}

const sf::Color* AssetsHolder::get(AssetsData::Color name) const {
    return mLightColorMap.at(name);
}