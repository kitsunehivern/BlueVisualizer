#include "AssetsHolder.hpp"

AssetsHolder::AssetsHolder() {
    std::ifstream fin;
	fin.open("assets/data/theme.dat", std::ios::binary);
	if (!fin.is_open()) {
		mTheme = AssetsData::Theme::light;
	} else {
		bool value;
		fin.read((char*)&value, 1);

		mTheme = !value ? AssetsData::Theme::light : AssetsData::Theme::dark;
	}
	fin.close();
}

AssetsHolder::~AssetsHolder() {
    for (auto& pair : mImageMap) {
        delete pair.second;
    }

    for (auto& pair : mFontMap) {
        delete pair.second;
    }

    for (auto& pair : mColorMap) {
        delete pair.second;
    }

    int theme = mTheme == AssetsData::Theme::light ? 0 : 1;
	std::ofstream fout;
	fout.open("assets/data/theme.dat", std::ios::binary);
	fout.write((char*)&theme, 1);
	fout.close();
}

void AssetsHolder::switchTheme() {
    if (mTheme == AssetsData::Theme::dark) {
        mTheme = AssetsData::Theme::light;
        for (auto pair : mLightColorMap) {
            if (!mColorMap.count(pair.first)) {
                mColorMap[pair.first] = new sf::Color(pair.second);
            } else {
                *mColorMap[pair.first] = pair.second;
            }
        }
    } else {
        mTheme = AssetsData::Theme::dark;
        for (auto pair : mDarkColorMap) {
            if (!mColorMap.count(pair.first)) {
                mColorMap[pair.first] = new sf::Color(pair.second);
            } else {
                *mColorMap[pair.first] = pair.second;
            }
        }
    }
}

AssetsData::Theme AssetsHolder::getTheme() const {
    return mTheme;
}

void AssetsHolder::load(AssetsData::Image name, const std::string& filename) {
    sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(filename)) {
        assert(false);
    }

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
    if (!font->loadFromFile(filename)) {
        assert(false);
    }
    
    mFontMap[name] = font;
}

sf::Font* AssetsHolder::get(AssetsData::Font name) {
    return mFontMap[name];
}

const sf::Font* AssetsHolder::get(AssetsData::Font name) const {
    return mFontMap.at(name);
}

void AssetsHolder::setLight(AssetsData::Color name, sf::Color color) {
    mLightColorMap[name] = color;
}

void AssetsHolder::setDark(AssetsData::Color name, sf::Color color) {
    mDarkColorMap[name] = color;
}

sf::Color* AssetsHolder::get(AssetsData::Color name) {
    return mColorMap[name];
}

const sf::Color* AssetsHolder::get(AssetsData::Color name) const {
    return mColorMap.at(name);
}