#include "AVLTree.hpp"

AVLTree::AVLTree() {
}

AVLTree::AVLTree(sf::RenderWindow* window, AssetsHolder* assets) : BinaryTreeVisualizer(window, assets) {
    mErasedNode = nullptr;
}

int AVLTree::getBalanceFactor(Node* node) {
    if (node == nullptr) {
        return 0;
    }

    return height(node->left) - height(node->right);
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

void AVLTree::freeMemory() {
    if (mErasedNode) {
        delete mErasedNode;
        mErasedNode = nullptr;
    }
}

void AVLTree::create(int size) {
    clear();
    std::set<int> tempValues;
    while ((int)tempValues.size() < size) {
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
            mRoot = new Node(GraphicNode(std::to_string(value), GraphicNodeData::initialPosition));
        } else if (value < std::stoi(path.back()->value.getValue())) {
            path.back()->left = new Node(GraphicNode(std::to_string(value), GraphicNodeData::initialPosition));
        } else {
            path.back()->right = new Node(GraphicNode(std::to_string(value), GraphicNodeData::initialPosition));
        }

        while (!path.empty()) {
            Node* current = path.back();
            path.pop_back();

            int balanceFactor = getBalanceFactor(current);
            if (balanceFactor < -1) {
                if (value > std::stoi(current->right->value.getValue())) {
                    current = leftRotate(current);
                } else {
                    current->right = rightRotate(current->right);
                    current = leftRotate(current);
                }
            } else if (balanceFactor > 1) {
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
    drawAllNodeFadeIn(mRoot);
    drawAllEdgeFadeIn(mRoot);
    drawCode();
}

void AVLTree::search(int value) {
    mCode.update({
        "if node == nullptr: return NOT_FOUND",
        "if value == node.value: return FOUND",
        "if value < node.value: search left",
        "else: search right",
    });

    if (mRoot == nullptr) {
        addNewStep();
        drawCodeFadeIn(0);
        addNewStep();
        drawCodeFadeOut(0);
        return;
    }

    int lastCase = -1;

    addNewStep();   
    drawAllNode(mRoot);
    drawAllEdge(mRoot);
    if (std::stoi(mRoot->value.getValue()) == value) {
        drawFadeIn({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
        drawCodeFadeIn(1);
        lastCase = 1;
    } else {
        drawFadeIn({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        if (value < std::stoi(mRoot->value.getValue())) {
            drawCodeFadeIn(2);
            lastCase = 2;
        } else {
            drawCodeFadeIn(3);
            lastCase = 3;
        }
    }

    if (std::stoi(mRoot->value.getValue()) == value) {
        addNewStep();
        drawAllNode(mRoot);
        drawFadeOut({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
        drawAllEdge(mRoot);
        drawCodeFadeOut(1);
        return;
    }

    std::vector<Node*> path;
    Node* current = mRoot;
    bool exist = false;
    while (current != nullptr) {
        int currentCase = -1;
        path.push_back(current);
        if (value < std::stoi(current->value.getValue())) {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current == nullptr) {
            currentCase = 0;
        } else if (std::stoi(current->value.getValue()) == value) {
            currentCase = 1;
        } else if (value < std::stoi(current->value.getValue())) {
            currentCase = 2;
        } else {
            currentCase = 3;
        }

        addNewStep();
        for (int i = 0; i < (int)path.size(); ++i) {
            draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            if (i == (int)path.size() - 1) {
                drawFadeOut({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            } else {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
            }

            if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                drawAllNode(path[i]->left);
                if (path[i]->left) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                }
                drawAllEdge(path[i]->left);
            } else {
                drawAllNode(path[i]->right);
                if (path[i]->right) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                }
                drawAllEdge(path[i]->right);
            }
        }

        if (current) {
            drawAllNode(current);
            if (value == std::stoi(current->value.getValue())) {
                drawFadeIn({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
                currentCase = 1;
            } else {
                drawFadeIn({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            }
            drawAllEdge(current);
            drawEdge({ std::make_pair(&path.back()->value, &current->value) }, Color::edge);
            drawEdgeSlideIn({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus);
        }
        drawCodeChangeLine(lastCase, currentCase);
        lastCase = currentCase;

        if (current == nullptr) {
            break;
        } else if (std::stoi(current->value.getValue()) == value) {
            exist = true;
            break;
        }
    }

    addNewStep();
    for (int i = 0; i < (int)path.size(); ++i) {
        drawChangeColor({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
        if (i == (int)path.size() - 1) {
        } else {
            drawEdgeChangeColor({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus, Color::edge);
        }

        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
            drawAllNode(path[i]->left);
            if (path[i]->left) {
                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
            }
            drawAllEdge(path[i]->left);
        } else {
            drawAllNode(path[i]->right);
            if (path[i]->right) {
                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
            }
            drawAllEdge(path[i]->right);
        }
    }

    drawAllNode(current);
    drawAllEdge(current);

    if (exist) {
        drawEdgeChangeColor({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus, Color::edge);
        drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
    } else {
    }

    drawCodeFadeOut(lastCase);
}

void AVLTree::insert(int value) {
    mCode.update({
        "insert v",
        "check balance factor of node and its children",
        "    case LL: rotateR(node)",
        "    case LR: rotateL(node.left), rotateR(node)",
        "    case RR: rotateL(node)",
        "    case RL: rotateR(node.right), rotateL(node)",
    });

    if (mRoot == nullptr) {
        mRoot = new Node(GraphicNode(std::to_string(value), GraphicNodeData::initialPosition));
        setPositions();

        addNewStep();
        drawFadeIn({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
        drawCodeFadeIn(0);

        addNewStep();
        draw({ &mRoot->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
        drawFadeOut({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
        drawCodeFadeOut(0);

        return;
    }

    addNewStep();
    drawReformat();
    drawAllNode(mRoot);
    if (std::stoi(mRoot->value.getValue()) == value) {
        drawFadeIn({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
    } else {
        drawFadeIn({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    }
    drawAllEdge(mRoot);
    drawCodeFadeIn(0);

    if (std::stoi(mRoot->value.getValue()) == value) {
        addNewStep();
        drawAllNode(mRoot);
        drawFadeOut({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
        drawAllEdge(mRoot);
        drawCodeFadeOut(0);
        return;
    }

    std::vector<Node*> path;
    Node* current = mRoot;
    bool alreadyExist = false;
    while (current != nullptr) {
        path.push_back(current);
        if (value < std::stoi(current->value.getValue())) {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current == nullptr) {
            break;
        }

        addNewStep();
        for (int i = 0; i < (int)path.size(); ++i) {
            draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            if (i == (int)path.size() - 1) {
                drawFadeOut({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            } else {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
            }

            if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                drawAllNode(path[i]->left);
                if (path[i]->left) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                }
                drawAllEdge(path[i]->left);
            } else {
                drawAllNode(path[i]->right);
                if (path[i]->right) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                }
                drawAllEdge(path[i]->right);
            }
        }

        drawAllNode(current);
        if (value == std::stoi(current->value.getValue())) {
            drawFadeIn({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
        } else {
            drawFadeIn({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        }
        drawAllEdge(current);
        drawEdge({ std::make_pair(&path.back()->value, &current->value) }, Color::edge);
        drawEdgeSlideIn({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus);
        drawCodeChangeLine(0, 0);

        if (value == std::stoi(current->value.getValue())) {
            alreadyExist = true;
            break;
        }
    }

    if (alreadyExist) {
        addNewStep();
        for (int i = 0; i < (int)path.size(); ++i) {
            draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            if (i == (int)path.size() - 1) {
                drawFadeIn({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            } else {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
            }

            if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                drawAllNode(path[i]->left);
                if (path[i]->left) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                }
                drawAllEdge(path[i]->left);
            } else {
                drawAllNode(path[i]->right);
                if (path[i]->right) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                }
                drawAllEdge(path[i]->right);
            }
        }

        drawAllNode(current);
        drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
        drawEdge({ std::make_pair(&path.back()->value, &current->value) }, Color::edge);
        drawEdgeSlideOut({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus);
        drawAllEdge(current);
        drawCodeChangeLine(0, 1);
    } else {
        if (value < std::stoi(path.back()->value.getValue())) {
            current = path.back()->left = new Node(GraphicNode(std::to_string(value), GraphicNodeData::initialPosition));
        } else {
            current = path.back()->right = new Node(GraphicNode(std::to_string(value), GraphicNodeData::initialPosition));
        }

        addNewStep();
        drawReformat();
        for (int i = 0; i < (int)path.size(); ++i) {
            draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            if (i == (int)path.size() - 1) {
                drawFadeOut({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            } else {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
            }

            if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                drawAllNode(path[i]->left);
                if (path[i]->left) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                }
                drawAllEdge(path[i]->left);
            } else {
                drawAllNode(path[i]->right);
                if (path[i]->right) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                }
                drawAllEdge(path[i]->right);
            }
        }

        drawFadeIn({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
        drawEdgeSlideIn({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus);
        drawCodeChangeLine(0, 0);

        addNewStep();
        for (int i = 0; i < (int)path.size(); ++i) {
            draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            if (i < (int)path.size() - 1) {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
            }

            if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                drawAllNode(path[i]->left);
                if (path[i]->left) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                }
                drawAllEdge(path[i]->left);
            } else {
                drawAllNode(path[i]->right);
                if (path[i]->right) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                }
                drawAllEdge(path[i]->right);
            }

            drawCode();
        }

        draw({ &current->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
        drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus3, Color::nodeTextFocus1);
        drawFadeIn({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        drawEdge({ std::make_pair(&path.back()->value, &current->value) }, Color::edge);
        drawEdgeSlideOut({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus);
        drawCodeChangeLine(0, 1);
    }

    while (!path.empty()) {
        current = path.back();
        path.pop_back();

        int balanceFactor = getBalanceFactor(current);
        int balanceCase = 1;
        if (balanceFactor < -1 || balanceFactor > 1) {
            addNewStep();
            if (balanceFactor < -1) {
                if (value > std::stoi(current->right->value.getValue())) {
                    for (int i = 0; i < (int)path.size(); ++i) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        if (i == (int)path.size() - 1) {
                        } else {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        }

                        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                            drawAllNode(path[i]->left);
                            if (path[i]->left) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->left);
                        } else {
                            drawAllNode(path[i]->right);
                            if (path[i]->right) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->right);
                        }
                    }

                    Node* right = current->right;
                    Node* rightLeft = right->left;

                    drawAllNode(current);
                    drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (current->left) {
                        drawEdge({ std::make_pair(&current->value, &current->left->value) }, Color::edge);
                    }
                    drawAllEdge(current->left);
                    drawEdge({ std::make_pair(&current->value, &right->value) }, Color::edge);
                    drawAllEdge(rightLeft);
                    drawEdge({ std::make_pair(&right->value, &right->right->value) }, Color::edge);
                    drawAllEdge(right->right);
                    
                    drawFadeIn({ &right->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (rightLeft) {
                        drawEdgeChangeNode({ std::make_pair(&rightLeft->value, std::make_pair(&right->value, &current->value)) }, Color::edge);
                    }

                    if (!path.empty()) {
                        drawEdgeChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&current->value, &right->value)) }, Color::edgeFocus);
                    }

                    right->left = current;
                    current->right = rightLeft;
                    current = right;
                    drawCodeChangeLine(1, 4);
                    balanceCase = 4;
                } else {
                    draw({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    drawAllNode(current->left);
                    if (current->left) {
                        drawEdge({ std::make_pair(&current->value, &current->left->value) }, Color::edge);
                    }
                    drawAllEdge(current->left);

                    Node*& currentRight = current->right;
                    drawEdgeChangeNode({ std::make_pair(&current->value, std::make_pair(&currentRight->value, &currentRight->left->value)) }, Color::edge);
                    for (int i = 0; i < (int)path.size(); ++i) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        if (i == (int)path.size() - 1) {
                        } else {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        }

                        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                            drawAllNode(path[i]->left);
                            if (path[i]->left) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->left);
                        } else {
                            drawAllNode(path[i]->right);
                            if (path[i]->right) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->right);
                        }
                    }

                    if (!path.empty()) {
                        drawEdge({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus);
                    }

                    Node* left = currentRight->left;
                    Node* leftRight = left->right;

                    drawAllNode(currentRight);
                    if (currentRight->right) {
                        drawEdge({ std::make_pair(&currentRight->value, &currentRight->right->value) }, Color::edge);
                    }
                    drawAllEdge(currentRight->right);
                    drawEdge({ std::make_pair(&currentRight->value, &left->value) }, Color::edge);
                    drawAllEdge(leftRight);
                    if (left->left) {
                        drawEdge({ std::make_pair(&left->value, &left->left->value) }, Color::edge);
                    }
                    drawAllEdge(left->left);
                    
                    if (leftRight) {
                        drawEdgeChangeNode({ std::make_pair(&leftRight->value, std::make_pair(&left->value, &currentRight->value)) }, Color::edge);
                    }

                    drawEdgeChangeNode({ std::make_pair(&current->value, std::make_pair(&currentRight->value, &left->value)) }, Color::edge);

                    left->right = currentRight;
                    currentRight->left = leftRight;
                    currentRight = left;
                    drawReformat();
                    drawCodeChangeLine(1, 5);

                    addNewStep();
                    for (int i = 0; i < (int)path.size(); ++i) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        if (i == (int)path.size() - 1) {
                        } else {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        }

                        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                            drawAllNode(path[i]->left);
                            if (path[i]->left) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->left);
                        } else {
                            drawAllNode(path[i]->right);
                            if (path[i]->right) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->right);
                        }
                    }

                    Node* right = current->right;
                    Node* rightLeft = right->left;

                    drawAllNode(current);
                    drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (current->left) {
                        drawEdge({ std::make_pair(&current->value, &current->left->value) }, Color::edge);
                    }
                    drawAllEdge(current->left);
                    drawEdge({ std::make_pair(&current->value, &right->value) }, Color::edge);
                    drawAllEdge(rightLeft);
                    drawEdge({ std::make_pair(&right->value, &right->right->value) }, Color::edge);
                    drawAllEdge(right->right);
                    
                    drawFadeIn({ &right->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (rightLeft) {
                        drawEdgeChangeNode({ std::make_pair(&rightLeft->value, std::make_pair(&right->value, &current->value)) }, Color::edge);
                    }

                    if (!path.empty()) {
                        drawEdgeChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&current->value, &right->value)) }, Color::edgeFocus);
                    }

                    right->left = current;
                    current->right = rightLeft;
                    current = right;
                    drawCodeChangeLine(5, 5);
                    balanceCase = 5;
                }
            } else if (balanceFactor > 1) {
                if (value < std::stoi(current->left->value.getValue())) {
                    for (int i = 0; i < (int)path.size(); ++i) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        if (i == (int)path.size() - 1) {
                        } else {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        }

                        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                            drawAllNode(path[i]->left);
                            if (path[i]->left) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->left);
                        } else {
                            drawAllNode(path[i]->right);
                            if (path[i]->right) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->right);
                        }
                    }

                    Node* left = current->left;
                    Node* leftRight = left->right;

                    drawAllNode(current);
                    drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (current->right) {
                        drawEdge({ std::make_pair(&current->value, &current->right->value) }, Color::edge);
                    }
                    drawAllEdge(current->right);
                    drawEdge({ std::make_pair(&current->value, &left->value) }, Color::edge);
                    drawAllEdge(leftRight);
                    drawEdge({ std::make_pair(&left->value, &left->left->value) }, Color::edge);
                    drawAllEdge(left->left);
                    
                    drawFadeIn({ &left->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (leftRight) {
                        drawEdgeChangeNode({ std::make_pair(&leftRight->value, std::make_pair(&left->value, &current->value)) }, Color::edge);
                    }

                    if (!path.empty()) {
                        drawEdgeChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&current->value, &left->value)) }, Color::edgeFocus);
                    }

                    left->right = current;
                    current->left = leftRight;
                    current = left;
                    drawCodeChangeLine(1, 2);
                    balanceCase = 2;
                } else {
                    draw({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    drawAllNode(current->right);
                    if (current->right) {
                        drawEdge({ std::make_pair(&current->value, &current->right->value) }, Color::edge);
                    }
                    drawAllEdge(current->right);

                    Node*& currentLeft = current->left;
                    drawEdgeChangeNode({ std::make_pair(&current->value, std::make_pair(&currentLeft->value, &currentLeft->right->value)) }, Color::edge);
                    for (int i = 0; i < (int)path.size(); ++i) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        if (i == (int)path.size() - 1) {
                        } else {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        }

                        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                            drawAllNode(path[i]->left);
                            if (path[i]->left) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->left);
                        } else {
                            drawAllNode(path[i]->right);
                            if (path[i]->right) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->right);
                        }
                    }

                    if (!path.empty()) {
                        drawEdge({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus);
                    }

                    Node* right = currentLeft->right;
                    Node* rightLeft = right->left;

                    drawAllNode(currentLeft);
                    if (currentLeft->left) {
                        drawEdge({ std::make_pair(&currentLeft->value, &currentLeft->left->value) }, Color::edge);
                    }
                    drawAllEdge(currentLeft->left);
                    drawEdge({ std::make_pair(&currentLeft->value, &right->value) }, Color::edge);
                    drawAllEdge(rightLeft);
                    if (right->right) {
                        drawEdge({ std::make_pair(&right->value, &right->right->value) }, Color::edge);
                    }
                    drawAllEdge(right->right);
                    
                    if (rightLeft) {
                        drawEdgeChangeNode({ std::make_pair(&rightLeft->value, std::make_pair(&right->value, &currentLeft->value)) }, Color::edge);
                    }

                    drawEdgeChangeNode({ std::make_pair(&current->value, std::make_pair(&currentLeft->value, &right->value)) }, Color::edge);

                    right->left = currentLeft;
                    currentLeft->right = rightLeft;
                    currentLeft = right;
                    drawReformat();
                    drawCodeChangeLine(1, 3);

                    addNewStep();
                    for (int i = 0; i < (int)path.size(); ++i) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        if (i == (int)path.size() - 1) {
                        } else {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        }

                        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                            drawAllNode(path[i]->left);
                            if (path[i]->left) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->left);
                        } else {
                            drawAllNode(path[i]->right);
                            if (path[i]->right) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->right);
                        }
                    }

                    Node* left = current->left;
                    Node* leftRight = left->right;

                    drawAllNode(current);
                    drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (current->right) {
                        drawEdge({ std::make_pair(&current->value, &current->right->value) }, Color::edge);
                    }
                    drawAllEdge(current->right);
                    drawEdge({ std::make_pair(&current->value, &left->value) }, Color::edge);
                    drawAllEdge(leftRight);
                    drawEdge({ std::make_pair(&left->value, &left->left->value) }, Color::edge);
                    drawAllEdge(left->left);
                    
                    drawFadeIn({ &left->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (leftRight) {
                        drawEdgeChangeNode({ std::make_pair(&leftRight->value, std::make_pair(&left->value, &current->value)) }, Color::edge);
                    }

                    if (!path.empty()) {
                        drawEdgeChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&current->value, &left->value)) }, Color::edgeFocus);
                    }

                    left->right = current;
                    current->left = leftRight;
                    current = left;
                    drawCodeChangeLine(3, 3);
                    balanceCase = 3;
                }
            }

            if (path.empty()) {
                mRoot = current;
            } else if (value < std::stoi(path.back()->value.getValue())) {
                path.back()->left = current;
            } else {
                path.back()->right = current;
            }

            drawReformat();
        }

        addNewStep();
        for (int i = 0; i < (int)path.size(); ++i) {
            draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            if (i == (int)path.size() - 1) {
                drawFadeIn({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            } else {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
            }

            if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                drawAllNode(path[i]->left);
                if (path[i]->left) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                }
                drawAllEdge(path[i]->left);
            } else {
                drawAllNode(path[i]->right);
                if (path[i]->right) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                }
                drawAllEdge(path[i]->right);
            }
        }
        
        if (!path.empty()) {
            drawEdge({ std::make_pair(&path.back()->value, &current->value) }, Color::edge);
            drawEdgeSlideOut({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus);
        }
        drawAllNode(current);
        drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        drawAllEdge(current);
        if (path.empty()) {
            drawCodeFadeOut(balanceCase);
        } else {
            drawCodeChangeLine(balanceCase, 1);
        }
    }
}

void AVLTree::erase(int value) {
    mCode.update({
        "erase v",
        "check balance factor of node and its children",
        "    case LL: rotateR(node)",
        "    case LR: rotateL(node.left), rotateR(node)",
        "    case RR: rotateL(node)",
        "    case RL: rotateR(node.right), rotateL(node)",
    });

    addNewStep();
    drawReformat();
    drawAllNode(mRoot);
    if (std::stoi(mRoot->value.getValue()) == value) {
        drawFadeIn({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
    } else {
        drawFadeIn({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    }
    
    drawAllEdge(mRoot);
    drawCodeFadeIn(0);

    std::vector<Node*> path;
    Node* current = mRoot;
    bool exist = false;
    while (current != nullptr) {
        if (value == std::stoi(current->value.getValue())) {
            exist = true;
            break;
        }

        path.push_back(current);
        if (value < std::stoi(current->value.getValue())) {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current == nullptr) {
            break;
        }

        addNewStep();
        for (int i = 0; i < (int)path.size(); ++i) {
            draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            if (i == (int)path.size() - 1) {
                drawFadeOut({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            } else {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
            }

            if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                drawAllNode(path[i]->left);
                if (path[i]->left) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                }
                drawAllEdge(path[i]->left);
            } else {
                drawAllNode(path[i]->right);
                if (path[i]->right) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                }
                drawAllEdge(path[i]->right);
            }
        }

        drawAllNode(current);
        if (value == std::stoi(current->value.getValue())) {
            drawFadeIn({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
        } else {
            drawFadeIn({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        }
        drawAllEdge(current);
        drawEdge({ std::make_pair(&path.back()->value, &current->value) }, Color::edge);
        drawEdgeSlideIn({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus);
        drawCodeChangeLine(0, 0);
    }

    if (exist) {
        if (size() == 0) {
            mErasedNode = mRoot;
            mRoot = nullptr;
            addNewStep();
            drawFadeOut({ &mErasedNode->value }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
            drawCodeFadeOut(0);
            return;
        }

        if (!current->left && !current->right) {
            mErasedNode = current;
            if (!path.empty()) {
                if (path.back()->left == mErasedNode) {
                    path.back()->left = nullptr;
                } else {
                    path.back()->right = nullptr;
                }
            } else {
                mRoot = nullptr;
            }

            addNewStep();
            drawReformat();
            for (int i = 0; i < (int)path.size(); ++i) {
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                if (i == (int)path.size() - 1) {
                    drawFadeIn({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                } else {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                }

                if ((i == (int)path.size() - 1 && value > std::stoi(path[i]->value.getValue())) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                    drawAllNode(path[i]->left);
                    if (path[i]->left) {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                    }
                    drawAllEdge(path[i]->left);
                } else {
                    drawAllNode(path[i]->right);
                    if (path[i]->right) {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                    }
                    drawAllEdge(path[i]->right);
                }
            }

            drawFadeOut({ &mErasedNode->value }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
            drawEdgeSlideOut({ std::make_pair(&path.back()->value, &mErasedNode->value) }, Color::edgeFocus);
            drawCodeChangeLine(0, 1);
        } else if (!current->right) {
            mErasedNode = current;
            if (!path.empty()) {
                if (path.back()->left == mErasedNode) {
                    path.back()->left = mErasedNode->left;
                } else {
                    path.back()->right = mErasedNode->left;
                }
            } else {
                mRoot = mErasedNode->left;
            }

            addNewStep();
            drawReformat();
            for (int i = 0; i < (int)path.size(); ++i) {
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                if (i == (int)path.size() - 1) {
                    drawFadeIn({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                } else {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                }

                if ((i == (int)path.size() - 1 && path[i]->right == mErasedNode->left) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                    drawAllNode(path[i]->left);
                    if (path[i]->left) {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                    }
                    drawAllEdge(path[i]->left);
                } else {
                    drawAllNode(path[i]->right);
                    if (path[i]->right) {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                    }
                    drawAllEdge(path[i]->right);
                }
            }

            drawAllNode(mErasedNode->left);
            drawEdgeSlideOut({ std::make_pair(&mErasedNode->left->value, &mErasedNode->value) }, Color::edge);
            if (!path.empty()) {
                drawEdgeChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&mErasedNode->value, &mErasedNode->left->value)) }, Color::edge);
                drawEdgeSlideOutChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&mErasedNode->value, &mErasedNode->left->value)) }, Color::edgeFocus);
            }
            drawFadeOut({ &mErasedNode->value }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
            drawAllEdge(mErasedNode->left);
            drawCodeChangeLine(0, 1);
        } else if (!current->left) {
            mErasedNode = current;
            if (!path.empty()) {
                if (path.back()->left == mErasedNode) {
                    path.back()->left = mErasedNode->right;
                } else {
                    path.back()->right = mErasedNode->right;
                }
            } else {
                mRoot = mErasedNode->right;
            }

            addNewStep();
            drawReformat();
            for (int i = 0; i < (int)path.size(); ++i) {
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                if (i == (int)path.size() - 1) {
                    drawFadeIn({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                } else {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                }

                if ((i == (int)path.size() - 1 && path[i]->right == mErasedNode->right) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                    drawAllNode(path[i]->left);
                    if (path[i]->left) {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                    }
                    drawAllEdge(path[i]->left);
                } else {
                    drawAllNode(path[i]->right);
                    if (path[i]->right) {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                    }
                    drawAllEdge(path[i]->right);
                }
            }

            drawAllNode(mErasedNode->right);
            drawEdgeSlideOut({ std::make_pair(&mErasedNode->right->value, &mErasedNode->value) }, Color::edge);
            if (!path.empty()) {
                drawEdgeChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&mErasedNode->value, &mErasedNode->right->value)) }, Color::edge);
                drawEdgeSlideOutChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&mErasedNode->value, &mErasedNode->right->value)) }, Color::edgeFocus);
            }
            drawFadeOut({ &mErasedNode->value }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
            drawAllEdge(mErasedNode->right);
            drawCodeChangeLine(0, 1);
        } else {
            Node* successor = current;
            while (successor->left) {
                path.push_back(successor);
                if (successor == current) {
                    successor = successor->right;
                } else {
                    successor = successor->left;
                }

                addNewStep();
                for (int i = 0; i < (int)path.size(); ++i) {
                    if (path[i] != current) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                    }

                    if (i == (int)path.size() - 1) {
                        drawFadeOut({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    } else {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                    }

                    if ((i == (int)path.size() - 1 && path[i]->right == successor) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                        drawAllNode(path[i]->left);
                        if (path[i]->left) {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                        }
                        drawAllEdge(path[i]->left);
                    } else {
                        drawAllNode(path[i]->right);
                        if (path[i]->right) {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                        }
                        drawAllEdge(path[i]->right);
                    }
                }

                drawEdge({ std::make_pair(&path.back()->value, &successor->value) }, Color::edge);
                drawEdgeSlideIn({ std::make_pair(&path.back()->value, &successor->value) }, Color::edgeFocus);
                drawAllNode(successor);
                draw({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
                drawFadeIn({ &successor->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                drawAllEdge(successor);
                drawCodeChangeLine(0, 0);
            }

            addNewStep();
            for (int i = 0; i < (int)path.size(); ++i) {
                if (path[i] != current) {
                    draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                }

                if (i == (int)path.size() - 1) {
                } else {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                }

                if ((i == (int)path.size() - 1 && path[i]->right == successor) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                    drawAllNode(path[i]->left);
                    if (path[i]->left) {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                    }
                    drawAllEdge(path[i]->left);
                } else {
                    drawAllNode(path[i]->right);
                    if (path[i]->right) {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                    }
                    drawAllEdge(path[i]->right);
                }
            }

            drawEdge({ std::make_pair(&path.back()->value, &successor->value) }, Color::edgeFocus);
            drawAllNode(successor->left);
            drawAllNode(successor->right);
            drawChangeValueColor(&current->value, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeFocus1, Color::nodeTextFocus1, Color::nodeTextFocus1, current->value.getValue(), successor->value.getValue());
            drawChangeColor({ &successor->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeFocus4, Color::nodeTextFocus1, Color::nodeTextFocus1);
            drawAllEdge(successor);
            drawCodeChangeLine(0, 0);

            mErasedNode = successor;
            int successorCase = 0;
            if (current->right == successor) {
                path.back()->right = successor->right;
                successorCase = 1;
            } else {
                path.back()->left = successor->right;
                successorCase = 2;
            }

            addNewStep();
            drawReformat();
            for (int i = 0; i < (int)path.size(); ++i) {
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                if (i == (int)path.size() - 1) {
                    drawFadeIn({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                } else {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                }

                if ((i == (int)path.size() - 1 && path[i]->right == (successorCase == 1 ? successor->right : current->right)) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                    drawAllNode(path[i]->left);
                    if (path[i]->left) {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                    }
                    drawAllEdge(path[i]->left);
                } else {
                    drawAllNode(path[i]->right);
                    if (path[i]->right) {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                    }
                    drawAllEdge(path[i]->right);
                }
            }

            drawFadeOut({ &current->value}, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            drawFadeOut({ &mErasedNode->value }, Shape::circle, Type::filled, Color::nodeFocus4, Color::nodeTextFocus1);
            if (successorCase == 1) {
                drawAllNode(mErasedNode->right);
                if (mErasedNode->right) {
                    drawEdgeSlideOut({ std::make_pair(&mErasedNode->right->value, &mErasedNode->value) }, Color::edge);
                    drawEdgeChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&mErasedNode->value, &mErasedNode->right->value)) }, Color::edge);
                    drawEdgeSlideOutChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&mErasedNode->value, &mErasedNode->right->value)) }, Color::edgeFocus);
                } else {
                    drawEdgeSlideOut({ std::make_pair(&path.back()->value, &mErasedNode->value) }, Color::edgeFocus);
                }
                drawAllEdge(mErasedNode->right);
            } else {
                if (!successor->right) {
                    drawEdgeSlideOut({ std::make_pair(&path.back()->value, &mErasedNode->value) }, Color::edgeFocus);
                } else {
                    drawAllNode(mErasedNode->right);
                    drawEdgeSlideOut({ std::make_pair(&mErasedNode->right->value, &mErasedNode->value) }, Color::edge);
                    drawEdgeChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&mErasedNode->value, &mErasedNode->right->value)) }, Color::edge);
                    drawEdgeSlideOutChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&mErasedNode->value, &mErasedNode->right->value)) }, Color::edgeFocus);
                    drawAllEdge(mErasedNode->right);
                }
            }
            drawCodeChangeLine(0, 1);
            
            value = std::stoi(successor->value.getValue());
        }
    }

    while (!path.empty()) {
        current = path.back();
        path.pop_back();

        int balanceFactor = getBalanceFactor(current);
        int balanceCase = 1;
        if (balanceFactor < -1 || balanceFactor > 1) {
            addNewStep();
            if (balanceFactor < -1) {
                if (getBalanceFactor(current->right) <= 0) {
                    for (int i = 0; i < (int)path.size(); ++i) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        if (i == (int)path.size() - 1) {
                        } else {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        }

                        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                            drawAllNode(path[i]->left);
                            if (path[i]->left) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->left);
                        } else {
                            drawAllNode(path[i]->right);
                            if (path[i]->right) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->right);
                        }
                    }

                    Node* right = current->right;
                    Node* rightLeft = right->left;

                    drawAllNode(current);
                    drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (current->left) {
                        drawEdge({ std::make_pair(&current->value, &current->left->value) }, Color::edge);
                    }
                    drawAllEdge(current->left);
                    drawEdge({ std::make_pair(&current->value, &right->value) }, Color::edge);
                    drawAllEdge(rightLeft);
                    drawEdge({ std::make_pair(&right->value, &right->right->value) }, Color::edge);
                    drawAllEdge(right->right);
                    
                    drawFadeIn({ &right->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (rightLeft) {
                        drawEdgeChangeNode({ std::make_pair(&rightLeft->value, std::make_pair(&right->value, &current->value)) }, Color::edge);
                    }

                    if (!path.empty()) {
                        drawEdgeChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&current->value, &right->value)) }, Color::edgeFocus);
                    }

                    right->left = current;
                    current->right = rightLeft;
                    current = right;
                    drawCodeChangeLine(1, 4);
                    balanceCase = 4;
                } else {
                    draw({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    drawAllNode(current->left);
                    if (current->left) {
                        drawEdge({ std::make_pair(&current->value, &current->left->value) }, Color::edge);
                    }
                    drawAllEdge(current->left);

                    Node*& currentRight = current->right;
                    drawEdgeChangeNode({ std::make_pair(&current->value, std::make_pair(&currentRight->value, &currentRight->left->value)) }, Color::edge);
                    for (int i = 0; i < (int)path.size(); ++i) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        if (i == (int)path.size() - 1) {
                        } else {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        }

                        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                            drawAllNode(path[i]->left);
                            if (path[i]->left) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->left);
                        } else {
                            drawAllNode(path[i]->right);
                            if (path[i]->right) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->right);
                        }
                    }

                    if (!path.empty()) {
                        drawEdge({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus);
                    }

                    Node* left = currentRight->left;
                    Node* leftRight = left->right;

                    drawAllNode(currentRight);
                    if (currentRight->right) {
                        drawEdge({ std::make_pair(&currentRight->value, &currentRight->right->value) }, Color::edge);
                    }
                    drawAllEdge(currentRight->right);
                    drawEdge({ std::make_pair(&currentRight->value, &left->value) }, Color::edge);
                    drawAllEdge(leftRight);
                    if (left->left) {
                        drawEdge({ std::make_pair(&left->value, &left->left->value) }, Color::edge);
                    }
                    drawAllEdge(left->left);
                    
                    if (leftRight) {
                        drawEdgeChangeNode({ std::make_pair(&leftRight->value, std::make_pair(&left->value, &currentRight->value)) }, Color::edge);
                    }

                    drawEdgeChangeNode({ std::make_pair(&current->value, std::make_pair(&currentRight->value, &left->value)) }, Color::edge);

                    left->right = currentRight;
                    currentRight->left = leftRight;
                    currentRight = left;
                    drawReformat();
                    drawCodeChangeLine(1, 5);

                    addNewStep();
                    for (int i = 0; i < (int)path.size(); ++i) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        if (i == (int)path.size() - 1) {
                        } else {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        }

                        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                            drawAllNode(path[i]->left);
                            if (path[i]->left) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->left);
                        } else {
                            drawAllNode(path[i]->right);
                            if (path[i]->right) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->right);
                        }
                    }

                    Node* right = current->right;
                    Node* rightLeft = right->left;

                    drawAllNode(current);
                    drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (current->left) {
                        drawEdge({ std::make_pair(&current->value, &current->left->value) }, Color::edge);
                    }
                    drawAllEdge(current->left);
                    drawEdge({ std::make_pair(&current->value, &right->value) }, Color::edge);
                    drawAllEdge(rightLeft);
                    drawEdge({ std::make_pair(&right->value, &right->right->value) }, Color::edge);
                    drawAllEdge(right->right);
                    
                    drawFadeIn({ &right->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (rightLeft) {
                        drawEdgeChangeNode({ std::make_pair(&rightLeft->value, std::make_pair(&right->value, &current->value)) }, Color::edge);
                    }

                    if (!path.empty()) {
                        drawEdgeChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&current->value, &right->value)) }, Color::edgeFocus);
                    }

                    right->left = current;
                    current->right = rightLeft;
                    current = right;
                    drawCodeChangeLine(5, 5);
                    balanceCase = 5;
                }
            } else if (balanceFactor > 1) {
                if (getBalanceFactor(current->left) >= 0) {
                    for (int i = 0; i < (int)path.size(); ++i) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        if (i == (int)path.size() - 1) {
                        } else {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        }

                        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                            drawAllNode(path[i]->left);
                            if (path[i]->left) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->left);
                        } else {
                            drawAllNode(path[i]->right);
                            if (path[i]->right) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->right);
                        }
                    }

                    Node* left = current->left;
                    Node* leftRight = left->right;

                    drawAllNode(current);
                    drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (current->right) {
                        drawEdge({ std::make_pair(&current->value, &current->right->value) }, Color::edge);
                    }
                    drawAllEdge(current->right);
                    drawEdge({ std::make_pair(&current->value, &left->value) }, Color::edge);
                    drawAllEdge(leftRight);
                    drawEdge({ std::make_pair(&left->value, &left->left->value) }, Color::edge);
                    drawAllEdge(left->left);
                    
                    drawFadeIn({ &left->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (leftRight) {
                        drawEdgeChangeNode({ std::make_pair(&leftRight->value, std::make_pair(&left->value, &current->value)) }, Color::edge);
                    }

                    if (!path.empty()) {
                        drawEdgeChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&current->value, &left->value)) }, Color::edgeFocus);
                    }

                    left->right = current;
                    current->left = leftRight;
                    current = left;
                    drawCodeChangeLine(1, 2);
                    balanceCase = 2;
                } else {
                    draw({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    drawAllNode(current->right);
                    if (current->right) {
                        drawEdge({ std::make_pair(&current->value, &current->right->value) }, Color::edge);
                    }
                    drawAllEdge(current->right);

                    Node*& currentLeft = current->left;
                    drawEdgeChangeNode({ std::make_pair(&current->value, std::make_pair(&currentLeft->value, &currentLeft->right->value)) }, Color::edge);
                    for (int i = 0; i < (int)path.size(); ++i) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        if (i == (int)path.size() - 1) {
                        } else {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        }

                        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                            drawAllNode(path[i]->left);
                            if (path[i]->left) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->left);
                        } else {
                            drawAllNode(path[i]->right);
                            if (path[i]->right) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->right);
                        }
                    }

                    if (!path.empty()) {
                        drawEdge({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus);
                    }

                    Node* right = currentLeft->right;
                    Node* rightLeft = right->left;

                    drawAllNode(currentLeft);
                    if (currentLeft->left) {
                        drawEdge({ std::make_pair(&currentLeft->value, &currentLeft->left->value) }, Color::edge);
                    }
                    drawAllEdge(currentLeft->left);
                    drawEdge({ std::make_pair(&currentLeft->value, &right->value) }, Color::edge);
                    drawAllEdge(rightLeft);
                    if (right->right) {
                        drawEdge({ std::make_pair(&right->value, &right->right->value) }, Color::edge);
                    }
                    drawAllEdge(right->right);
                    
                    if (rightLeft) {
                        drawEdgeChangeNode({ std::make_pair(&rightLeft->value, std::make_pair(&right->value, &currentLeft->value)) }, Color::edge);
                    }

                    drawEdgeChangeNode({ std::make_pair(&current->value, std::make_pair(&currentLeft->value, &right->value)) }, Color::edge);

                    right->left = currentLeft;
                    currentLeft->right = rightLeft;
                    currentLeft = right;
                    drawReformat();
                    drawCodeChangeLine(1, 3);

                    addNewStep();
                    for (int i = 0; i < (int)path.size(); ++i) {
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        if (i == (int)path.size() - 1) {
                        } else {
                            drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        }

                        if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                            drawAllNode(path[i]->left);
                            if (path[i]->left) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->left);
                        } else {
                            drawAllNode(path[i]->right);
                            if (path[i]->right) {
                                drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                            }
                            drawAllEdge(path[i]->right);
                        }
                    }

                    Node* left = current->left;
                    Node* leftRight = left->right;

                    drawAllNode(current);
                    drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (current->right) {
                        drawEdge({ std::make_pair(&current->value, &current->right->value) }, Color::edge);
                    }
                    drawAllEdge(current->right);
                    drawEdge({ std::make_pair(&current->value, &left->value) }, Color::edge);
                    drawAllEdge(leftRight);
                    drawEdge({ std::make_pair(&left->value, &left->left->value) }, Color::edge);
                    drawAllEdge(left->left);
                    
                    drawFadeIn({ &left->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    if (leftRight) {
                        drawEdgeChangeNode({ std::make_pair(&leftRight->value, std::make_pair(&left->value, &current->value)) }, Color::edge);
                    }

                    if (!path.empty()) {
                        drawEdgeChangeNode({ std::make_pair(&path.back()->value, std::make_pair(&current->value, &left->value)) }, Color::edgeFocus);
                    }

                    left->right = current;
                    current->left = leftRight;
                    current = left;
                    drawCodeChangeLine(3, 3);
                    balanceCase = 3;
                }
            }

            if (path.empty()) {
                mRoot = current;
            } else if (value < std::stoi(path.back()->value.getValue())) {
                path.back()->left = current;
            } else {
                path.back()->right = current;
            }

            drawReformat();
        }

        addNewStep();
        for (int i = 0; i < (int)path.size(); ++i) {
            draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            if (i == (int)path.size() - 1) {
                drawFadeIn({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            } else {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
            }

            if ((i == (int)path.size() - 1 && path[i]->right == current) || (i < (int)path.size() - 1 && path[i]->right == path[i + 1])) {
                drawAllNode(path[i]->left);
                if (path[i]->left) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->left->value) }, Color::edge);
                }
                drawAllEdge(path[i]->left);
            } else {
                drawAllNode(path[i]->right);
                if (path[i]->right) {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i]->right->value) }, Color::edge);
                }
                drawAllEdge(path[i]->right);
            }
        }
        
        if (!path.empty()) {
            drawEdge({ std::make_pair(&path.back()->value, &current->value) }, Color::edge);
            drawEdgeSlideOut({ std::make_pair(&path.back()->value, &current->value) }, Color::edgeFocus);
        }
        drawAllNode(current);
        drawFadeOut({ &current->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        drawAllEdge(current);
        if (path.empty()) {
            drawCodeFadeOut(balanceCase);
        } else if (!exist) {
            drawCodeChangeLine(0, 1);
            exist = true;
        } else {
            drawCodeChangeLine(balanceCase, 1);
        }
    }
}

void AVLTree::run() { 
    std::function<bool()> conditionNone = [&]() { return true; };
    std::function<bool()> conditionTreeNotLarge = [&]() { return size() < AVLTreeData::maxSize; };
    std::function<bool()> conditionTreeNotEmpty = [&]() { return size() > 0; };

    mOption.addOption("Create");
    mOption.addSuboption("Empty", conditionNone);
    mOption.addSuboption("Random", conditionNone);
    mOption.addSuboptionInputBox("n",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, AVLTreeData::minSize, AVLTreeData::maxSize),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), AVLTreeData::minSize, AVLTreeData::maxSize)
    );

    mOption.addOption("Search");
    mOption.addSuboption("", conditionNone);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, AVLTreeData::minValue, AVLTreeData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), AVLTreeData::minValue, AVLTreeData::maxValue)
    );

    mOption.addOption("Insert");
    mOption.addSuboption("", conditionTreeNotLarge);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, AVLTreeData::minValue, AVLTreeData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), AVLTreeData::minValue, AVLTreeData::maxValue)
    );

    mOption.addOption("Erase");
    mOption.addSuboption("", conditionTreeNotEmpty);
    mOption.addSuboptionInputBox("v",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, AVLTreeData::minValue, AVLTreeData::maxValue),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), AVLTreeData::minValue, AVLTreeData::maxValue)
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

                case 1: // Search
                    switch (option.second) {
                    case 0:
                        search(std::stoi(values[0]));
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
                    case 0:
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
