#include "TrieVisualizer.hpp"

TrieVisualizer::Node::Node() {
}

TrieVisualizer::Node::Node(GraphicNode value) {
    this->value = value;
    this->children.clear();
    this->marked = false;
}

TrieVisualizer::TrieVisualizer() : Visualizer() {
}

TrieVisualizer::TrieVisualizer(sf::RenderWindow* window, AssetsHolder* assets) : Visualizer(window, assets) {
    mRoot = nullptr;
    mErasedNodes.clear();
}

void TrieVisualizer::freeMemory() {
    for (auto node : mErasedNodes) {
        delete node;
    }
    mErasedNodes.clear();
}

void TrieVisualizer::deleteTree() {
    if (mRoot == nullptr) {
        return;
    }

    std::function<void(Node*)> deleteSubtree = [&](Node* node) {
        if (node == nullptr) {
            return;
        }

        for (auto& child : node->children) {
            deleteSubtree(child.second);
        }

        delete node;
    };

    deleteSubtree(mRoot);
    mRoot = nullptr;
}

void TrieVisualizer::setPositions() {
    std::function<sf::Vector2f(Node* node)> getSubtreeRange = [&](Node* node) {
        if (node == nullptr) {
            return sf::Vector2f(1e9, -1e9);
        }

        sf::Vector2f range = sf::Vector2f(node->value.getPosition().x, node->value.getPosition().x);
        for (auto& child : node->children) {
            sf::Vector2f childRange = getSubtreeRange(child.second);
            range.x = std::min(range.x, childRange.x);
            range.y = std::max(range.y, childRange.y);
        }

        return range;
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
        if (node == nullptr) {
            return;
        }

        if (node->children.empty()) {
            node->value.setPosition(GraphicNodeData::initialPosition);
            return;
        }

        for (auto& child : node->children) {
            setSubtreePosition(child.second);
        }

        float firstChildPositionY = node->children.begin()->second->value.getPosition().y;
        float previousChildPositionX = 0.f;
        bool isFirstChild = true;
        sf::Vector2f subtreeRange = sf::Vector2f(1e9, -1e9);
        for (auto& child : node->children) {
            if (!isFirstChild) {
                float deltaY = child.second->value.getPosition().y - firstChildPositionY;
                offsetSubtree(child.second, sf::Vector2f(0.f, -deltaY));

                float deltaX = getSubtreeRange(child.second).x - previousChildPositionX - 2 * TrieVisualizerData::space.x;
                offsetSubtree(child.second, sf::Vector2f(-deltaX, 0.f));
            } else {
                isFirstChild = false;
            }

            previousChildPositionX = getSubtreeRange(child.second).y;
            subtreeRange.x = std::min(subtreeRange.x, child.second->value.getPosition().x);
            subtreeRange.y = std::max(subtreeRange.y, child.second->value.getPosition().x);
        }

        node->value.setPosition(sf::Vector2f((subtreeRange.x + subtreeRange.y) / 2.f, firstChildPositionY - TrieVisualizerData::space.y));
    };

    if (mRoot == nullptr) {
        return;
    }
    
    setSubtreePosition(mRoot);

    sf::Vector2f treeRange = getSubtreeRange(mRoot);
    offsetSubtree(mRoot, sf::Vector2f(((float)mWindow->getSize().x - (treeRange.y - treeRange.x)) / 2.f - treeRange.x, TrieVisualizerData::treePositionY - mRoot->value.getPosition().y));
    offsetSubtree(mRoot, sf::Vector2f(-GraphicNodeData::nodeSize.x / 2.f, -GraphicNodeData::nodeSize.y / 2.f));
}

void TrieVisualizer::drawReformat() {
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

void TrieVisualizer::drawAllNode(Node* node) {
    if (node == nullptr) {
        return;
    }

    if (node->marked) {
        draw({ &node->value }, Shape::circle, Type::hollow, Color::nodeFocus2, Color::nodeTextFocus3);
    } else {
        draw({ &node->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
    }

    for (auto& child : node->children) {
        drawAllNode(child.second);
    }
}

void TrieVisualizer::drawAllNodeExceptRoot(Node* node) {
    if (node == nullptr) {
        return;
    }

    for (auto& child : node->children) {
        drawAllNode(child.second);
    }
}

void TrieVisualizer::drawAllNodeFadeIn(Node* node) {
    if (node == nullptr) {
        return;
    }

    if (node->marked) {
        drawFadeIn({ &node->value }, Shape::circle, Type::hollow, Color::nodeFocus2, Color::nodeTextFocus3);
    } else {
        drawFadeIn({ &node->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
    }

    for (auto& child : node->children) {
        drawAllNodeFadeIn(child.second);
    }
}

void TrieVisualizer::drawAllNodeExceptChild(Node* node, char childLetter) {
    if (node == nullptr) {
        return;
    }

    for (auto& child : node->children) {
        if (child.first != childLetter) {
            drawAllNode(child.second);
        }
    }
}

void TrieVisualizer::drawAllEdge(Node* node) {
    if (node == nullptr) {
        return;
    }

    for (auto& child : node->children) {
        drawEdge({ std::make_pair(&node->value, &child.second->value) }, Color::edge);
        drawAllEdge(child.second);
    }
}

void TrieVisualizer::drawAllEdgeFadeIn(Node* node) {
    if (node == nullptr) {
        return;
    }

    for (auto& child : node->children) {
        drawEdgeFadeIn({ std::make_pair(&node->value, &child.second->value) }, Color::edge);
        drawAllEdgeFadeIn(child.second);
    }
}

void TrieVisualizer::drawAllEdgeExceptChild(Node* node, char childLetter) {
    if (node == nullptr) {
        return;
    }

    for (auto& child : node->children) {
        if (child.first != childLetter) {
            drawEdge({ std::make_pair(&node->value, &child.second->value) }, Color::edge);
            drawAllEdge(child.second);
        }
    }
}

void TrieVisualizer::create(int number) {
    deleteTree();

    mRoot = new Node(GraphicNode(""));

    std::set<std::string> strs;
    while ((int)strs.size() < number) {
        strs.insert(Randomizer::stringOfLowercaseLetters(TrieVisualizerData::minLength, TrieVisualizerData::maxLength));
    }

    for (std::string str : strs) {
        Node* current = mRoot;
        for (char letter : str) {
            if (!current->children.count(letter)) {
                current->children[letter] = new Node(GraphicNode(std::string(1, letter)));
            }

            current = current->children[letter];
        }

        current->marked = true;
    }

    mCode.update({});

    setPositions();

    addNewStep();
    drawAllEdgeFadeIn(mRoot);
    drawAllNodeFadeIn(mRoot);
    drawCode();
}

void TrieVisualizer::createFromList(std::string input) {
    for (auto& character : input) {
        if (!(character >= 'a' && character <= 'z')) {
            character = ' ';
        }
    }

    std::stringstream ss(input);
    std::vector<std::string> strs;
    for (std::string token; ss >> token; ) {
        strs.push_back(token);
    }

    deleteTree();

    mRoot = new Node(GraphicNode(""));
    for (std::string str : strs) {
        Node* current = mRoot;
        for (char letter : str) {
            if (!current->children.count(letter)) {
                current->children[letter] = new Node(GraphicNode(std::string(1, letter)));
            }

            current = current->children[letter];
        }

        current->marked = true;
    }

    mCode.update({});

    setPositions();

    addNewStep();
    drawAllEdgeFadeIn(mRoot);
    drawAllNodeFadeIn(mRoot);
    drawCode();
}

void TrieVisualizer::search(std::string str) {
    mCode.update({
        "Node cur = root",
        "for each c in s:",
        "    if cur.child[c] == NULL: return NOT_FOUND",
        "    cur = cur.child[c]",
        "if cur.leaf: return FOUND",
        "return NOT_FOUND"
    });

    addNewStep();
    drawAllEdge(mRoot);
    drawAllNode(mRoot);
    drawFadeIn({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    drawCodeFadeIn(0);

    std::vector<Node*> path;
    Node* current = mRoot;
    path.push_back(current);
    for (char letter : str) {
        if (!current->children.count(letter)) {
            addNewStep();
            for (int i = 0; i < (int)path.size() - 1; i++) {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
                drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
            }
            drawAllEdge(path.back());
            draw({ &path.back()->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            drawFadeOut({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            drawAllNodeExceptRoot(path.back());
            if ((int)path.size() == 1) {
                drawCodeChangeLine(0, 2);
            } else {
                drawCodeChangeLine(3, 2);
            }

            addNewStep();
            for (int i = 0; i < (int)path.size() - 1; i++) {
                drawEdgeChangeColor({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus, Color::edge);
                if (path[i]->marked) {
                    drawChangeColor({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeFocus2, Color::nodeTextFocus2, Color::nodeTextFocus3);
                } else {
                    drawChangeColor({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
                }
                drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
                drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
            }
            drawAllEdge(path.back());
            if (path.back()->marked) {
                drawChangeColor({ &path.back()->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeFocus2, Color::nodeTextFocus2, Color::nodeTextFocus3);
            } else {
                drawChangeColor({ &path.back()->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
            }
            drawAllNodeExceptRoot(path.back());
            drawCodeFadeOut(2);

            return;
        }

        current = current->children[letter];
        path.push_back(current);

        addNewStep();
        for (int i = 0; i < (int)path.size() - 1; i++) {
            if (i == (int)path.size() - 2) {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edge);
                drawEdgeSlideIn({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                drawFadeOut({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            } else {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            }
            drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
            drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
        }
        drawAllEdge(path.back());
        drawAllNode(path.back());
        drawFadeIn({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        if ((int)path.size() == 2) {
            drawCodeChangeLine(0, 3);
        } else {
            drawCodeChangeLine(3, 3);
        }
    }

    addNewStep();
    for (int i = 0; i < (int)path.size() - 1; i++) {
        drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
        drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
        drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
    }
    drawAllEdge(path.back());
    drawAllNodeExceptRoot(path.back());
    if (current->marked) {
        drawChangeColor({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeFocus2, Color::nodeTextFocus1, Color::nodeTextFocus1);
        drawCodeChangeLine(3, 4);
    } else {
        draw({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        drawCodeChangeLine(3, 5);
    }
    
    addNewStep();
    for (int i = 0; i < (int)path.size() - 1; i++) {
        drawEdgeChangeColor({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus, Color::edge);
        if (path[i]->marked) {
            drawChangeColor({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeFocus2, Color::nodeTextFocus2, Color::nodeTextFocus3);
        } else {
            drawChangeColor({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
        }
        drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
        drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
    }
    drawAllEdge(path.back());
    drawAllNode(path.back());
    if (current->marked) {
        drawFadeOut({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus2, Color::nodeTextFocus1);
        drawCodeFadeOut(4);
    } else {
        drawFadeOut({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        drawCodeFadeOut(5);
    }
}

void TrieVisualizer::insert(std::string str) {
    mCode.update({
        "Node cur = root",
        "for each c in s:",
        "    if cur.child[c] == NULL:",
        "        cur.child[c] = new Node()",
        "    cur = cur.child[c]",
        "cur.leaf = TRUE"
    });

    addNewStep();
    drawReformat();
    drawAllEdge(mRoot);
    drawAllNode(mRoot);
    drawFadeIn({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    drawCodeFadeIn(0);

    std::vector<Node*> path;
    Node* current = mRoot;
    path.push_back(current);
    for (char letter : str) {
        bool hasChild = true;
        if (!current->children.count(letter)) {
            hasChild = false;
            current->children[letter] = new Node(GraphicNode(std::string(1, letter)));

            addNewStep();
            drawReformat();
            for (int i = 0; i < (int)path.size() - 1; i++) {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
                drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
            }
            drawAllEdgeExceptChild(path.back(), letter);
            drawEdgeSlideIn({ std::make_pair(&path.back()->value, &path.back()->children[letter]->value) }, Color::edge);
            draw({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            drawAllNodeExceptChild(path.back(), letter);
            drawFadeIn({ &path.back()->children[letter]->value }, Shape::circle, Type::hollow, Color::node, Color::nodeText);
            if ((int)path.size() == 1) {
                drawCodeChangeLine(0, 3);
            } else {
                drawCodeChangeLine(4, 3);
            }
        }

        current = current->children[letter];
        path.push_back(current);

        addNewStep();
        for (int i = 0; i < (int)path.size() - 1; i++) {
            if (i == (int)path.size() - 2) {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edge);
                drawEdgeSlideIn({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                drawFadeOut({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            } else {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            }
            drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
            drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
        }
        drawAllEdge(path.back());
        drawAllNode(path.back());
        drawFadeIn({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        if (hasChild) {
            if ((int)path.size() == 2) {
                drawCodeChangeLine(0, 4);
            } else {
                drawCodeChangeLine(4, 4);
            }
        } else {
            drawCodeChangeLine(3, 4);
        }
    }

    addNewStep();
    for (int i = 0; i < (int)path.size() - 1; i++) {
        drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);

        drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
        drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
    }
    drawAllEdge(path.back());
    drawAllNodeExceptRoot(path.back());
    drawChangeColor({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeFocus2, Color::nodeTextFocus1, Color::nodeTextFocus1);
    drawCodeChangeLine(4, 5);

    current->marked = true;

    addNewStep();
    for (int i = 0; i < (int)path.size() - 1; i++) {
        drawEdgeChangeColor({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus, Color::edge);
        if (path[i]->marked) {
            drawChangeColor({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeFocus2, Color::nodeTextFocus2, Color::nodeTextFocus3);
        } else {
            drawChangeColor({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::node, Color::nodeTextFocus2, Color::nodeText);
        }
        
        drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
        drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
    }
    drawAllEdge(path.back());
    drawAllNode(path.back());
    drawFadeOut({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus2, Color::nodeTextFocus1);
    drawCodeFadeOut(5);
}

void TrieVisualizer::erase(std::string str) {
    mCode.update({
        "if i == len(s):",
        "    if cur.leaf: cur.leaf = FALSE",
        "        return isEmpty(cur.child)",
        "    return FALSE",
        "if erase(cur.child[s[i]], s, i + 1):",
        "    delete cur.child[s[i]]",
        "return isEmpty(cur.child) and !cur.leaf",
    });

    addNewStep();
    drawReformat();
    drawAllEdge(mRoot);
    drawAllNode(mRoot);
    drawFadeIn({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    drawCodeFadeIn(4);

    std::vector<Node*> path;
    Node* current = mRoot;
    path.push_back(current);
    for (char letter : str) {
        if (!current->children.count(letter)) {
            addNewStep();
            for (int i = 0; i < (int)path.size() - 1; i++) {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
                drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
            }
            drawAllEdge(path.back());
            draw({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            drawAllNodeExceptRoot(path.back());
            drawCodeChangeLine(4, 6);

            while ((int)path.size() >= 2) {
                addNewStep();
                for (int i = 0; i < (int)path.size() - 1; i++) {
                    if (i == (int)path.size() - 2) {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edge);
                        drawEdgeSlideOut({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                        drawFadeIn({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    } else {
                        drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                    }
                    drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
                    drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
                }
                drawAllEdge(path.back());
                drawAllNode(path.back());
                drawFadeOut({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                drawCodeChangeLine(6, 6);

                path.pop_back();
            }

            addNewStep();
            drawAllEdge(mRoot);
            drawAllNode(mRoot);
            drawFadeOut({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            drawCodeFadeOut(6);

            return;
        }

        current = current->children[letter];
        path.push_back(current);

        addNewStep();
        for (int i = 0; i < (int)path.size() - 1; i++) {
            if (i == (int)path.size() - 2) {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edge);
                drawEdgeSlideIn({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                drawFadeOut({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            } else {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            }
            drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
            drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
        }
        drawAllEdge(path.back());
        drawAllNode(path.back());
        drawFadeIn({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        drawCodeChangeLine(4, 4);
    }

    addNewStep();
    for (int i = 0; i < (int)path.size() - 1; i++) {
        drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
        drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
        drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
    }
    drawAllEdge(path.back());
    draw({ &path.back()->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
    drawFadeOut({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    drawAllNodeExceptRoot(path.back());
    if (current->marked) {
        drawCodeChangeLine(4, 2);
    } else {
        drawCodeChangeLine(4, 3);
    }

    addNewStep();
    for (int i = 0; i < (int)path.size() - 1; i++) {
        drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
        draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
        drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
        drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
    }
    drawAllEdge(path.back());
    draw({ &path.back()->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
    drawFadeIn({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    drawAllNodeExceptRoot(path.back());
    if (current->marked) {
        drawCodeChangeLine(2, 6);
    } else {
        drawCodeChangeLine(3, 6);
    }

    bool nodeErased = false;
    if (current->marked) {
        current->marked = false;
        bool firstErased = true;
        while ((int)path.size() >= 2 && path.back()->children.empty() && !path.back()->marked) {
            nodeErased = true;

            mErasedNodes.push_back(path.back());
            path.pop_back();
            path.back()->children.erase(mErasedNodes.back()->value.getValue().front());

            addNewStep();
            drawReformat();
            for (int i = 0; i < (int)path.size(); i++) {
                if (i == (int)path.size() - 1) {
                    drawEdgeSlideOut({ std::make_pair(&path[i]->value, &mErasedNodes.back()->value) }, Color::edgeFocus);
                    draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                    drawFadeIn({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
                    drawAllEdge(path[i]);
                    drawAllNodeExceptRoot(path[i]);
                } else {
                    drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                    draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                    drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
                    drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
                }
            }
            drawFadeOut({ &mErasedNodes.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            if (firstErased) {
                drawCodeChangeLine(6, 5);
                firstErased = false;
            } else {
                drawCodeChangeLine(5, 5);
            }
        }
    }

    while ((int)path.size() >= 2) {
        addNewStep();
        for (int i = 0; i < (int)path.size() - 1; i++) {
            if (i == (int)path.size() - 2) {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edge);
                drawEdgeSlideOut({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
                drawFadeIn({ &path[i]->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
            } else {
                drawEdge({ std::make_pair(&path[i]->value, &path[i + 1]->value) }, Color::edgeFocus);
                draw({ &path[i]->value }, Shape::circle, Type::hollow, Color::nodeFocus1, Color::nodeTextFocus2);
            }
            drawAllEdgeExceptChild(path[i], path[i + 1]->value.getValue().front());
            drawAllNodeExceptChild(path[i], path[i + 1]->value.getValue().front());
        }
        drawAllEdge(path.back());
        drawAllNode(path.back());
        drawFadeOut({ &path.back()->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
        if (nodeErased) {
            drawCodeChangeLine(5, 6);
            nodeErased = false;
        } else {
            drawCodeChangeLine(6, 6);
        }

        path.pop_back();
    }

    addNewStep();
    drawAllEdge(mRoot);
    drawAllNode(mRoot);
    drawFadeOut({ &mRoot->value }, Shape::circle, Type::filled, Color::nodeFocus1, Color::nodeTextFocus1);
    if (nodeErased) {
        drawCodeFadeOut(5);
    } else {
        drawCodeFadeOut(6);
    }
}

void TrieVisualizer::run() {
    std::function<bool()> conditionNone = [&]() { return true; };

    mOption.addOption("Create");
    mOption.addSuboption("Empty", conditionNone);
    mOption.addSuboption("Random", conditionNone);
    mOption.addSuboptionInputBox("n",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isIntegerInRange), std::placeholders::_1, std::placeholders::_2, TrieVisualizerData::minSize, TrieVisualizerData::maxSize),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::integerInRange), TrieVisualizerData::minSize, TrieVisualizerData::maxSize)
    );

    mOption.addSuboption("File", conditionNone);
    mOption.addSuboptionFileBox("s", std::bind(Validator::isListOfStringWithLowercaseLetters, std::placeholders::_1, std::placeholders::_2, TrieVisualizerData::minSize, TrieVisualizerData::maxSize, TrieVisualizerData::minLength, TrieVisualizerData::maxLength));

    mOption.addOption("Search");
    mOption.addSuboption("String", conditionNone);
    mOption.addSuboptionInputBox("s",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isStringWithLowercaseLetters), std::placeholders::_1, std::placeholders::_2, TrieVisualizerData::minLength, TrieVisualizerData::maxLength),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::stringOfLowercaseLetters), TrieVisualizerData::minLength, TrieVisualizerData::maxLength)
    );

    mOption.addOption("Insert");
    mOption.addSuboption("String", conditionNone);
    mOption.addSuboptionInputBox("s",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isStringWithLowercaseLetters), std::placeholders::_1, std::placeholders::_2, TrieVisualizerData::minLength, TrieVisualizerData::maxLength),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::stringOfLowercaseLetters), TrieVisualizerData::minLength, TrieVisualizerData::maxLength)
    );

    mOption.addOption("Erase");
    mOption.addSuboption("String", conditionNone);
    mOption.addSuboptionInputBox("s",
        std::bind(static_cast<std::string(*)(std::string, std::string, int, int)>(Validator::isStringWithLowercaseLetters), std::placeholders::_1, std::placeholders::_2, TrieVisualizerData::minLength, TrieVisualizerData::maxLength),
        std::bind(static_cast<std::string(*)(int, int)>(Randomizer::stringOfLowercaseLetters), TrieVisualizerData::minLength, TrieVisualizerData::maxLength)
    );

    mOption.processOption();

    clearAllSteps();
    create(TrieVisualizerData::defaultSize);

    while (mWindow->isOpen()) {
        updateState();

        sf::Event event;
        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                freeMemory();
                mWindow->close();
            }

            VisualizerData::Event action = handleEvent(event);
            if (action == VisualizerData::Event::quit) {
                freeMemory();
                return;
            }

            if (action == VisualizerData::Event::confirm) {
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

                    case 2: // File
                        createFromList(values[0]);
                        break;
                    }
                    
                    break;

                case 1: // Search
                    switch (option.second) {
                    case 0:
                        search(values[0]);
                        break;
                    }

                    break;

                case 2: // Insert
                    switch (option.second) {
                    case 0:
                        insert(values[0]);
                        break;
                    }

                    break;

                case 3: // Erase
                    switch (option.second) {
                    case 0:
                        erase(values[0]);
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