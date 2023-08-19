#include "BinaryHeapVisualizer.hpp"

BinaryHeapVisualizer::BinaryHeapVisualizer() : BinaryTreeVisualizer() {
}

BinaryHeapVisualizer::BinaryHeapVisualizer(sf::RenderWindow* window, AssetsHolder* assets) : BinaryTreeVisualizer(window, assets) {
    mHeapType = BinaryHeapVisualizerData::MaxHeap;
    mErasedNode = nullptr;
}

bool BinaryHeapVisualizer::compare(int value1, int value2) {
    if (value1 == value2) {
        return false;
    }
    
    if (mHeapType == BinaryHeapVisualizerData::HeapType::MaxHeap) {
        return value1 > value2;
    } else {
        return value1 < value2;
    }
}

void BinaryHeapVisualizer::setBinaryTree() {
    if (mNodes.empty()) {
        mRoot = nullptr;
        return;
    }

    mRoot = mNodes.front();
    mRoot->left = mRoot->right = nullptr;
    for (int i = 1; i < (int)mNodes.size(); i++) {
        mNodes[i]->left = mNodes[i]->right = nullptr;
        if (i % 2 == 1) {
            mNodes[(i - 1) / 2]->left = mNodes[i];
        } else {
            mNodes[(i - 1) / 2]->right = mNodes[i];
        }
    }
}

void BinaryHeapVisualizer::drawAllLabel(Node* node, int index) {
    if (node == nullptr) {
        return;
    }

    drawLabel({ &node->value }, { std::to_string(index) }, Color::label);

    drawAllLabel(node->left, 2 * index + 1);
    drawAllLabel(node->right, 2 * index + 2);
}

void BinaryHeapVisualizer::drawAllLabelFadeIn(Node* node, int index) {
    if (node == nullptr) {
        return;
    }

    drawLabelFadeIn({ &node->value }, { std::to_string(index) }, Color::label);

    drawAllLabelFadeIn(node->left, 2 * index + 1);
    drawAllLabelFadeIn(node->right, 2 * index + 2); 
}

void BinaryHeapVisualizer::drawAllLabelFixed(Node* node, int index) {
    if (node == nullptr) {
        return;
    }

    drawLabelFixed({ &node->value }, { std::to_string(index) }, Color::label);

    drawAllLabelFixed(node->left, 2 * index + 1);
    drawAllLabelFixed(node->right, 2 * index + 2);
}

void BinaryHeapVisualizer::freeMemory() {
    if (mErasedNode != nullptr) {
        delete mErasedNode;
        mErasedNode = nullptr;
    }
}

void BinaryHeapVisualizer::create(int size) {
    while (!mNodes.empty()) {
        delete mNodes.back();
        mNodes.pop_back();
    }
    
    for (int i = 0; i < size; i++) {
        int value = Randomizer::random(BinaryHeapVisualizerData::minValue, BinaryHeapVisualizerData::maxValue);
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

void BinaryHeapVisualizer::getTop() {
    mCode.update({
        "return a[0] // which is " + mNodes.front()->value.getValue(),
    });

    addNewStep();
    drawAllNode(mRoot);
    drawFadeIn({ &mNodes.front()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    drawAllEdge(mRoot);
    drawAllLabel(mRoot, 0);
    drawCodeFadeIn(0);

    addNewStep();
    drawAllNode(mRoot);
    drawFadeOut({ &mNodes.front()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    drawAllEdge(mRoot);
    drawAllLabel(mRoot, 0);
    drawCodeFadeOut(0);
}

void BinaryHeapVisualizer::getSize() {
    mCode.update({
        "return a.size() // which is " + std::to_string(size()),
    });

    addNewStep();
    drawAllNode(mRoot);
    drawAllEdge(mRoot);
    drawAllLabel(mRoot, 0);
    drawCodeFadeIn(0);

    addNewStep();
    drawAllNode(mRoot);
    drawAllEdge(mRoot);
    drawAllLabel(mRoot, 0);
    drawCodeFadeOut(0);
}

void BinaryHeapVisualizer::insert(int value) {
    if (mHeapType == BinaryHeapVisualizerData::HeapType::MaxHeap) {
        mCode.update({
            "a.append(v)",
            "i = len(a) - 1",
            "while i > 0 and a[i] > a[par(i)]:",
            "    swap(a[i], a[par(i)]), i = par(i)",
        });
    } else {
        mCode.update({
            "a.append(v)",
            "i = len(a) - 1",
            "while i > 0 and a[i] < a[par(i)]:",
            "    swap(a[i], a[par(i)]), i = par(i)",
        });
    }

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
                drawLabelChangeName({ &pathNode[i]->value }, { std::to_string(pathIndex[i + 1]) }, { std::to_string(pathIndex[i]) }, Color::label);
                drawLabelChangeName({ &pathNode[i + 1]->value }, { std::to_string(pathIndex[i]) }, { std::to_string(pathIndex[i + 1]) }, Color::label);
            } else {
                draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
                drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);
                drawLabel({ &pathNode[i]->value }, { std::to_string(pathIndex[i]) }, Color::label);
            }

            if (pathNode[i]->right == pathNode[i + 1]) {
                drawAllNode(pathNode[i]->left);
                if (pathNode[i]->left) {
                    drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->left->value) }, Color::edge);
                }
                drawAllEdge(pathNode[i]->left);
                drawAllLabel(pathNode[i]->left, 2 * pathIndex[i] + 1);
            } else {
                drawAllNode(pathNode[i]->right);
                if (pathNode[i]->right) {
                    drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->right->value) }, Color::edge);
                }
                drawAllEdge(pathNode[i]->right);
                drawAllLabel(pathNode[i]->right, 2 * pathIndex[i] + 2);
            }
        }
        draw({ &pathNode.back()->value, }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
        drawAllNode(pathNode.back()->left);
        drawAllNode(pathNode.back()->right);
        drawAllEdgeFixed(pathNode.back());
        drawAllLabel(pathNode.back()->left, 2 * pathIndex.back() + 1);
        drawAllLabel(pathNode.back()->right, 2 * pathIndex.back() + 2);
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

void BinaryHeapVisualizer::eraseTop() {
    if (mHeapType == BinaryHeapVisualizerData::HeapType::MaxHeap) {
        mCode.update({
            "swap(a[0], a.top()), a.pop(), i = 0",
            "while a[i] > a[maxchild(i)]:",
            "    swap(a[i], a[maxchild(i)]), i = maxchild(i)",
        });
    } else {
        mCode.update({
            "swap(a[0], a.top()), a.pop(), i = 0",
            "while a[i] < a[minchild(i)]:",
            "    swap(a[i], a[minchild(i)]), i = minchild(i)",
        });
    }

    if (mNodes.size() == 1) {
        addNewStep();
        draw({ &mNodes.front()->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
        drawFadeIn({ &mNodes.front()->value }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
        drawLabel({ &mNodes.front()->value }, { std::to_string(0) }, Color::label);
        drawCodeFadeIn(0);

        mErasedNode = mNodes.front();
        mNodes.pop_back();
        setBinaryTree();

        addNewStep();
        drawFadeOut({ &mErasedNode->value }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
        drawLabelFadeOut({ &mErasedNode->value }, { std::to_string(0) }, Color::label);
        drawCodeChangeLine(0, 0);

        addNewStep();
        drawCodeChangeLine(0, 1);

        addNewStep();
        drawCodeFadeOut(1);

        return;
    }

    std::vector<int> pathIndex;
    std::vector<Node*> pathNode;
    int erasedIndex = (int)mNodes.size() - 1;
    for (int i = erasedIndex; ; i = (i - 1) / 2) {
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
    if (pathIndex.back() == 2 * pathIndex[(int)pathIndex.size() - 2] + 1) {
        currentNode->left = mNodes.back();
    } else {
        currentNode->right = mNodes.back();
    }
    pathNode.push_back(mNodes.back());

    addNewStep();
    drawReformat();
    drawAllNode(mRoot);
    drawFadeIn({ &mNodes.front()->value }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
    drawFadeIn({ &mNodes.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    drawAllEdge(mRoot);
    drawAllLabel(mRoot, 0);
    drawCodeFadeIn(0);

    std::swap(mNodes.front(), mNodes.back());
    std::swap(pathNode.front(), pathNode.back());
    setBinaryTree();

    addNewStep();
    drawReformat();
    for (int i = 0; i < (int)pathNode.size() - 1; i++) {
        if (i == 0) {
            draw({ &pathNode[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            drawLabelChangeName({ &pathNode[i]->value }, { std::to_string(pathIndex.back()) }, { std::to_string(pathIndex.front()) }, Color::label);
        } else {
            draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
            drawLabel({ &pathNode[i]->value }, { std::to_string(pathIndex[i]) }, Color::label);
        }

        drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);

        if (pathNode[i]->right == pathNode[i + 1]) {
            drawAllNode(pathNode[i]->left);
            if (pathNode[i]->left) {
                drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->left->value) }, Color::edge);
            }
            drawAllEdge(pathNode[i]->left);
            drawAllLabel(pathNode[i]->left, 2 * pathIndex[i] + 1);
        } else {
            drawAllNode(pathNode[i]->right);
            if (pathNode[i]->right) {
                drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->right->value) }, Color::edge);
            }
            drawAllEdge(pathNode[i]->right);
            drawAllLabel(pathNode[i]->right, 2 * pathIndex[i] + 2);
        }
    }
    draw({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
    drawLabelChangeName({ &pathNode.back()->value }, { std::to_string(pathIndex.front()) }, { std::to_string(pathIndex.back()) }, Color::label);
    drawCodeChangeLine(0, 0);

    mErasedNode = mNodes.back();
    mNodes.pop_back();
    pathNode.pop_back();
    int mEraseIndex = pathIndex.back();
    pathIndex.pop_back();
    setBinaryTree();

    addNewStep();
    drawReformat();
    for (int i = 0; i < (int)pathNode.size() - 1; i++) {
        if (i == 0) {
            draw({ &pathNode[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        } else {
            draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
        }
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
    if (pathNode.size() == 1) {
        draw({ &pathNode.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        drawAllNode(pathNode.back()->left);
        drawAllNode(pathNode.back()->right);
    } else {
        drawAllNode(pathNode.back());
    }
    drawAllEdge(pathNode.back());
    drawFadeOut({ &mErasedNode->value, }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
    drawLabelFadeOut({ &mErasedNode->value }, { std::to_string(mEraseIndex) }, Color::label);
    drawEdgeSlideOut({ std::make_pair(&pathNode.back()->value, &mErasedNode->value) }, Color::edge);
    drawAllLabel(mRoot, 0);
    drawCodeChangeLine(0, 0);

    pathNode.clear();
    pathIndex.clear();
    pathNode.push_back(mRoot);
    pathIndex.push_back(0);
    std::pair<int, int> maxChild;
    while (true) {
        maxChild = std::make_pair(-1, -1);
        if (pathNode.back()->left) {
            if (maxChild.second == -1 || compare(std::stoi(pathNode.back()->left->value.getValue()), maxChild.first)) {
                maxChild = std::make_pair(std::stoi(pathNode.back()->left->value.getValue()), 2 * pathIndex.back() + 1);
            }
        }

        if (pathNode.back()->right) {
            if (maxChild.second == -1 || compare(std::stoi(pathNode.back()->right->value.getValue()), maxChild.first)) {
                maxChild = std::make_pair(std::stoi(pathNode.back()->right->value.getValue()), 2 * pathIndex.back() + 2);
            }
        }

        addNewStep();
        for (int i = 0; i < (int)pathNode.size() - 1; i++) {
            drawEdge({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);
            if (i == (int)pathNode.size() - 2) {
                drawChangeColor({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);    
                drawEdgeSlideOut({ std::make_pair(&pathNode[i + 1]->value, &pathNode[i]->value) }, Color::edgeFocus);
            } else {
                draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
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
        draw({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        if (maxChild.second == 2 * pathIndex.back() + 1) {
            drawAllNode(pathNode.back()->right);
            drawAllEdge(pathNode.back()->right);
            if (pathNode.back()->right) {
                drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->right->value) }, Color::edge);
            }
            drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->left->value) }, Color::edge);
            drawEdgeSlideIn({ std::make_pair(&pathNode.back()->value, &pathNode.back()->left->value) }, Color::edgeFocus);
            drawChangeColor({ &pathNode.back()->left->value, }, Shape::circle, Type::hollow, Color::node, Color::nodeFocus1, Color::nodeText, Color::nodeTextFocus2);
            drawAllNode(pathNode.back()->left->left);
            drawAllNode(pathNode.back()->left->right);
            drawAllEdge(pathNode.back()->left);
        } else if (maxChild.second == 2 * pathIndex.back() + 2) {
            drawAllNode(pathNode.back()->left);
            drawAllEdge(pathNode.back()->left);
            if (pathNode.back()->left) {
                drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->left->value) }, Color::edge);
            }
            drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->right->value) }, Color::edge);
            drawEdgeSlideIn({ std::make_pair(&pathNode.back()->value, &pathNode.back()->right->value) }, Color::edgeFocus);
            drawChangeColor({ &pathNode.back()->right->value, }, Shape::circle, Type::hollow, Color::node, Color::nodeFocus1, Color::nodeText, Color::nodeTextFocus2);
            drawAllNode(pathNode.back()->right->left);
            drawAllNode(pathNode.back()->right->right);
            drawAllEdge(pathNode.back()->right);
        } else {
            drawAllNode(pathNode.back()->left);
            drawAllNode(pathNode.back()->right);
            drawAllEdge(pathNode.back()->left);
            drawAllEdge(pathNode.back()->right);
        }

        drawAllLabel(mRoot, 0);
        if (pathNode.size() == 1) {
            drawCodeChangeLine(0, 1);
        } else {
            drawCodeChangeLine(2, 1);
        }

        if (maxChild.second == -1 || !compare(maxChild.first, std::stoi(pathNode.back()->value.getValue()))) {
            break;
        }

        if (maxChild.second == 2 * pathIndex.back() + 1) {
            pathNode.push_back(pathNode.back()->left);
            pathIndex.push_back(2 * pathIndex.back() + 1);
            std::swap(mNodes[pathIndex.back()], mNodes[pathIndex[(int)pathIndex.size() - 2]]);
            std::swap(pathNode[pathNode.size() - 2], pathNode.back());
        } else if (maxChild.second == 2 * pathIndex.back() + 2) {
            pathNode.push_back(pathNode.back()->right);
            pathIndex.push_back(2 * pathIndex.back() + 2);
            std::swap(mNodes[pathIndex.back()], mNodes[pathIndex[(int)pathIndex.size() - 2]]);
            std::swap(pathNode[pathNode.size() - 2], pathNode.back());
        } else {
            break;
        }

        setBinaryTree();

        addNewStep();
        drawReformat();
        for (int i = 0; i < (int)pathNode.size() - 1; i++) {
            if (i == (int)pathNode.size() - 2) {
                draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                drawEdge({ std::make_pair(&pathNode[i + 1]->value, &pathNode[i]->value) }, Color::edgeFocus);
                drawLabelChangeName({ &pathNode[i]->value }, { std::to_string(pathIndex[i + 1]) }, { std::to_string(pathIndex[i]) }, Color::label);
            } else {
                draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
                drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);
                drawLabel({ &pathNode[i]->value }, { std::to_string(pathIndex[i]) }, Color::label);
            }
            
            if (pathNode[i]->right == pathNode[i + 1]) {
                drawAllNode(pathNode[i]->left);
                if (pathNode[i]->left) {
                    drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->left->value) }, Color::edge);
                }
                drawAllEdge(pathNode[i]->left);
                drawAllLabel(pathNode[i]->left, 2 * pathIndex[i] + 1);
            } else {
                drawAllNode(pathNode[i]->right);
                if (pathNode[i]->right) {
                    drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->right->value) }, Color::edge);
                }
                drawAllEdge(pathNode[i]->right);
                drawAllLabel(pathNode[i]->right, 2 * pathIndex[i] + 2);
            }
        }
        draw({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        drawLabelChangeName({ &pathNode.back()->value }, { std::to_string(pathIndex[(int)pathIndex.size() - 2]) }, { std::to_string(pathIndex.back()) }, Color::label);
        drawAllNode(pathNode.back()->left);
        drawAllNode(pathNode.back()->right);
        drawAllEdgeFixed(pathNode.back());
        drawAllLabel(pathNode.back()->left, 2 * pathIndex.back() + 1);
        drawAllLabel(pathNode.back()->right, 2 * pathIndex.back() + 2);
        drawCodeChangeLine(1, 2);
    }

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
    draw({ &pathNode.back()->value, }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
    drawFadeOut({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    if (maxChild.second == 2 * pathIndex.back() + 1) {
        drawAllNode(pathNode.back()->right);
        drawAllEdge(pathNode.back()->right);
        if (pathNode.back()->right) {
            drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->right->value) }, Color::edge);
        }
        drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->left->value) }, Color::edge);
        drawEdgeChangeColor({ std::make_pair(&pathNode.back()->value, &pathNode.back()->left->value) }, Color::edgeFocus, Color::edge);
        drawChangeColor({ &pathNode.back()->left->value, }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
        drawAllNode(pathNode.back()->left->left);
        drawAllNode(pathNode.back()->left->right);
        drawAllEdge(pathNode.back()->left);
    } else if (maxChild.second == 2 * pathIndex.back() + 2) {
        drawAllNode(pathNode.back()->left);
        drawAllEdge(pathNode.back()->left);
        if (pathNode.back()->left) {
            drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->left->value) }, Color::edge);
        }
        drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->right->value) }, Color::edge);
        drawEdgeChangeColor({ std::make_pair(&pathNode.back()->value, &pathNode.back()->right->value) }, Color::edgeFocus, Color::edge);
        drawChangeColor({ &pathNode.back()->right->value, }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
        drawAllNode(pathNode.back()->right->left);
        drawAllNode(pathNode.back()->right->right);
        drawAllEdge(pathNode.back()->right);
    } else {
        drawAllNode(pathNode.back()->left);
        drawAllNode(pathNode.back()->right);
        drawAllEdge(pathNode.back()->left);
        drawAllEdge(pathNode.back()->right);
    }
    drawAllLabel(mRoot, 0);
    drawCodeFadeOut(1);
}

void BinaryHeapVisualizer::erase(int index) {
    mCode.update({
        "a[i] = a[0] + 1, shiftTop(i)",
        "eraseTop()"
    });

    std::vector<int> pathIndex;
    std::vector<Node*> pathNode;
    int insertedIndex = index;
    for (int i = insertedIndex; ; i = (i - 1) / 2) {
        pathIndex.push_back(i);
        if (i == 0) {
            break;
        }
    }
    
    std::reverse(pathIndex.begin(), pathIndex.end());

    Node* currentNode = mRoot;
    for (int i = 1; i < (int)pathIndex.size(); i++) {
        pathNode.push_back(currentNode);
        if (pathIndex[i] == 2 * pathIndex[i - 1] + 1) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
    }
    pathNode.push_back(currentNode);

    assert(pathIndex.size() == pathNode.size());

    addNewStep();
    drawReformat();
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
    draw({ &pathNode.back()->value, }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
    drawFadeIn({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    drawAllNode(pathNode.back()->left);
    drawAllNode(pathNode.back()->right);
    drawAllEdge(pathNode.back());
    drawAllLabel(mRoot, 0);
    drawCodeFadeIn(0);

    int newNodeValue = std::stoi(mNodes.front()->value.getValue()) + (mHeapType == BinaryHeapVisualizerData::HeapType::MaxHeap ? 1 : -1);

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
    drawChangeValue({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1, { pathNode.back()->value.getValue() }, { std::to_string(newNodeValue) });
    drawAllNode(pathNode.back()->left);
    drawAllNode(pathNode.back()->right);
    drawAllEdge(pathNode.back());
    drawAllLabel(mRoot, 0);
    drawCodeChangeLine(0, 0);

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
        if (lastNode == nullptr) {
            drawAllNode(pathNode.back()->left);
            drawAllNode(pathNode.back()->right);
            drawAllEdge(pathNode.back());
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
        drawCodeChangeLine(0, 0);

        if (!(pathNode.size() >= 2)) {
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
                drawLabelChangeName({ &pathNode[i]->value }, { std::to_string(pathIndex[i + 1]) }, { std::to_string(pathIndex[i]) }, Color::label);
                drawLabelChangeName({ &pathNode[i + 1]->value }, { std::to_string(pathIndex[i]) }, { std::to_string(pathIndex[i + 1]) }, Color::label);
            } else {
                draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
                drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);
                drawLabel({ &pathNode[i]->value }, { std::to_string(pathIndex[i]) }, Color::label);
            }

            if (pathNode[i]->right == pathNode[i + 1]) {
                drawAllNode(pathNode[i]->left);
                if (pathNode[i]->left) {
                    drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->left->value) }, Color::edge);
                }
                drawAllEdge(pathNode[i]->left);
                drawAllLabel(pathNode[i]->left, 2 * pathIndex[i] + 1);
            } else {
                drawAllNode(pathNode[i]->right);
                if (pathNode[i]->right) {
                    drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->right->value) }, Color::edge);
                }
                drawAllEdge(pathNode[i]->right);
                drawAllLabel(pathNode[i]->right, 2 * pathIndex[i] + 2);
            }
        }
        draw({ &pathNode.back()->value, }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
        drawAllNode(pathNode.back()->left);
        drawAllNode(pathNode.back()->right);
        drawAllEdgeFixed(pathNode.back());
        drawAllLabel(pathNode.back()->left, 2 * pathIndex.back() + 1);
        drawAllLabel(pathNode.back()->right, 2 * pathIndex.back() + 2);
        drawCodeChangeLine(0, 0);

        lastNode = pathNode.back();
        pathNode.pop_back();
        pathIndex.pop_back();
    }

    pathIndex.clear();
    pathNode.clear();
    int erasedIndex = (int)mNodes.size() - 1;
    for (int i = erasedIndex; ; i = (i - 1) / 2) {
        pathIndex.push_back(i);
        if (i == 0) {
            break;
        }
    }
    
    std::reverse(pathIndex.begin(), pathIndex.end());

    currentNode = mRoot;
    for (int i = 1; i < (int)pathIndex.size() - 1; i++) {
        pathNode.push_back(currentNode);
        if (pathIndex[i] == 2 * pathIndex[i - 1] + 1) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
    }

    pathNode.push_back(currentNode);
    if (pathIndex.back() == 2 * pathIndex[(int)pathIndex.size() - 2] + 1) {
        currentNode->left = mNodes.back();
    } else {
        currentNode->right = mNodes.back();
    }
    pathNode.push_back(mNodes.back());

    addNewStep();
    drawReformat();
    drawAllNode(mRoot);
    drawChangeColor({ &mNodes.front()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeFocus4, Color::nodeTextFocus1, Color::nodeTextFocus1);
    drawFadeIn({ &mNodes.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    drawAllEdge(mRoot);
    drawAllLabel(mRoot, 0);
    drawCodeChangeLine(0, 1);

    std::swap(mNodes.front(), mNodes.back());
    std::swap(pathNode.front(), pathNode.back());
    setBinaryTree();

    addNewStep();
    drawReformat();
    for (int i = 0; i < (int)pathNode.size() - 1; i++) {
        if (i == 0) {
            draw({ &pathNode[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            drawLabelChangeName({ &pathNode[i]->value }, { std::to_string(pathIndex.back()) }, { std::to_string(pathIndex.front()) }, Color::label);
        } else {
            draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
            drawLabel({ &pathNode[i]->value }, { std::to_string(pathIndex[i]) }, Color::label);
        }

        drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);

        if (pathNode[i]->right == pathNode[i + 1]) {
            drawAllNode(pathNode[i]->left);
            if (pathNode[i]->left) {
                drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->left->value) }, Color::edge);
            }
            drawAllEdge(pathNode[i]->left);
            drawAllLabel(pathNode[i]->left, 2 * pathIndex[i] + 1);
        } else {
            drawAllNode(pathNode[i]->right);
            if (pathNode[i]->right) {
                drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->right->value) }, Color::edge);
            }
            drawAllEdge(pathNode[i]->right);
            drawAllLabel(pathNode[i]->right, 2 * pathIndex[i] + 2);
        }
    }
    draw({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
    drawLabelChangeName({ &pathNode.back()->value }, { std::to_string(pathIndex.front()) }, { std::to_string(pathIndex.back()) }, Color::label);
    drawCodeChangeLine(1, 1);

    mErasedNode = mNodes.back();
    mNodes.pop_back();
    pathNode.pop_back();
    int mEraseIndex = pathIndex.back();
    pathIndex.pop_back();
    setBinaryTree();

    addNewStep();
    drawReformat();
    for (int i = 0; i < (int)pathNode.size() - 1; i++) {
        if (i == 0) {
            draw({ &pathNode[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        } else {
            draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
        }
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
    if (pathNode.size() == 1) {
        draw({ &pathNode.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        drawAllNode(pathNode.back()->left);
        drawAllNode(pathNode.back()->right);
    } else {
        drawAllNode(pathNode.back());
    }
    drawAllEdge(pathNode.back());
    drawFadeOut({ &mErasedNode->value, }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
    drawLabelFadeOut({ &mErasedNode->value }, { std::to_string(mEraseIndex) }, Color::label);
    drawEdgeSlideOut({ std::make_pair(&pathNode.back()->value, &mErasedNode->value) }, Color::edge);
    drawAllLabel(mRoot, 0);
    drawCodeChangeLine(1, 1);

    pathNode.clear();
    pathIndex.clear();
    pathNode.push_back(mRoot);
    pathIndex.push_back(0);
    std::pair<int, int> maxChild;
    while (true) {
        maxChild = std::make_pair(-1, -1);
        if (pathNode.back()->left) {
            if (maxChild.second == -1 || compare(std::stoi(pathNode.back()->left->value.getValue()), maxChild.first)) {
                maxChild = std::make_pair(std::stoi(pathNode.back()->left->value.getValue()), 2 * pathIndex.back() + 1);
            }
        }

        if (pathNode.back()->right) {
            if (maxChild.second == -1 || compare(std::stoi(pathNode.back()->right->value.getValue()), maxChild.first)) {
                maxChild = std::make_pair(std::stoi(pathNode.back()->right->value.getValue()), 2 * pathIndex.back() + 2);
            }
        }

        addNewStep();
        for (int i = 0; i < (int)pathNode.size() - 1; i++) {
            drawEdge({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);
            if (i == (int)pathNode.size() - 2) {
                drawChangeColor({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);    
                drawEdgeSlideOut({ std::make_pair(&pathNode[i + 1]->value, &pathNode[i]->value) }, Color::edgeFocus);
            } else {
                draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
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
        draw({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        if (maxChild.second == 2 * pathIndex.back() + 1) {
            drawAllNode(pathNode.back()->right);
            drawAllEdge(pathNode.back()->right);
            if (pathNode.back()->right) {
                drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->right->value) }, Color::edge);
            }
            drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->left->value) }, Color::edge);
            drawEdgeSlideIn({ std::make_pair(&pathNode.back()->value, &pathNode.back()->left->value) }, Color::edgeFocus);
            drawChangeColor({ &pathNode.back()->left->value, }, Shape::circle, Type::hollow, Color::node, Color::nodeFocus1, Color::nodeText, Color::nodeTextFocus2);
            drawAllNode(pathNode.back()->left->left);
            drawAllNode(pathNode.back()->left->right);
            drawAllEdge(pathNode.back()->left);
        } else if (maxChild.second == 2 * pathIndex.back() + 2) {
            drawAllNode(pathNode.back()->left);
            drawAllEdge(pathNode.back()->left);
            if (pathNode.back()->left) {
                drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->left->value) }, Color::edge);
            }
            drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->right->value) }, Color::edge);
            drawEdgeSlideIn({ std::make_pair(&pathNode.back()->value, &pathNode.back()->right->value) }, Color::edgeFocus);
            drawChangeColor({ &pathNode.back()->right->value, }, Shape::circle, Type::hollow, Color::node, Color::nodeFocus1, Color::nodeText, Color::nodeTextFocus2);
            drawAllNode(pathNode.back()->right->left);
            drawAllNode(pathNode.back()->right->right);
            drawAllEdge(pathNode.back()->right);
        } else {
            drawAllNode(pathNode.back()->left);
            drawAllNode(pathNode.back()->right);
            drawAllEdge(pathNode.back()->left);
            drawAllEdge(pathNode.back()->right);
        }

        drawAllLabel(mRoot, 0);
        drawCodeChangeLine(1, 1);

        if (maxChild.second == -1 || !compare(maxChild.first, std::stoi(pathNode.back()->value.getValue()))) {
            break;
        }

        if (maxChild.second == 2 * pathIndex.back() + 1) {
            pathNode.push_back(pathNode.back()->left);
            pathIndex.push_back(2 * pathIndex.back() + 1);
            std::swap(mNodes[pathIndex.back()], mNodes[pathIndex[(int)pathIndex.size() - 2]]);
            std::swap(pathNode[pathNode.size() - 2], pathNode.back());
        } else if (maxChild.second == 2 * pathIndex.back() + 2) {
            pathNode.push_back(pathNode.back()->right);
            pathIndex.push_back(2 * pathIndex.back() + 2);
            std::swap(mNodes[pathIndex.back()], mNodes[pathIndex[(int)pathIndex.size() - 2]]);
            std::swap(pathNode[pathNode.size() - 2], pathNode.back());
        } else {
            break;
        }

        setBinaryTree();

        addNewStep();
        drawReformat();
        for (int i = 0; i < (int)pathNode.size() - 1; i++) {
            if (i == (int)pathNode.size() - 2) {
                draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                drawEdge({ std::make_pair(&pathNode[i + 1]->value, &pathNode[i]->value) }, Color::edgeFocus);
                drawLabelChangeName({ &pathNode[i]->value }, { std::to_string(pathIndex[i + 1]) }, { std::to_string(pathIndex[i]) }, Color::label);
            } else {
                draw({ &pathNode[i]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
                drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i + 1]->value) }, Color::edge);
                drawLabel({ &pathNode[i]->value }, { std::to_string(pathIndex[i]) }, Color::label);
            }
            
            if (pathNode[i]->right == pathNode[i + 1]) {
                drawAllNode(pathNode[i]->left);
                if (pathNode[i]->left) {
                    drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->left->value) }, Color::edge);
                }
                drawAllEdge(pathNode[i]->left);
                drawAllLabel(pathNode[i]->left, 2 * pathIndex[i] + 1);
            } else {
                drawAllNode(pathNode[i]->right);
                if (pathNode[i]->right) {
                    drawEdgeFixed({ std::make_pair(&pathNode[i]->value, &pathNode[i]->right->value) }, Color::edge);
                }
                drawAllEdge(pathNode[i]->right);
                drawAllLabel(pathNode[i]->right, 2 * pathIndex[i] + 2);
            }
        }
        draw({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        drawLabelChangeName({ &pathNode.back()->value }, { std::to_string(pathIndex[(int)pathIndex.size() - 2]) }, { std::to_string(pathIndex.back()) }, Color::label);
        drawAllNode(pathNode.back()->left);
        drawAllNode(pathNode.back()->right);
        drawAllEdgeFixed(pathNode.back());
        drawAllLabel(pathNode.back()->left, 2 * pathIndex.back() + 1);
        drawAllLabel(pathNode.back()->right, 2 * pathIndex.back() + 2);
        drawCodeChangeLine(1, 1);
    }

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
    draw({ &pathNode.back()->value, }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
    drawFadeOut({ &pathNode.back()->value, }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    if (maxChild.second == 2 * pathIndex.back() + 1) {
        drawAllNode(pathNode.back()->right);
        drawAllEdge(pathNode.back()->right);
        if (pathNode.back()->right) {
            drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->right->value) }, Color::edge);
        }
        drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->left->value) }, Color::edge);
        drawEdgeChangeColor({ std::make_pair(&pathNode.back()->value, &pathNode.back()->left->value) }, Color::edgeFocus, Color::edge);
        drawChangeColor({ &pathNode.back()->left->value, }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
        drawAllNode(pathNode.back()->left->left);
        drawAllNode(pathNode.back()->left->right);
        drawAllEdge(pathNode.back()->left);
    } else if (maxChild.second == 2 * pathIndex.back() + 2) {
        drawAllNode(pathNode.back()->left);
        drawAllEdge(pathNode.back()->left);
        if (pathNode.back()->left) {
            drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->left->value) }, Color::edge);
        }
        drawEdge({ std::make_pair(&pathNode.back()->value, &pathNode.back()->right->value) }, Color::edge);
        drawEdgeChangeColor({ std::make_pair(&pathNode.back()->value, &pathNode.back()->right->value) }, Color::edgeFocus, Color::edge);
        drawChangeColor({ &pathNode.back()->right->value, }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
        drawAllNode(pathNode.back()->right->left);
        drawAllNode(pathNode.back()->right->right);
        drawAllEdge(pathNode.back()->right);
    } else {
        drawAllNode(pathNode.back()->left);
        drawAllNode(pathNode.back()->right);
        drawAllEdge(pathNode.back()->left);
        drawAllEdge(pathNode.back()->right);
    }
    drawAllLabel(mRoot, 0);
    drawCodeFadeOut(1);
}

void BinaryHeapVisualizer::run() {
    std::function<bool()> conditionNone = [&]() { return true; };
    std::function<bool()> conditionTreeNotLarge = [&]() { return mNodes.size() < BinaryHeapVisualizerData::maxSize; };
    std::function<bool()> conditionTreeNotEmpty = [&]() { return mNodes.size() > 0; };

    mOption.addOption("Create");
    mOption.addSuboption("Empty", conditionNone);
    mOption.addSuboption("Random", conditionNone);
    mOption.addSuboptionInputBox("n",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, BinaryHeapVisualizerData::minSize, BinaryHeapVisualizerData::maxSize),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), BinaryHeapVisualizerData::minSize, BinaryHeapVisualizerData::maxSize)
    );

    mOption.addOption("Get");
    mOption.addSuboption("Top", conditionTreeNotEmpty);
    mOption.addSuboption("Size", conditionNone);

    mOption.addOption("Insert");
    mOption.addSuboption("", conditionTreeNotLarge);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, BinaryHeapVisualizerData::minValue, BinaryHeapVisualizerData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), BinaryHeapVisualizerData::minValue, BinaryHeapVisualizerData::maxValue)
    );

    mOption.addOption("Erase");
    mOption.addSuboption("Top", conditionTreeNotEmpty);
    mOption.addSuboption("Index", conditionTreeNotEmpty);
    mOption.addSuboptionInputBox("i",
        std::bind(static_cast<std::string(*)(std::string, std::string, std::function<int()>, std::function<int()>)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, [&]() { return 1; }, [&]() { return (int)mNodes.size() - 1; }),
        std::bind(static_cast<std::string(*)(std::function<int()>, std::function<int()>)>(Randomizer::integerInRange), [&]() { return 1; }, [&]() { return (int)mNodes.size() - 1; })
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

                case 1: // Get
                    switch (option.second) {
                    case 0: // Top
                        getTop();
                        break;

                    case 1: // Size
                        getSize();
                        break;
                    }

                    break;

                case 2: // Insert
                    switch (option.second) {
                    case 0:
                        insert(std::stoi(values[0]));
                        break;
                    }

                    break;

                case 3: // Erase
                    switch (option.second) {
                    case 0: // Top
                        eraseTop();
                        break;

                    case 1: // Index
                        erase(std::stoi(values[0]));
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