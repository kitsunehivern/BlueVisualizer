#include "BinaryHeap.hpp"

BinaryHeap::BinaryHeap() : BinaryTreeVisualizer() {
}

BinaryHeap::BinaryHeap(sf::RenderWindow* window, AssetsHolder* assets) : BinaryTreeVisualizer(window, assets) {
    mHeapType = BinaryHeapData::MaxHeap;
    mErasedNode = nullptr;
}

bool BinaryHeap::compare(int value1, int value2) {
    if (value1 == value2) {
        return false;
    }
    
    if (mHeapType == BinaryHeapData::MaxHeap) {
        return value1 > value2;
    } else {
        return value1 < value2;
    }
}

void BinaryHeap::setBinaryTree() {
    mSize = mNodes.size();
    if (mSize == 0) {
        mRoot = nullptr;
        return;
    }

    mRoot = mNodes.front();
    for (int i = 1; i < size(); i++) {
        mNodes[i]->left = mNodes[i]->right = nullptr;
        if (i % 2 == 1) {
            mNodes[(i - 1) / 2]->left = mNodes[i];
        } else {
            mNodes[(i - 1) / 2]->right = mNodes[i];
        }
    }
}

void BinaryHeap::drawAllLabel(Node* node, int index) {
    if (node == nullptr) {
        return;
    }

    drawLabel({ &node->value }, { std::to_string(index) }, Color::label);

    drawAllLabel(node->left, 2 * index + 1);
    drawAllLabel(node->right, 2 * index + 2);
}

void BinaryHeap::drawAllLabelFadeIn(Node* node, int index) {
    if (node == nullptr) {
        return;
    }

    drawLabelFadeIn({ &node->value }, { std::to_string(index) }, Color::label);

    drawAllLabelFadeIn(node->left, 2 * index + 1);
    drawAllLabelFadeIn(node->right, 2 * index + 2); 
}

void BinaryHeap::drawAllLabelFixed(Node* node, int index) {
    if (node == nullptr) {
        return;
    }

    drawLabelFixed({ &node->value }, { std::to_string(index) }, Color::label);

    drawAllLabelFixed(node->left, 2 * index + 1);
    drawAllLabelFixed(node->right, 2 * index + 2);
}

void BinaryHeap::freeMemory() {
    if (mErasedNode != nullptr) {
        delete mErasedNode;
        mErasedNode = nullptr;
    }
}

void BinaryHeap::create(int size) {
    while (!mNodes.empty()) {
        delete mNodes.back();
        mNodes.pop_back();
    }
    
    for (int i = 0; i < size; i++) {
        int value = Randomizer::random(BinaryHeapData::minValue, BinaryHeapData::maxValue);
        mNodes.push_back(new Node(GraphicNode(std::to_string(value))));
        for (int i = (int)mNodes.size() - 1; i > 0; i = (i - 1) / 2) {
            if (compare(std::stoi(mNodes[i]->value.getValue()), std::stoi(mNodes[(i - 1) / 2]->value.getValue()))) {
                std::swap(mNodes[i], mNodes[(i - 1) / 2]);
            } else {
                break;
            }
        }
    }

    mCode.update({});

    setBinaryTree();
    setPositions();

    addNewStep();
    drawAllNodeFadeIn(mRoot);
    drawAllEdgeFadeIn(mRoot);
    drawAllLabelFadeIn(mRoot, 0);
    drawCode();
}

void BinaryHeap::insert(int value) {
    mCode.update({
        "a.append(v)",
        "i = len(a) - 1",
        "while i > 0 and a[i] > a[par(i)]:",
        "    swap(a[i], a[par(i)]), i = par(i)",
    });

    mNodes.push_back(new Node(GraphicNode(std::to_string(value))));
    std::vector<int> pathIndex;
    std::vector<Node*> pathNode;
    if (mRoot == nullptr) {
        mRoot = mNodes.back();
        pathIndex.push_back(0);
        pathNode.push_back(mNodes.back());
    } else {
        int insertedIndex = (int)mNodes.size() - 1;
        for (int i = insertedIndex; ; i = (i - 1) / 2) {
            pathIndex.push_back(i);
            if (i == 0) {
                break;
            }
        }
        
        std::reverse(pathIndex.begin(), pathIndex.end());

        Node* currentNode = mRoot;
        for (int i = 1; i < (int)pathIndex.size() - 1; i++) {
            pathNode.push_back(currentNode);
            if (pathIndex[i] == 2 * pathIndex[i - 1] + 1) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }
        }

        pathNode.push_back(currentNode);
        if (pathIndex.back() == 2 * pathIndex[pathIndex.size() - 2] + 1) {
            currentNode->left = mNodes.back();
        } else {
            currentNode->right = mNodes.back();
        }
        pathNode.push_back(mNodes.back());
        mSize++;
    }

    addNewStep();
    drawReformat();
    for (int i = 0; i < (int)pathNode.size() - 1; i++) {
        draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
        drawLabel({ &pathNode[i]->value }, { std::to_string(pathIndex[i]) }, Color::label);
        if (i == (int)pathNode.size() - 2) {
            drawEdgeSlideIn({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);
        } else {
            drawEdge({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);
        }

        if (pathNode[i]->right == pathNode[i + 1]) {
            drawAllNode(pathNode[i]->left);
            if (pathNode[i]->left) {
                drawEdge({ std::make_pair(&pathNode[i]->value, &pathNode[i]->left->value) }, Color::edge);
            }
            drawAllEdge(pathNode[i]->left);
            drawAllLabel(pathNode[i]->left, 2 * pathIndex[i] + 1);
        } else {
            drawAllNode(pathNode[i]->right);
            if (pathNode[i]->right) {
                drawEdge({ std::make_pair(&pathNode[i]->value, &pathNode[i]->right->value) }, Color::edge);
            }
            drawAllEdge(pathNode[i]->right);
            drawAllLabel(pathNode[i]->right, 2 * pathIndex[i] + 2);
        }
    }
    drawFadeIn({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
    drawLabelFadeIn({ &pathNode.back()->value }, { std::to_string(pathIndex.back()) }, Color::label);
    drawCodeFadeIn(0);

    addNewStep();
    for (int i = 0; i < (int)pathNode.size() - 1; i++) {
        draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
        drawEdge({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);

        if (pathNode[i]->right == pathNode[i + 1]) {
            drawAllNode(pathNode[i]->left);
            if (pathNode[i]->left) {
                drawEdge({ std::make_pair(&pathNode[i]->value, &pathNode[i]->left->value) }, Color::edge);
            }
            drawAllEdge(pathNode[i]->left);
        } else {
            drawAllNode(pathNode[i]->right);
            if (pathNode[i]->right) {
                drawEdge({ std::make_pair(&pathNode[i]->value, &pathNode[i]->right->value) }, Color::edge);
            }
            drawAllEdge(pathNode[i]->right);
        }
    }
    drawChangeColor({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeFocus1, Color::nodeTextFocus1, Color::nodeTextFocus1);
    drawAllLabel(mRoot, 0);
    drawCodeChangeLine(0, 1);

    Node* lastNode = nullptr;
    while (true) {
        addNewStep();
        drawAllEdge(mRoot);
        for (int i = 0; i < (int)pathNode.size() - 1; i++) {
            if (i == (int)pathNode.size() - 2) {
                drawChangeColor({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeFocus1, Color::nodeText, Color::nodeTextFocus2);
                drawEdgeSlideIn({ std::make_pair(&pathNode[i + 1]->value, &pathNode[i]->value) }, Color::edgeFocus);
            } else {
                draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
            }

            if (pathNode[i]->right == pathNode[i + 1]) {
                drawAllNode(pathNode[i]->left);
            } else {
                drawAllNode(pathNode[i]->right);
            }
        }
        draw({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        if (!pathNode.back()->left && !pathNode.back()->right) {
        } else {
            drawChangeColor({ &lastNode->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
            drawAllNode(lastNode->left);
            drawAllNode(lastNode->right);
            drawEdgeSlideOut({ std::make_pair(&pathNode.back()->value, &lastNode->value) }, Color::edgeFocus);
            if (pathNode.back()->left == lastNode) {
                drawAllNode(pathNode.back()->right);
            } else if (pathNode.back()->right == lastNode) {
                drawAllNode(pathNode.back()->left);
            } else {
                assert(false);
            }
        }
        drawAllLabel(mRoot, 0);
        if (lastNode == nullptr) {
            drawCodeChangeLine(1, 2);
        } else {
            drawCodeChangeLine(3, 2);
        }

        if (!(pathNode.size() >= 2 && compare(std::stoi(pathNode.back()->value.getValue()), std::stoi(pathNode[pathNode.size() - 2]->value.getValue())))) {
            break;
        }
        
        std::swap(mNodes[pathIndex.back()], mNodes[pathIndex[pathIndex.size() - 2]]);
        std::swap(pathNode[pathNode.size() - 2], pathNode.back());

        setBinaryTree();

        addNewStep();
        drawReformat();
        for (int i = 0; i < (int)pathNode.size() - 1; i++) {
            if (i == (int)pathNode.size() - 2) {
                draw({ &pathNode[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                drawEdge({ std::make_pair(&pathNode[i + 1]->value, &pathNode[i]->value) }, Color::edgeFocus);
            } else {
                draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
                drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);
            }

            if (pathNode[i]->right == pathNode[i + 1]) {
                drawAllNode(pathNode[i]->left);
                if (pathNode[i]->left) {
                    drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->left->value) }, Color::edge);
                }
                drawAllEdge(pathNode[i]->left);
            } else {
                drawAllNode(pathNode[i]->right);
                if (pathNode[i]->right) {
                    drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->right->value) }, Color::edge);
                }
                drawAllEdge(pathNode[i]->right);
            }
        }
        draw({ &pathNode.back()->value, }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
        drawAllNode(pathNode.back()->left);
        drawAllNode(pathNode.back()->right);
        drawAllEdgeFixed(pathNode.back());
        drawAllLabelFixed(mRoot, 0);
        drawCodeChangeLine(2, 3);

        lastNode = pathNode.back();
        pathNode.pop_back();
        pathIndex.pop_back();
    }

    addNewStep();
    drawAllEdge(mRoot);
    for (int i = 0; i < (int)pathNode.size() - 1; i++) {
        if (i == (int)pathNode.size() - 2) {
            drawChangeColor({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
            drawEdgeChangeColor({ std::make_pair(&pathNode[i + 1]->value, &pathNode[i]->value) }, Color::edgeFocus, Color::edge);
        } else {
            draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
            drawEdge({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);
        }

        if (pathNode[i]->right == pathNode[i + 1]) {
            drawAllNode(pathNode[i]->left);
            if (pathNode[i]->left) {
                drawEdge({ std::make_pair(&pathNode[i]->value, &pathNode[i]->left->value) }, Color::edge);
            }
            drawAllEdge(pathNode[i]->left);
        } else {
            drawAllNode(pathNode[i]->right);
            if (pathNode[i]->right) {
                drawEdge({ std::make_pair(&pathNode[i]->value, &pathNode[i]->right->value) }, Color::edge);
            }
            drawAllEdge(pathNode[i]->right);
        }
    }
    drawAllNode(pathNode.back());
    drawFadeOut({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    drawAllEdge(pathNode.back());
    drawAllLabel(mRoot, 0);
    drawCodeFadeOut(2);
}

void BinaryHeap::run() {
    std::function<bool()> conditionNone = [&]() { return true; };
    std::function<bool()> conditionTreeNotLarge = [&]() { return size() < BinaryHeapData::maxSize; };
    std::function<bool()> conditionTreeNotEmpty = [&]() { return size() > 0; };

    mOption.addOption("Create");
    mOption.addSuboption("Empty", conditionNone);
    mOption.addSuboption("Random", conditionNone);
    mOption.addSuboptionInputBox("n",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, BinaryHeapData::minSize, BinaryHeapData::maxSize),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), BinaryHeapData::minSize, BinaryHeapData::maxSize)
    );

    mOption.addOption("Insert");
    mOption.addSuboption("", conditionTreeNotLarge);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, BinaryHeapData::minValue, BinaryHeapData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), BinaryHeapData::minValue, BinaryHeapData::maxValue)
    );

    mOption.processOption();

    clearAllSteps();
    create(15);

    while (mWindow->isOpen()) {
        updateState();

        sf::Event event;
        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                freeMemory();
                mWindow->close();
            }

            if (handleEvent(event)) {
                abortAllSteps();
                clearAllSteps();

                freeMemory();

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

                case 1: // Insert
                    switch (option.second) {
                    case 0:
                        insert(std::stoi(values[0]));
                        break;
                    }
                }
            }
        }

        mWindow->clear(sf::Color::White);
        draw();
        mWindow->display();
    }
}