#include "BinaryTreeVisualizer.hpp"

BinaryTreeVisualizer::BinaryTreeVisualizer() : Visualizer() {
    mRoot = nullptr;
}

BinaryTreeVisualizer::BinaryTreeVisualizer(sf::RenderWindow* window, AssetsHolder* assets) : Visualizer(window, assets) {
    mRoot = nullptr;
}

void BinaryTreeVisualizer::setPositions() {
    std::function<sf::Vector2f(Node* node)> getSubtreeRange = [&](Node* node) {
        if (node == nullptr) {
            return sf::Vector2f(1e9, -1e9);
        }

        return sf::Vector2f(std::min(node->value.getPosition().x, getSubtreeRange(node->left).x), std::max(node->value.getPosition().x, getSubtreeRange(node->right).y));
    };

    std::function<void(Node*, sf::Vector2f)> offsetSubtree = [&](Node* node, sf::Vector2f offset) {
        if (node == nullptr) {
            return;
        }

        node->value.setPosition(node->value.getPosition() + offset);
        if (node->left != nullptr) {
            offsetSubtree(node->left, offset);
        }
        if (node->right != nullptr) {
            offsetSubtree(node->right, offset);
        }
    };

    std::function<void(Node*)> setSubtreePosition = [&](Node* node) {
        if (node->left != nullptr) {
            setSubtreePosition(node->left);
        }
        if (node->right != nullptr) {
            setSubtreePosition(node->right);
        }

        if (node->right == nullptr && node->left == nullptr) {
            node->value.setPosition(GraphicNodeData::initialPosition);
        } else if (node->left == nullptr) {
            sf::Vector2f rightSubtreeRange = getSubtreeRange(node->right);
            node->value.setPosition(sf::Vector2f(rightSubtreeRange.x - BinaryTreeVisualizerData::space.x, node->right->value.getPosition().y - BinaryTreeVisualizerData::space.y));
        } else if (node->right == nullptr) {
            sf::Vector2f leftSubtreeRange = getSubtreeRange(node->left);
            node->value.setPosition(sf::Vector2f(leftSubtreeRange.y + BinaryTreeVisualizerData::space.x, node->left->value.getPosition().y - BinaryTreeVisualizerData::space.y));
        } else {
            float deltaY = node->right->value.getPosition().y - node->left->value.getPosition().y;
            offsetSubtree(node->right, sf::Vector2f(0.f, -deltaY));

            sf::Vector2f leftSubtreeRange = getSubtreeRange(node->left);
            sf::Vector2f rightSubtreeRange = getSubtreeRange(node->right);
            float deltaX = rightSubtreeRange.x - leftSubtreeRange.y - 2 * BinaryTreeVisualizerData::space.x;
            offsetSubtree(node->right, sf::Vector2f(-deltaX, 0.f));
            rightSubtreeRange = getSubtreeRange(node->right);

            node->value.setPosition(sf::Vector2f((leftSubtreeRange.y + rightSubtreeRange.x) / 2.f, node->right->value.getPosition().y - BinaryTreeVisualizerData::space.y));
        }
    };

    if (mRoot == nullptr) {
        return;
    }
    
    setSubtreePosition(mRoot);

    sf::Vector2f treeRange = getSubtreeRange(mRoot);
    offsetSubtree(mRoot, sf::Vector2f(((float)mWindow->getSize().x - (treeRange.y - treeRange.x)) / 2.f - treeRange.x, BinaryTreeVisualizerData::treePositionY - mRoot->value.getPosition().y));
    offsetSubtree(mRoot, sf::Vector2f(-GraphicNodeData::nodeSize.x / 2.f, -GraphicNodeData::nodeSize.y / 2.f));
}

void BinaryTreeVisualizer::drawReformat() {
    std::vector<GraphicNode*> graphicNodes;
    std::function<void(Node*)> getGraphicNode = [&](Node* node) {
        if (node == nullptr) {
            return;
        }

        graphicNodes.push_back(&node->value);
        getGraphicNode(node->left);
        getGraphicNode(node->right);
    }; getGraphicNode(mRoot);

    std::vector<sf::Vector2f> oldPositions;
    for (GraphicNode* graphicNode : graphicNodes) {
        oldPositions.push_back(graphicNode->getPosition());
    }

    setPositions();

    std::vector<sf::Vector2f> newPositions;
    for (GraphicNode* graphicNode : graphicNodes) {
        newPositions.push_back(graphicNode->getPosition());
    }

    drawChangePosition(graphicNodes, oldPositions, newPositions);
}

void BinaryTreeVisualizer::drawAllNode(Node* node) {
    if (node == nullptr) {
        return;
    }

    draw({ &node->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);

    drawAllNode(node->left);
    drawAllNode(node->right);
}

void BinaryTreeVisualizer::drawAllNodeFadeIn(Node* node) {
    if (node == nullptr) {
        return;
    }

    drawFadeIn({ &node->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);

    drawAllNodeFadeIn(node->left);
    drawAllNodeFadeIn(node->right);
}

void BinaryTreeVisualizer::drawAllEdge(Node* node) {
    if (node == nullptr) {
        return;
    }

    if (node->left != nullptr) {
        drawEdge({ std::make_pair(&node->value, &node->left->value) }, Color::edge);
    }

    if (node->right != nullptr) {
        drawEdge({ std::make_pair(&node->value, &node->right->value) }, Color::edge);
    }

    drawAllEdge(node->left);
    drawAllEdge(node->right);
}

void BinaryTreeVisualizer::drawAllEdgeFadeIn(Node* node) {
    if (node == nullptr) {
        return;
    }

    if (node->left != nullptr) {
        drawEdgeFadeIn({ std::make_pair(&node->value, &node->left->value) }, Color::node);
    }

    if (node->right != nullptr) {
        drawEdgeFadeIn({ std::make_pair(&node->value, &node->right->value) }, Color::node);
    }

    drawAllEdgeFadeIn(node->left);
    drawAllEdgeFadeIn(node->right);
}

void BinaryTreeVisualizer::drawAllEdgeFixed(Node* node) {
    if (node == nullptr) {
        return;
    }

    if (node->left != nullptr) {
        drawEdgeFixed({ std::make_pair(&node->value, &node->left->value) }, Color::node);
    }

    if (node->right != nullptr) {
        drawEdgeFixed({ std::make_pair(&node->value, &node->right->value) }, Color::node);
    }

    drawAllEdgeFixed(node->left);
    drawAllEdgeFixed(node->right);
}