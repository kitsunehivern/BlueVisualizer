#include "TreeVisualizer.hpp"

TreeVisualizer::TreeVisualizer() : Visualizer() {
    mRoot = nullptr;
}

TreeVisualizer::TreeVisualizer(sf::RenderWindow* window, AssetsHolder* assets) : Visualizer(window, assets) {
    mRoot = nullptr;
}

void TreeVisualizer::setPositions() {
    std::function<sf::Vector2f(Node* node)> getSubtreeRange = [&](Node* node) {
        if (node == nullptr) {
            return sf::Vector2f(1e9, -1e9);
        }

        return sf::Vector2f(
            std::min(node->value.getPosition().x, getSubtreeRange(node->children.begin()->second).x),
            std::max(node->value.getPosition().x, getSubtreeRange(node->children.rbegin()->second).y)
        );
    };

    std::function<void(Node*, sf::Vector2f)> offsetSubtree = [&](Node* node, sf::Vector2f offset) {
        if (node == nullptr) {
            return;
        }

        node->value.setPosition(node->value.getPosition() + offset);
        for (auto& child : node->children) {
            offsetSubtree(child.second, offset);
        }
    };

    std::function<void(Node*)> setSubtreePosition = [&](Node* node) {
        for (auto& child : node->children) {
            setSubtreePosition(child.second);
        }

        if (node->children.empty()) {
            node->value.setPosition(GraphicNodeData::initialPosition);
            return;
        }

        sf::Vector2f firstChildPosition = node->children.begin()->second->value.getPosition();
        sf::Vector2f lastChildPosition = node->children.rbegin()->second->value.getPosition();
        for (auto& child : node->children) {
            if (child.second->value.getPosition() == firstChildPosition) {
                continue;
            }

            float deltaY = child.second->value.getPosition().y - firstChildPosition.y;
            offsetSubtree(child.second, sf::Vector2f(0.f, -deltaY));

            float deltaX = child.second->value.getPosition().x - lastChildPosition.x - 2 * TreeVisualizerData::space.x;
            offsetSubtree(child.second, sf::Vector2f(-deltaX, 0.f));

            lastChildPosition = child.second->value.getPosition();
        }
    };

    if (mRoot == nullptr) {
        return;
    }
    
    setSubtreePosition(mRoot);

    sf::Vector2f treeRange = getSubtreeRange(mRoot);
    offsetSubtree(mRoot, sf::Vector2f(((float)mWindow->getSize().x - (treeRange.y - treeRange.x)) / 2.f - treeRange.x, TreeVisualizerData::treePositionY - mRoot->value.getPosition().y));
    offsetSubtree(mRoot, sf::Vector2f(-GraphicNodeData::nodeSize.x / 2.f, -GraphicNodeData::nodeSize.y / 2.f));
}

void TreeVisualizer::drawReformat() {
    std::vector<GraphicNode*> graphicNodes;
    std::function<void(Node*)> getGraphicNode = [&](Node* node) {
        if (node == nullptr) {
            return;
        }

        graphicNodes.push_back(&node->value);
        for (auto& child : node->children) {
            getGraphicNode(child.second);
        }
    }; getGraphicNode(mRoot);

    std::vector<sf::Vector2f> oldPositions;
    for (auto& graphicNode : graphicNodes) {
        oldPositions.push_back(graphicNode->getPosition());
    }

    setPositions();

    std::vector<sf::Vector2f> newPositions;
    for (auto& graphicNode : graphicNodes) {
        newPositions.push_back(graphicNode->getPosition());
    }
    
    drawChangePosition(graphicNodes, oldPositions, newPositions);
}