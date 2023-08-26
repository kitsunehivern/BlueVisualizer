#include "BTreeVisualizer.hpp"

BTreeVisualizer::Node::Node() {
    values.clear();
    children.clear();
}

float BTreeVisualizer::Node::getLength() {
    if (values.empty()) {
        return 0.f;
    }

    return (int)values.size() * GraphicNodeData::nodeSize.x - ((int)values.size() - 1) * GraphicNodeData::edgeThickness;
}

sf::Vector2f BTreeVisualizer::Node::getUpperPoint() {
    if (values.empty()) {
        return sf::Vector2f(0.f, 0.f);
    }

    return values.front()->getPosition() + sf::Vector2f(getLength() / 2, GraphicNodeData::edgeThickness / 2);
}

sf::Vector2f BTreeVisualizer::Node::getLowerPoint(int index) {
    if (values.empty()) {
        return sf::Vector2f(0.f, 0.f);
    }

    assert(index >= 0 && index <= (int)values.size());

    if (index == (int)values.size()) {
        return values.back()->getPosition() + sf::Vector2f(GraphicNodeData::nodeSize.x - GraphicNodeData::edgeThickness / 2, GraphicNodeData::nodeSize.y - GraphicNodeData::edgeThickness / 2);
    }

    return values[index]->getPosition() + sf::Vector2f(GraphicNodeData::edgeThickness / 2, GraphicNodeData::nodeSize.y - GraphicNodeData::edgeThickness / 2);
}

BTreeVisualizer::BTreeVisualizer() : Visualizer() {
}

BTreeVisualizer::BTreeVisualizer(sf::RenderWindow* window, AssetsHolder* assets) : Visualizer(window, assets) {
    mRoot = nullptr;
    mOldPosition.clear();
}

int BTreeVisualizer::getSize() {
    std::function<int(Node* node)> getSizeSubtree = [&](Node* node) -> int {
        if (node == nullptr) {
            return 0;
        }

        int size = (int)node->values.size();
        for (auto& child : node->children) {
            size += getSizeSubtree(child);
        }

        return size;
    };

    return getSizeSubtree(mRoot);
}

void BTreeVisualizer::clearTree() {
    std::function<void(Node* node)> clearSubtree = [&](Node* node) {
        if (node == nullptr) {
            return;
        }

        for (auto& child : node->children) {
            clearSubtree(child);
        }

        for (auto& value : node->values) {
            delete value;
        }

        delete node;
    };

    clearSubtree(mRoot);
    mRoot = nullptr;
}

int BTreeVisualizer::getChildIndex(Node* node, int value) {
    if (node == nullptr) {
        return -1;
    }

    for (int i = 0; i < (int)node->values.size(); ++i) {
        if (value == std::stoi(node->values[i]->getValue())) {
            return -1;
        }
    }

    if (value < std::stoi(node->values.front()->getValue())) {
        return 0;
    }
    
    if (value > std::stoi(node->values.back()->getValue())) {
        return (int)node->values.size();
    }

    for (int i = 0; i < (int)node->values.size() - 1; ++i) {
        if (value > std::stoi(node->values[i]->getValue()) && value < std::stoi(node->values[i + 1]->getValue())) {
            return i + 1;
        }
    }

    return -1;
}

void BTreeVisualizer::setPositions() {
    if (mRoot == nullptr) {
        return;
    }

    std::function<std::pair<float, float>(Node* node)> getSubtreeRange = [&](Node* node) -> std::pair<float, float> {
        if (node == nullptr) {
            return std::make_pair(1e9, -1e9);
        }

        std::pair<float, float> range = std::make_pair(node->values.front()->getPosition().x, node->values.front()->getPosition().x + node->getLength());
        for (int i = 0; i < (int)node->children.size(); ++i) {
            std::pair<float, float> childRange = getSubtreeRange(node->children[i]);
            range.first = std::min(range.first, childRange.first);
            range.second = std::max(range.second, childRange.second);
        }

        return range;
    };

    std::function<void(Node* node, sf::Vector2f)> offsetSubtree = [&](Node* node, sf::Vector2f offset) {
        if (node == nullptr) {
            return;
        }

        for (int i = 0; i < (int)node->values.size(); ++i) {
            node->values[i]->setPosition(node->values[i]->getPosition() + offset);
        }

        for (int i = 0; i < (int)node->children.size(); ++i) {
            offsetSubtree(node->children[i], offset);
        }
    };

    std::function<void(Node* node)> setSubtreePostion = [&](Node* node) {
        if (node == nullptr) {
            return;
        }

        assert(node->children.empty() || node->children.size() == node->values.size() + 1);

        if (node->children.empty()) {
            node->values.front()->setPosition(GraphicNodeData::initialPosition);
            for (int i = 1; i < (int)node->values.size(); ++i) {
                node->values[i]->setPosition(node->values[i - 1]->getPosition() + sf::Vector2f(GraphicNodeData::nodeSize.x - GraphicNodeData::edgeThickness, 0.f));
            }
            
            return;
        }
    
        for (auto& child : node->children) {
            setSubtreePostion(child);
        }

        float firstChildPositionY = node->children.front()->values.front()->getPosition().y;
        float previousChildPositionX = 0.f;
        std::pair<float, float> subtreeRange = std::make_pair(1e9, -1e9);
        bool isFirstChild = true;
        for (auto &child : node->children) {
            if (!isFirstChild) {
                float deltaY = firstChildPositionY - child->values.front()->getPosition().y;
                offsetSubtree(child, sf::Vector2f(0.f, deltaY));

                float deltaX = previousChildPositionX - getSubtreeRange(child).first + BTreeVisualizerData::space.x;
                offsetSubtree(child, sf::Vector2f(deltaX, 0.f));
            } else {
                isFirstChild = false;
            }

            previousChildPositionX = getSubtreeRange(child).second;
            subtreeRange.first = std::min(subtreeRange.first, getSubtreeRange(child).first);
            subtreeRange.second = std::max(subtreeRange.second, getSubtreeRange(child).second);
        }

        float childLength = subtreeRange.second - subtreeRange.first;
        float nodeLength = node->getLength();
        float offset = (childLength - nodeLength) / 2;
        node->values.front()->setPosition(sf::Vector2f(subtreeRange.first + offset, firstChildPositionY - BTreeVisualizerData::space.y));
        for (int i = 1; i < (int)node->values.size(); ++i) {
            node->values[i]->setPosition(node->values[i - 1]->getPosition() + sf::Vector2f(GraphicNodeData::nodeSize.x - GraphicNodeData::edgeThickness, 0.f));
        }
    };

    setSubtreePostion(mRoot);

    std::pair<float, float> treeRange = getSubtreeRange(mRoot);
    offsetSubtree(mRoot, sf::Vector2f((VisualizerData::visualizeBoxCenter.x - (treeRange.second - treeRange.first) / 2) - treeRange.first, BTreeVisualizerData::treePositionY - mRoot->values.front()->getPosition().y));
}

void BTreeVisualizer::drawReformat() {
    std::vector<GraphicNode*> graphicNodes;
    std::function<void(Node*)> getGraphicNode = [&](Node* node) {
        if (node == nullptr) {
            return;
        }

        for (auto& value : node->values) {
            graphicNodes.push_back(value);
        }

        for (auto& child : node->children) {
            getGraphicNode(child);
        }
    }; getGraphicNode(mRoot);

    std::vector<sf::Vector2f> oldPositions;
    for (GraphicNode* graphicNode : graphicNodes) {
        oldPositions.push_back(graphicNode->getPosition());
        mOldPosition[graphicNode] = graphicNode->getPosition();
    }

    setPositions();

    std::vector<sf::Vector2f> newPositions;
    for (GraphicNode* graphicNode : graphicNodes) {
        newPositions.push_back(graphicNode->getPosition());
    }

    drawChangePosition(graphicNodes, oldPositions, newPositions);
}

void BTreeVisualizer::drawAllNode(Node* node) {
    if (node == nullptr) {
        return;
    }

    for (auto &value : node->values) {
        draw({ value }, Shape::square, Type::hollow, Color::node, Color::nodeText);
    }

    for (auto& child : node->children) {
        drawAllNode(child);
    }
}

void BTreeVisualizer::drawAllEdge(Node* node) {
    if (node == nullptr || node->children.empty()) {
        return;
    }

    for (int i = 0; i < (int)node->children.size(); i++) {
        drawEdgeFree({ std::make_pair(node->getLowerPoint(i), node->children[i]->getUpperPoint()) }, Color::edge);
    }

    for (auto& child : node->children) {
        drawAllEdge(child);
    }
}

void BTreeVisualizer::drawAllNodeFadeIn(Node* node) {
    if (node == nullptr) {
        return;
    }

    for (auto &value : node->values) {
        drawFadeIn({ value }, Shape::square, Type::hollow, Color::node, Color::nodeText);
    }

    for (auto& child : node->children) {
        drawAllNodeFadeIn(child);
    }
}

void BTreeVisualizer::drawAllEdgeFadeIn(Node* node) {
    if (node == nullptr || node->children.empty()) {
        return;
    }

    for (int i = 0; i < (int)node->children.size(); i++) {
        drawEdgeFreeFadeIn({ std::make_pair(node->getLowerPoint(i), node->children[i]->getUpperPoint()) }, Color::edge);
    }

    for (auto& child : node->children) {
        drawAllEdgeFadeIn(child);
    }
}

void BTreeVisualizer::drawAllEdgeChangePosition(Node* node) {
    if (node == nullptr || node->children.empty()) {
        return;
    }

    assert(mOldPosition.count(node->values.front()));
    sf::Vector2f offsetNode = node->values.front()->getPosition() - mOldPosition[node->values.front()];
    std::cerr << "offsetNode: " << offsetNode.x << " " << offsetNode.y << std::endl;
    for (int i = 0; i < (int)node->children.size(); i++) {
        assert(mOldPosition.count(node->children[i]->values.front()));
        sf::Vector2f offsetChild = node->children[i]->values.front()->getPosition() - mOldPosition[node->children[i]->values.front()];
        std::cerr << "offsetChild: " << offsetChild.x << " " << offsetChild.y << std::endl;
        std::cerr << "draw change position " << "(" << node->getLowerPoint(i).x - offsetNode.x << ", " << node->getLowerPoint(i).y - offsetNode.y << ") -> (" << node->children[i]->getUpperPoint().x - offsetChild.x << ", " << node->children[i]->getUpperPoint().y - offsetChild.y << ")" << std::endl;
        std::cerr << "to " << "(" << node->getLowerPoint(i).x << ", " << node->getLowerPoint(i).y << ") -> (" << node->children[i]->getUpperPoint().x << ", " << node->children[i]->getUpperPoint().y << ")" << std::endl;
        drawEdgeFreeChangePosition({ std::make_pair(node->getLowerPoint(i) - offsetNode, node->children[i]->getUpperPoint() - offsetChild) }, { std::make_pair(node->getLowerPoint(i), node->children[i]->getUpperPoint()) }, Color::edge);
    }

    for (auto& child : node->children) {
        drawAllEdgeChangePosition(child);
    }
}

void BTreeVisualizer::create(int size) {
    mCode.update({});

    clearTree();
    mOldPosition.clear();

    std::set<int> tempValues;
    while ((int)tempValues.size() < size) {
        tempValues.insert(Randomizer::random(BTreeVisualizerData::minValue, BTreeVisualizerData::maxValue));
    }

    std::vector<int> values(tempValues.begin(), tempValues.end());
    std::shuffle(values.begin(), values.end(), Randomizer::rng);

    for (auto value : values) {
        if (mRoot == nullptr) {
            mRoot = new Node();
            mRoot->values.push_back(new GraphicNode(std::to_string(value)));
            continue;
        }

        if ((int)mRoot->values.size() == 3) {
            Node* leftChild = new Node();
            leftChild->values.push_back(mRoot->values[0]);
            if (!mRoot->children.empty()) {
                leftChild->children.push_back(mRoot->children[0]);
                leftChild->children.push_back(mRoot->children[1]);
            }

            Node* rightChild = new Node();
            rightChild->values.push_back(mRoot->values[2]);
            if (!mRoot->children.empty()) {
                rightChild->children.push_back(mRoot->children[2]);
                rightChild->children.push_back(mRoot->children[3]);
            }

            mRoot->values = { mRoot->values[1] };
            mRoot->children = { leftChild, rightChild };
        }

        Node* current = mRoot;
        while (true) {
            int childIndex = getChildIndex(current, value);
            if (current->children.empty()) {
                current->values.insert(current->values.begin() + childIndex, new GraphicNode(std::to_string(value)));
                break;
            }

            if ((int)current->children[childIndex]->values.size() == 3) {
                Node* newChild = new Node();

                // move value to parent
                current->values.insert(current->values.begin() + childIndex, current->children[childIndex]->values[1]);
                current->children[childIndex]->values.erase(current->children[childIndex]->values.begin() + 1);

                // split child to new child
                newChild->values.push_back(current->children[childIndex]->values[1]);
                current->children[childIndex]->values.erase(current->children[childIndex]->values.begin() + 1);
                
                // attach new child to parent
                current->children.insert(current->children.begin() + childIndex + 1, newChild);
                
                if (!current->children[childIndex]->children.empty()) {
                    // move children to new child
                    newChild->children.push_back(current->children[childIndex]->children[2]);
                    current->children[childIndex]->children.erase(current->children[childIndex]->children.begin() + 2);
                    
                    // move children to new child
                    newChild->children.push_back(current->children[childIndex]->children[2]);
                    current->children[childIndex]->children.erase(current->children[childIndex]->children.begin() + 2);
                }

                childIndex = getChildIndex(current, value);
            }

            current = current->children[childIndex];
        }
    }

    setPositions();

    addNewStep();
    drawAllNodeFadeIn(mRoot);
    drawAllEdgeFadeIn(mRoot);
    drawCode();
}

void BTreeVisualizer::createFromList(std::string input) {
    for (auto& character : input) {
        if (!(character == '-' || (character >= '0' && character <= '9'))) {
            character = ' ';
        }
    }

    std::stringstream ss(input);
    std::vector<int> values;
    for (std::string token; ss >> token; ) {
        values.push_back(std::stoi(token));
    }

    clearTree();
    mOldPosition.clear();

    for (auto value : values) {
        if (mRoot == nullptr) {
            mRoot = new Node();
            mRoot->values.push_back(new GraphicNode(std::to_string(value)));
            continue;
        }

        if ((int)mRoot->values.size() == 3) {
            Node* leftChild = new Node();
            leftChild->values.push_back(mRoot->values[0]);
            if (!mRoot->children.empty()) {
                leftChild->children.push_back(mRoot->children[0]);
                leftChild->children.push_back(mRoot->children[1]);
            }

            Node* rightChild = new Node();
            rightChild->values.push_back(mRoot->values[2]);
            if (!mRoot->children.empty()) {
                rightChild->children.push_back(mRoot->children[2]);
                rightChild->children.push_back(mRoot->children[3]);
            }

            mRoot->values = { mRoot->values[1] };
            mRoot->children = { leftChild, rightChild };
        }

        Node* current = mRoot;
        while (true) {
            int childIndex = getChildIndex(current, value);
            if (childIndex == -1) {
                break;
            }

            if (current->children.empty()) {
                current->values.insert(current->values.begin() + childIndex, new GraphicNode(std::to_string(value)));
                break;
            }

            if ((int)current->children[childIndex]->values.size() == 3) {
                Node* newChild = new Node();

                current->values.insert(current->values.begin() + childIndex, current->children[childIndex]->values[1]);
                current->children[childIndex]->values.erase(current->children[childIndex]->values.begin() + 1);
                newChild->values.push_back(current->children[childIndex]->values[1]);
                current->children[childIndex]->values.erase(current->children[childIndex]->values.begin() + 1);
                current->children.insert(current->children.begin() + childIndex + 1, newChild);
                
                if (!current->children[childIndex]->children.empty()) {
                    newChild->children.push_back(current->children[childIndex]->children[2]);
                    current->children[childIndex]->children.erase(current->children[childIndex]->children.begin() + 2);
                    newChild->children.push_back(current->children[childIndex]->children[2]);
                    current->children[childIndex]->children.erase(current->children[childIndex]->children.begin() + 2);
                }

                childIndex = getChildIndex(current, value);
            }

            current = current->children[childIndex];
        }
    }

    setPositions();

    addNewStep();
    drawAllNodeFadeIn(mRoot);
    drawAllEdgeFadeIn(mRoot);
    drawCode();
}

void BTreeVisualizer::insert(int value) {
    mCode.update({
        "if node is full: split node",
        "if node.find(v): return",
        "if node is leaf:",
        "    insert v to node, return",
        "node = node.next(v)"
    });

    std::vector<Node*> path;
    Node* current = mRoot;
    path.push_back(current);
    while (true) {
        std::cerr << "current: " << current->values.front()->getValue() << std::endl;

        addNewStep();
        if (current == mRoot) {
            drawReformat();
        }
        std::cerr << "Draw path" << std::endl;
        for (int i = 0; i < (int)path.size() - 1; ++i) {
            for (int j = 0; j < (int)path[i]->children.size(); ++j) {
                if (path[i]->children[j] != path[i + 1]) {
                    drawAllEdge(path[i]->children[j]);
                    drawEdgeFree({ std::make_pair(path[i]->getLowerPoint(j), path[i]->children[j]->getUpperPoint()) }, Color::edge);
                    drawAllNode(path[i]->children[j]);
                }
            }

            if (i == (int)path.size() - 2) {
                drawEdgeFree({ std::make_pair(path[i]->getLowerPoint(getChildIndex(path[i], value)), path[i + 1]->getUpperPoint()) }, Color::edge);
                drawEdgeFreeSlideIn({ std::make_pair(path[i]->getLowerPoint(getChildIndex(path[i], value)), path[i + 1]->getUpperPoint()) }, Color::edgeFocus);
            } else {
                drawEdgeFree({ std::make_pair(path[i]->getLowerPoint(getChildIndex(path[i], value)), path[i + 1]->getUpperPoint()) }, Color::edgeFocus);
            }

            for (auto value : path[i]->values) {
                draw({ value }, Shape::square, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            }

            if (i == (int)path.size() - 2) {
                for (auto value : path[i]->values) {
                    drawFadeOut({ value }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                }
            }
        }
        drawAllEdge(current);
        drawAllNode(current);
        for (auto value : current->values) {
            drawFadeIn({ value }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        }

        int childIndex = getChildIndex(current, value);
        if (current->children.empty() ||  childIndex == -1) {
            break;
        }

        if ((int)current->children[childIndex]->values.size() == 3) {
            Node* newChild = new Node();
            current->values.insert(current->values.begin() + childIndex, current->children[childIndex]->values[1]);
            current->children[childIndex]->values.erase(current->children[childIndex]->values.begin() + 1);
            newChild->values.push_back(current->children[childIndex]->values[1]);
            current->children[childIndex]->values.erase(current->children[childIndex]->values.begin() + 1);
            current->children.insert(current->children.begin() + childIndex + 1, newChild);
            if (!current->children[childIndex]->children.empty()) {
                newChild->children.push_back(current->children[childIndex]->children[2]);
                current->children[childIndex]->children.erase(current->children[childIndex]->children.begin() + 2);
                newChild->children.push_back(current->children[childIndex]->children[2]);
                current->children[childIndex]->children.erase(current->children[childIndex]->children.begin() + 2);
            }

            addNewStep();
            drawReformat();
            drawAllEdgeChangePosition(mRoot);
            drawAllNode(mRoot);

            // for (int i = 0; i < (int)path.size() - 1; ++i) {
            //     for (int j = 0; j < (int)path[i]->children.size(); ++j) {
            //         if (path[i]->children[j] != path[i + 1]) {
            //             drawAllEdge(path[i]->children[j]);
            //             drawEdgeFree({ std::make_pair(path[i]->getLowerPoint(j), path[i]->children[j]->getUpperPoint()) }, Color::edge);
            //             drawAllNode(path[i]->children[j]);
            //         }
            //     }

            //     if (i == (int)path.size() - 2) {
            //         drawEdgeFree({ std::make_pair(path[i]->getLowerPoint(getChildIndex(path[i], value)), path[i + 1]->getUpperPoint()) }, Color::edge);
            //         drawEdgeFreeSlideIn({ std::make_pair(path[i]->getLowerPoint(getChildIndex(path[i], value)), path[i + 1]->getUpperPoint()) }, Color::edgeFocus);
            //     } else {
            //         drawEdgeFree({ std::make_pair(path[i]->getLowerPoint(getChildIndex(path[i], value)), path[i + 1]->getUpperPoint()) }, Color::edgeFocus);
            //     }

            //     for (auto value : path[i]->values) {
            //         draw({ value }, Shape::square, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            //     }

            //     if (i == (int)path.size() - 2) {
            //         for (auto value : path[i]->values) {
            //             drawFadeOut({ value }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            //         }
            //     }
            // }
            // drawAllEdge(current);
            // drawAllNode(current);
            // for (auto value : current->values) {
            //     drawFadeIn({ value }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            // }

            // childIndex = getChildIndex(current, value);
        }

        current = current->children[childIndex];
        path.push_back(current);
    }
}

void BTreeVisualizer::run() {
    std::function<bool()> conditionNone = [&]() { return true; };
    std::function<bool()> conditionSizeNotLarge = [&]() { return getSize() < BTreeVisualizerData::maxSize; };

    mOption.addOption("Create");
    mOption.addSuboption("Empty", conditionNone);
    mOption.addSuboption("Random", conditionNone);
    mOption.addSuboptionInputBox("n",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, BTreeVisualizerData::minSize, BTreeVisualizerData::maxSize),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), BTreeVisualizerData::minSize, BTreeVisualizerData::maxSize)
    );

    mOption.addSuboption("File", conditionNone);
    mOption.addSuboptionFileBox("v", std::bind(Validator::isListOfIntegerInRange, std::placeholders::_1, std::placeholders::_2, BTreeVisualizerData::minSize, BTreeVisualizerData::maxSize, BTreeVisualizerData::minValue, BTreeVisualizerData::maxValue));


    mOption.addOption("Insert");
    mOption.addSuboption("Value", conditionSizeNotLarge);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, BTreeVisualizerData::minValue, BTreeVisualizerData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), BTreeVisualizerData::minValue, BTreeVisualizerData::maxValue)
    );

    mOption.processOption();

    clearAllSteps();
    create(BTreeVisualizerData::defaultSize);

    while (mWindow->isOpen()) {
        updateState();

        sf::Event event;
        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mWindow->close();
                return;
            }

            VisualizerData::Event action = handleEvent(event);
            if (action == VisualizerData::Event::quit) {
                return;
            }

            if (action == VisualizerData::Event::confirm) {
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

                    case 2: // File
                        createFromList(values[0]);
                        break;
                    }
                    
                    break;

                case 1: // Insert
                    switch (option.second) {
                    case 0: // Value
                        insert(std::stoi(values[0]));
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