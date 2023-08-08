#include "AVLTree.hpp"

AVLTree::AVLTree() {
}

AVLTree::AVLTree(sf::RenderWindow* window, AssetsHolder* assets) : Visualizer(window, assets) {
}

void AVLTree::setPositions() {
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
            node->value.setPosition(sf::Vector2f(0.f, 0.f));
        } else if (node->left == nullptr) {
            sf::Vector2f rightSubtreeRange = getSubtreeRange(node->right);
            node->value.setPosition(sf::Vector2f(rightSubtreeRange.x - AVLTreeData::space.x, node->right->value.getPosition().y - AVLTreeData::space.y));
        } else if (node->right == nullptr) {
            sf::Vector2f leftSubtreeRange = getSubtreeRange(node->left);
            node->value.setPosition(sf::Vector2f(leftSubtreeRange.y + AVLTreeData::space.x, node->left->value.getPosition().y - AVLTreeData::space.y));
        } else {
            int deltaY = node->right->value.getPosition().y - node->left->value.getPosition().y;
            offsetSubtree(node->right, sf::Vector2f(0.f, -deltaY));

            sf::Vector2f leftSubtreeRange = getSubtreeRange(node->left);
            sf::Vector2f rightSubtreeRange = getSubtreeRange(node->right);
            int deltaX = rightSubtreeRange.x - leftSubtreeRange.y - 2 * AVLTreeData::space.x;
            offsetSubtree(node->right, sf::Vector2f(-deltaX, 0.f));
            rightSubtreeRange = getSubtreeRange(node->right);

            node->value.setPosition(sf::Vector2f((leftSubtreeRange.y + rightSubtreeRange.x) / 2.f, node->right->value.getPosition().y - AVLTreeData::space.y));
        }
    };

    if (mRoot == nullptr) {
        return;
    }
    
    setSubtreePosition(mRoot);

    sf::Vector2f treeRange = getSubtreeRange(mRoot);
    offsetSubtree(mRoot, sf::Vector2f(((float)mWindow->getSize().x - (treeRange.y - treeRange.x)) / 2.f - treeRange.x, AVLTreeData::treePositionY - mRoot->value.getPosition().y));
    offsetSubtree(mRoot, sf::Vector2f(-GraphicNodeData::nodeSize.x / 2.f, -GraphicNodeData::nodeSize.y / 2.f));
}

std::vector<GraphicNode*> AVLTree::getGraphicNodes(Node* node) {
    std::vector<GraphicNode*> nodes;
    if (node == nullptr) {
        return nodes;
    }

    nodes.push_back(&node->value);
    std::vector<GraphicNode*> leftNodes = getGraphicNodes(node->left);
    std::vector<GraphicNode*> rightNodes = getGraphicNodes(node->right);
    nodes.insert(nodes.end(), leftNodes.begin(), leftNodes.end());
    nodes.insert(nodes.end(), rightNodes.begin(), rightNodes.end());

    return nodes;
}

int AVLTree::getBalanceFactor(Node* node) {
    if (node == nullptr) {
        return 0;
    }

    return height(node->right) - height(node->left);
}

BinaryTree<GraphicNode>::Node* AVLTree::leftRotate(Node* node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;

    return newRoot;
}

BinaryTree<GraphicNode>::Node* AVLTree::rightRotate(Node* node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;

    return newRoot;
}

void AVLTree::drawAllEdge(Node* node) {
    if (node == nullptr) {
        return;
    }

    if (node->left != nullptr) {
        drawEdge({ std::make_pair(&node->value, &node->left->value) }, Color::node);
    }

    if (node->right != nullptr) {
        drawEdge({ std::make_pair(&node->value, &node->right->value) }, Color::node);
    }

    drawAllEdge(node->left);
    drawAllEdge(node->right);
}

void AVLTree::drawAllEdgeFadeIn(Node* node) {
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

void AVLTree::create(int n) {
    clear();
    std::set<int> tempValues;
    while ((int)tempValues.size() < n) {
        tempValues.insert(Randomizer::random(AVLTreeData::minValue, AVLTreeData::maxValue));
    }

    std::vector<int> values(tempValues.begin(), tempValues.end());
    std::shuffle(values.begin(), values.end(), Randomizer::rng);

    for (int value : values) {
        std::vector<Node*> path;
        Node* current = mRoot;
        while (current != nullptr) {
            path.push_back(current);
            if (value < std::stoi(current->value.getValue())) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (path.empty()) {
            mRoot = new Node(GraphicNode(std::to_string(value), sf::Vector2f(0.f, 0.f)));
        } else if (value < std::stoi(path.back()->value.getValue())) {
            path.back()->left = new Node(GraphicNode(std::to_string(value), sf::Vector2f(0.f, 0.f)));
        } else {
            path.back()->right = new Node(GraphicNode(std::to_string(value), sf::Vector2f(0.f, 0.f)));
        }

        while (!path.empty()) {
            Node* current = path.back();
            path.pop_back();

            int balanceFactor = getBalanceFactor(current);
            if (balanceFactor > 1) {
                if (value > std::stoi(current->right->value.getValue())) {
                    current = leftRotate(current);
                } else {
                    current->right = rightRotate(current->right);
                    current = leftRotate(current);
                }
            } else if (balanceFactor < -1) {
                if (value < std::stoi(current->left->value.getValue())) {
                    current = rightRotate(current);
                } else {
                    current->left = leftRotate(current->left);
                    current = rightRotate(current);
                }
            }

            if (path.empty()) {
                mRoot = current;
            } else if (value < std::stoi(path.back()->value.getValue())) {
                path.back()->left = current;
            } else {
                path.back()->right = current;
            }
        }
    }

    setPositions();

    mCode.update({});

    addNewStep();
    drawFadeIn(getGraphicNodes(mRoot), Shape::circle, Type::hollow, Color::node, Color::nodeText);
    drawAllEdgeFadeIn(mRoot);
    drawCode();
}

void AVLTree::run() { 
    std::function <bool()> conditionNone = []() { return true; };

    mOption.addOption("Create");
    mOption.addSuboption("Empty", conditionNone);
    mOption.addSuboption("Random", conditionNone);
    mOption.addSuboptionInputBox("n",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, AVLTreeData::minSize, AVLTreeData::maxSize),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), AVLTreeData::minSize, AVLTreeData::maxSize)
    );

    mOption.processOption();

    clearAllSteps();
    create(10);

    while (mWindow->isOpen()) {
        updateState();

        sf::Event event;
        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mWindow->close();
            }

            if (handleEvent(event)) {
                abortAllSteps();
                clearAllSteps();

                std::pair<int, int> option = mOption.getOption();
                std::vector<std::string> values = mOption.getValues();

                switch (option.first) {
                case 0: // Create
                    switch (option.second) {
                    case 0: // Empty
                        create(0);
                        break;

                    case 1: // Random
                        create(std::stoi(values[0]));
                        break;
                    }
                    
                    break;
                }
            }
        }

        mWindow->clear(sf::Color::White);
        draw();
        mWindow->display();
    }
}
