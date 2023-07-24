#ifndef OPTION_BOX_HPP
#define OPTION_BOX_HPP

#include "../../Global.hpp"
#include "../AssetsHolder.hpp"

class OptionBox : public sf::Drawable {
public:
    OptionBox();
    OptionBox(AssetsHolder* assets, sf::Vector2f position);

    void addOption(const std::string& option);
    void addSuboption(const std::string& suboption);

private:
    AssetsHolder* mAssets;
    sf::Vector2f mPosition;
    std::vector<std::string> mOptionList;
    int mOptionIndex;
    std::vector<std::vector<std::string>> mSuboptionList;
    std::vector<int> mSuboptionIndex;
    std::vector<std::vector<std::function<bool()>>> mSuboptionConditionList;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // OPTION_BOX_HPP