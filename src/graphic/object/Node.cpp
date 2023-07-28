#include "Node.hpp"
#include "../Animation.hpp"

Node::Node() {
    mWindow = nullptr;
    mAssetsHolder = nullptr;
    mValue = 0;
    mPosition = sf::Vector2f(0, 0);
}

Node::Node(sf::RenderWindow* window, AssetsHolder* AssetsHolder) {
    mWindow = window;
    mAssetsHolder = AssetsHolder;
    mValue = 0;
    mPosition = sf::Vector2f(0, 0);
}

int Node::getValue() {
    return mValue;
}

void Node::setValue(int value) {
    mValue = value;
}

sf::Vector2f Node::getPosition() {
    return mPosition;
}

void Node::setPosition(sf::Vector2f position) {
    mPosition = position;
}

void Node::draw(NodeData::Type type, NodeData::State state, std::vector<std::any> args, float ratioTime, bool fakeDraw) {
    sf::Sprite sprite;

    switch (type) {
    case NodeData::hollow:
        sprite.setTexture(*mAssetsHolder->get(AssetsData::Image::hollowCircle));
        break;
    case NodeData::solid:
        sprite.setTexture(*mAssetsHolder->get(AssetsData::Image::solidCircle));
        break;
    default:
        assert(false);
    }

    sf::Text text(std::to_string(mValue), *mAssetsHolder->get(AssetsData::Font::consolasBold), NodeData::textSize);

    switch (state) {
    case NodeData::none:
        sprite.setColor(*std::any_cast<sf::Color*>(args[0]));
        text.setFillColor(*std::any_cast<sf::Color*>(args[1]));
        break;
    case NodeData::fadeIn:
        sprite.setColor(*std::any_cast<sf::Color*>(args[0]));
        text.setFillColor(*std::any_cast<sf::Color*>(args[1]));

        sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, Animation::Bezier(ratioTime) * 255));
        text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, Animation::Bezier(ratioTime) * 255));
        break;
    case NodeData::fadeOut:
        sprite.setColor(*std::any_cast<sf::Color*>(args[0]));
        text.setFillColor(*std::any_cast<sf::Color*>(args[1]));

        sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, (1 - Animation::Bezier(ratioTime)) * 255));
        text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, (1 - Animation::Bezier(ratioTime)) * 255));
        break;
    case NodeData::move:
        sprite.setColor(*std::any_cast<sf::Color*>(args[0]));
        text.setFillColor(*std::any_cast<sf::Color*>(args[1]));

        mPosition = std::any_cast<sf::Vector2f>(args[2]) + (std::any_cast<sf::Vector2f>(args[3]) - std::any_cast<sf::Vector2f>(args[2])) * Animation::Bezier(ratioTime);
        break;
    case NodeData::changeColor:
        sprite.setColor(sf::Color(
            std::any_cast<sf::Color*>(args[0])->r + (std::any_cast<sf::Color*>(args[1])->r - std::any_cast<sf::Color*>(args[0])->r) * Animation::Bezier(ratioTime),
            std::any_cast<sf::Color*>(args[0])->g + (std::any_cast<sf::Color*>(args[1])->g - std::any_cast<sf::Color*>(args[0])->g) * Animation::Bezier(ratioTime),
            std::any_cast<sf::Color*>(args[0])->b + (std::any_cast<sf::Color*>(args[1])->b - std::any_cast<sf::Color*>(args[0])->b) * Animation::Bezier(ratioTime),
            std::any_cast<sf::Color*>(args[0])->a + (std::any_cast<sf::Color*>(args[1])->a - std::any_cast<sf::Color*>(args[0])->a) * Animation::Bezier(ratioTime)
        ));

        text.setFillColor(sf::Color(
            std::any_cast<sf::Color*>(args[2])->r + (std::any_cast<sf::Color*>(args[3])->r - std::any_cast<sf::Color*>(args[2])->r) * Animation::Bezier(ratioTime),
            std::any_cast<sf::Color*>(args[2])->g + (std::any_cast<sf::Color*>(args[3])->g - std::any_cast<sf::Color*>(args[2])->g) * Animation::Bezier(ratioTime),
            std::any_cast<sf::Color*>(args[2])->b + (std::any_cast<sf::Color*>(args[3])->b - std::any_cast<sf::Color*>(args[2])->b) * Animation::Bezier(ratioTime),
            std::any_cast<sf::Color*>(args[2])->a + (std::any_cast<sf::Color*>(args[3])->a - std::any_cast<sf::Color*>(args[2])->a) * Animation::Bezier(ratioTime)
        ));

        break;
    case NodeData::changeValue:
        sprite.setColor(*std::any_cast<sf::Color*>(args[0]));
        text.setFillColor(*std::any_cast<sf::Color*>(args[1]));

        mValue = std::any_cast<int>(args[2]) + (std::any_cast<int>(args[3]) - std::any_cast<int>(args[2])) * Animation::Bezier(ratioTime);

        break;
    default:
        assert(false);
    }

    sprite.setPosition(mPosition);

    text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2.f, text.getLocalBounds().top + text.getLocalBounds().height / 2.f);
    text.setPosition(mPosition.x + NodeData::nodeSize / 2.f, mPosition.y + NodeData::nodeSize / 2.f);

    if (!fakeDraw) {
        mWindow->draw(sprite);
        mWindow->draw(text);
    }
}