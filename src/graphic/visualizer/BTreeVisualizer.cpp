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

void BTreeVisualizer::drawEdgeChangePosition(Node* node, int childIndex, Color color) {
    if (node == nullptr || node->children.empty() || childIndex == -1) {
        return;
    }

    sf::Vector2f offsetNode = node->values.front()->getPosition() - mOldPosition[node->values.front()];
    sf::Vector2f offsetChild = node->children[childIndex]->values.front()->getPosition() - mOldPosition[node->children[childIndex]->values.front()];
    drawEdgeFreeChangePosition({ std::make_pair(node->getLowerPoint(childIndex) - offsetNode, node->children[childIndex]->getUpperPoint() - offsetChild) }, { std::make_pair(node->getLowerPoint(childIndex), node->children[childIndex]->getUpperPoint()) }, color);
}

void BTreeVisualizer::drawEdgeChangePositionSlideIn(Node* node, int childIndex, Color color) {
    if (node == nullptr || node->children.empty() || childIndex == -1) {
        return;
    }

    sf::Vector2f offsetNode = node->values.front()->getPosition() - mOldPosition[node->values.front()];
    sf::Vector2f offsetChild = node->children[childIndex]->values.front()->getPosition() - mOldPosition[node->children[childIndex]->values.front()];
    drawEdgeFreeChangePositionSlideIn({ std::make_pair(node->getLowerPoint(childIndex) - offsetNode, node->children[childIndex]->getUpperPoint() - offsetChild) }, { std::make_pair(node->getLowerPoint(childIndex), node->children[childIndex]->getUpperPoint()) }, color);
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

void BTreeVisualizer::drawAllNodeExceptRoot(Node* node) {
    if (node == nullptr) {
        return;
    }   

    for (auto& child : node->children) {
        drawAllNode(child);
    }
}

void BTreeVisualizer::drawAllEdgeChangePosition(Node* node) {
    if (node == nullptr || node->children.empty()) {
        return;
    }

    for (int i = 0; i < (int)node->children.size(); i++) {
        drawEdgeChangePosition(node, i, Color::edge);
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

void BTreeVisualizer::search(int value) {
    mCode.update({
        "if node.find(v): return FOUND",
        "if node is leaf: return NOT_FOUND",
        "node = node.next(v)"
    });

    std::vector<Node*> path;
    Node* current = mRoot;
    path.push_back(current);
    while (true) {
        addNewStep();
        if (current == mRoot) {
            drawReformat();
        }

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

            for (auto gnode : path[i]->values) {
                draw({ gnode }, Shape::square, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            }

            if (i == (int)path.size() - 2) {
                for (auto gnode : path[i]->values) {
                    drawFadeOut({ gnode }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                }
            }
        }
        drawAllEdge(current);
        drawAllNode(current);
        for (auto gnode : current->values) {
            drawFadeIn({ gnode }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        }
        
        if (current == mRoot) {
            drawCodeFadeIn(2);
        } else {
            drawCodeChangeLine(2, 2);
        }

        int childIndex = getChildIndex(current, value);
        if (current->children.empty() ||  childIndex == -1) {
            break;
        }

        current = current->children[childIndex];
        path.push_back(current);
    }

    int exist = -1;
    for (int i = 0; i < (int)current->values.size(); ++i) {
        if (value == std::stoi(current->values[i]->getValue())) {
            exist = i;
            break;
        }
    }

    addNewStep();
    for (int i = 0; i < (int)path.size() - 1; ++i) {
        for (int j = 0; j < (int)path[i]->children.size(); ++j) {
            if (path[i]->children[j] != path[i + 1]) {
                drawAllEdge(path[i]->children[j]);
                drawEdgeFree({ std::make_pair(path[i]->getLowerPoint(j), path[i]->children[j]->getUpperPoint()) }, Color::edge);
                drawAllNode(path[i]->children[j]);
            }
        }

        drawEdgeFree({ std::make_pair(path[i]->getLowerPoint(getChildIndex(path[i], value)), path[i + 1]->getUpperPoint()) }, Color::edgeFocus);

        for (auto gnode : path[i]->values) {
            draw({ gnode }, Shape::square, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
        }
    }
    drawAllEdge(current);
    drawAllNodeExceptRoot(current);
    if (exist != -1) {
        for (int i = 0; i < (int)current->values.size(); i++) {
            if (i == exist) {
                continue;
            }

            draw({ current->values[i] }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        }
        drawChangeColor({ current->values[exist] }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeFocus3, Color::nodeTextFocus1, Color::nodeTextFocus1);
        drawCodeChangeLine(2, 0);
    } else {
        for (auto gnode : current->values) {
            draw({ gnode }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        }
        drawCodeChangeLine(2, 1);
    }

    addNewStep();
    for (int i = 0; i < (int)path.size() - 1; ++i) {
        for (int j = 0; j < (int)path[i]->children.size(); ++j) {
            if (path[i]->children[j] != path[i + 1]) {
                drawAllEdge(path[i]->children[j]);
                drawEdgeFree({ std::make_pair(path[i]->getLowerPoint(j), path[i]->children[j]->getUpperPoint()) }, Color::edge);
                drawAllNode(path[i]->children[j]);
            }
        }

        drawEdgeFreeChangeColor({ std::make_pair(path[i]->getLowerPoint(getChildIndex(path[i], value)), path[i + 1]->getUpperPoint()) }, Color::edgeFocus, Color::edge);

        for (auto gnode : path[i]->values) {
            drawChangeColor({ gnode }, Shape::square, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
        }
    }
    drawAllEdge(current);
    drawAllNode(current);
    if (exist != -1) {
        for (int i = 0; i < (int)current->values.size(); i++) {
            if (i == exist) {
                continue;
            }

            drawFadeOut({ current->values[i] }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        }
        drawFadeOut({ current->values[exist] }, Shape::square, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
        drawCodeFadeOut(0);
    } else {
        for (auto gnode : current->values) {
            drawFadeOut({ gnode }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        }
        drawCodeFadeOut(1);
    }
}

void BTreeVisualizer::insert(int value) {
    mCode.update({
        "if node is full: split node",
        "if node.find(v): return",
        "if node is leaf: insert v to node, return",
        "node = node.next(v)"
    });

    if (mRoot == nullptr) {
        mRoot = new Node();
        mRoot->values.push_back(new GraphicNode(std::to_string(value)));
        setPositions();

        addNewStep();
        drawFadeIn({ mRoot->values.front() }, Shape::square, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
        drawCodeFadeIn(2);
        
        addNewStep();
        draw({ mRoot->values.front() }, Shape::square, Type::hollow, Color::node, Color::nodeText);
        drawFadeOut({ mRoot->values.front() }, Shape::square, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
        drawCodeFadeOut(2);

        return;
    }

    int lastCodeLine = -1;
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

        addNewStep();
        drawReformat();
        drawAllNode(mRoot);
        drawEdgeChangePositionSlideIn(mRoot, 0, Color::edge);
        drawEdgeChangePositionSlideIn(mRoot, 1, Color::edge);
        drawAllEdgeChangePosition(mRoot->children[0]);
        drawAllEdgeChangePosition(mRoot->children[1]);
        drawCodeFadeIn(0);
        lastCodeLine = 0;
    }

    std::vector<Node*> path;
    Node* current = mRoot;
    path.push_back(current);
    while (true) {
        addNewStep();
        if (current == mRoot) {
            drawReformat();
        }
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

        if (lastCodeLine == -1) {
            drawCodeFadeIn(3);
        } else {
            drawCodeChangeLine(lastCodeLine, 3);
        }
        lastCodeLine = 3;

        int childIndex = getChildIndex(current, value);
        if (childIndex == -1) {
            int valueIndex = -1;
            for (int i = 0; i < (int)current->values.size(); ++i) {
                if (value == std::stoi(current->values[i]->getValue())) {
                    valueIndex = i;
                    break;
                }
            }

            addNewStep();
            for (int i = 0; i < (int)path.size() - 1; ++i) {
                for (int j = 0; j < (int)path[i]->children.size(); ++j) {
                    if (path[i]->children[j] != path[i + 1]) {
                        drawAllEdge(path[i]->children[j]);
                        drawEdgeFree({ std::make_pair(path[i]->getLowerPoint(j), path[i]->children[j]->getUpperPoint()) }, Color::edge);
                        drawAllNode(path[i]->children[j]);
                    }
                }

                drawEdgeFree({ std::make_pair(path[i]->getLowerPoint(getChildIndex(path[i], value)), path[i + 1]->getUpperPoint()) }, Color::edgeFocus);

                for (auto value : path[i]->values) {
                    draw({ value }, Shape::square, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                }
            }
            drawAllEdge(current);
            drawAllNodeExceptRoot(current);
            for (int i = 0; i < (int)current->values.size(); ++i) {
                if (i != valueIndex) {
                    draw({ current->values[i] }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                }
            }
            drawChangeColor({ current->values[valueIndex] }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeFocus3, Color::nodeTextFocus1, Color::nodeTextFocus1);
            drawCodeChangeLine(lastCodeLine, 1);
            lastCodeLine = 1;

            break;
        }

        if (current->children.empty()) {
            sf::Vector2f edgeOldPosition1 = (int)path.size() >= 2 ? path[path.size() - 2]->getLowerPoint(getChildIndex(path[path.size() - 2], value)) : sf::Vector2f(0.f, 0.f);
            sf::Vector2f edgeOldPosition2 = path.back()->getUpperPoint();

            current->values.insert(current->values.begin() + childIndex, new GraphicNode(std::to_string(value)));

            addNewStep();
            drawReformat();
            for (int i = 0; i < (int)path.size() - 1; ++i) {
                for (int j = 0; j < (int)path[i]->children.size(); ++j) {
                    if (path[i]->children[j] != path[i + 1]) {
                        drawAllEdgeChangePosition(path[i]->children[j]);
                        drawEdgeChangePosition(path[i], j, Color::edge);
                        drawAllNode(path[i]->children[j]);
                    }
                }

                if (i < (int)path.size() - 2) {
                    drawEdgeChangePosition(path[i], getChildIndex(path[i], value), Color::edgeFocus);
                } else {
                    sf::Vector2f edgeNewPosition1 = path[i]->getLowerPoint(getChildIndex(path[i], value));
                    sf::Vector2f edgeNewPosition2 = path[i + 1]->getUpperPoint();
                    drawEdgeFreeChangePosition({ std::make_pair(edgeOldPosition1, edgeOldPosition2) }, { std::make_pair(edgeNewPosition1, edgeNewPosition2) }, Color::edgeFocus);
                }

                for (auto value : path[i]->values) {
                    draw({ value }, Shape::square, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                }
            }
            drawAllEdge(current);
            for (int i = 0; i < (int)current->values.size(); ++i) {
                if (i != childIndex) {
                    draw({ current->values[i] }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                }
            }
            drawFadeIn({ current->values[childIndex] }, Shape::square, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
            drawAllNodeExceptRoot(current);
            drawCodeChangeLine(lastCodeLine, 2);
            lastCodeLine = 2;

            break;
        }

        if ((int)current->children[childIndex]->values.size() == 3) {
            sf::Vector2f edgeSlideOutPosition1 = current->getLowerPoint(childIndex);
            sf::Vector2f edgeSlideOutPosition2 = current->children[childIndex]->getUpperPoint();
            sf::Vector2f edgeChangeOldPosition1 = (int)path.size() >= 2 ? path[path.size() - 2]->getLowerPoint(getChildIndex(path[path.size() - 2], value)) : sf::Vector2f(0.f, 0.f);
            sf::Vector2f edgeChangeOldPosition2 = path.back()->getUpperPoint();
            std::vector<sf::Vector2f> currentLowerPoint;
            for (int i = 0; i < (int)current->children.size(); ++i) {
                currentLowerPoint.push_back(current->getLowerPoint(i));
            }
            std::vector<sf::Vector2f> childUpperPoint;
            for (int i = 0; i < (int)current->children.size(); ++i) {
                childUpperPoint.push_back(current->children[i]->getUpperPoint());
            }

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
            for (int i = 0; i < (int)path.size() - 1; ++i) {
                for (int j = 0; j < (int)path[i]->children.size(); ++j) {
                    if (path[i]->children[j] != path[i + 1]) {
                        drawAllEdgeChangePosition(path[i]->children[j]);
                        drawEdgeChangePosition(path[i], j, Color::edge);
                        drawAllNode(path[i]->children[j]);
                    }
                }

                if (i < (int)path.size() - 2) {
                    drawEdgeChangePosition(path[i], getChildIndex(path[i], value), Color::edgeFocus);
                } else {
                    sf::Vector2f edgeChangeNewPosition1 = path[i]->getLowerPoint(getChildIndex(path[i], value));
                    sf::Vector2f edgeChangeNewPosition2 = path[i + 1]->getUpperPoint();
                    drawEdgeFreeChangePosition({ std::make_pair(edgeChangeOldPosition1, edgeChangeOldPosition2) }, { std::make_pair(edgeChangeNewPosition1, edgeChangeNewPosition2) }, Color::edgeFocus);
                }

                for (auto value : path[i]->values) {
                    draw({ value }, Shape::square, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                }
            }
            drawEdgeFreeSlideOut({ std::make_pair(edgeSlideOutPosition1, edgeSlideOutPosition2) }, Color::edge);
            drawEdgeFreeSlideIn({ std::make_pair(current->getLowerPoint(childIndex), current->children[childIndex]->getUpperPoint()) }, Color::edge);
            drawEdgeFreeSlideIn({ std::make_pair(current->getLowerPoint(childIndex + 1), current->children[childIndex + 1]->getUpperPoint()) }, Color::edge);
            for (int i = 0; i < (int)current->children.size(); ++i) {
                drawAllEdgeChangePosition(current->children[i]);

                if (i == childIndex || i == childIndex + 1) {
                    continue;
                }

                if (i < childIndex) {
                    drawEdgeFreeChangePosition({ std::make_pair(currentLowerPoint[i], childUpperPoint[i]) }, { std::make_pair(current->getLowerPoint(i), current->children[i]->getUpperPoint()) }, Color::edge);
                } else {
                    drawEdgeFreeChangePosition({ std::make_pair(currentLowerPoint[i - 1], childUpperPoint[i - 1]) }, { std::make_pair(current->getLowerPoint(i), current->children[i]->getUpperPoint()) }, Color::edge);
                }
            }

            for (int i = 0; i < (int)current->values.size(); ++i) {
                if (i == childIndex) {
                    continue;
                }

                draw({ current->values[i] }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            }
            draw({ current->values[childIndex] }, Shape::square, Type::hollow, Color::node, Color::nodeText);
            drawFadeIn({ current->values[childIndex] }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            drawAllNodeExceptRoot(current);
            drawCodeChangeLine(lastCodeLine, 0);
            lastCodeLine = 0;

            childIndex = getChildIndex(current, value);
        }

        current = current->children[childIndex];
        path.push_back(current);
    }

    addNewStep();
    for (int i = 0; i < (int)path.size() - 1; ++i) {
        for (int j = 0; j < (int)path[i]->children.size(); ++j) {
            if (path[i]->children[j] != path[i + 1]) {
                drawAllEdge(path[i]->children[j]);
                drawEdgeFree({ std::make_pair(path[i]->getLowerPoint(j), path[i]->children[j]->getUpperPoint()) }, Color::edge);
                drawAllNode(path[i]->children[j]);
            }
        }

        drawEdgeFreeChangeColor({ std::make_pair(path[i]->getLowerPoint(getChildIndex(path[i], value)), path[i + 1]->getUpperPoint()) }, Color::edgeFocus, Color::edge);

        for (auto value : path[i]->values) {
            drawChangeColor({ value }, Shape::square, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
        }
    }
    drawAllEdge(current);
    drawAllNode(current);
    int valueIndex = -1;
    for (int i = 0; i < (int)current->values.size(); ++i) {
        if (std::stoi(current->values[i]->getValue()) != value) {
            drawFadeOut({ current->values[i] }, Shape::square, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        } else {
            valueIndex = i;
        }
    }
    drawFadeOut({ current->values[valueIndex] }, Shape::square, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
    drawCodeFadeOut(lastCodeLine);
}

void BTreeVisualizer::run() {
    std::function<bool()> conditionNone = [&]() { return true; };
    std::function<bool()> conditionSizeNotZero = [&]() { return getSize() != 0; };
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

    mOption.addOption("Search");
    mOption.addSuboption("Value", conditionSizeNotZero);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, BTreeVisualizerData::minValue, BTreeVisualizerData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), BTreeVisualizerData::minValue, BTreeVisualizerData::maxValue)
    );

    mOption.addOption("Insert");
    mOption.addSuboption("Value", conditionSizeNotLarge);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, BTreeVisualizerData::minValue, BTreeVisualizerData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), BTreeVisualizerData::minValue, BTreeVisualizerData::maxValue)
    );

    mOption.addOption("Erase");
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

                case 1: // Search
                    switch (option.second) {
                    case 0: // Value
                        search(std::stoi(values[0]));
                        break;
                    }
                    
                    break;

                case 2: // Insert
                    switch (option.second) {
                    case 0: // Value
                        insert(std::stoi(values[0]));
                        break;
                    }
                    
                    break;

                case 3: // Erase
                    switch (option.second) {
                    case 0: // Value
                        
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